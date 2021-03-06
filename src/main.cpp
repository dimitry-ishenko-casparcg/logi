////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2018 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#include "posix/error.hpp"
#include "util/logging.hpp"

#include <asio.hpp>
#include <asio/steady_timer.hpp>
#include <chrono>
#include <csignal>
#include <functional>
#include <random>
#include <stdexcept>
#include <string>
#include <vector>

#include <fcntl.h>
#include <linux/uinput.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

using namespace std::chrono_literals;
using namespace util::logging;

////////////////////////////////////////////////////////////////////////////////
template<unsigned n, typename T = void>
struct io_cmd
{
    constexpr auto name() const noexcept { return n; }
    constexpr void* data() noexcept { return reinterpret_cast<void*>(data_); }
    T data_;
};

template<unsigned n, typename T>
struct io_cmd<n, T*>
{
    constexpr auto name() const noexcept { return n; }
    constexpr void* data() noexcept { return &data_; }
    T data_;
};

template<unsigned n>
struct io_cmd<n, void>
{
    constexpr auto name() const noexcept { return n; }
    constexpr void* data() noexcept { return nullptr; }
};

////////////////////////////////////////////////////////////////////////////////
auto open_input(asio::io_service& io, const std::string& path)
{
    info() << "Opening " << path;
    auto fd = ::open(path.data(), O_RDONLY);
    if(fd < 0) throw posix::errno_error();

    asio::posix::stream_descriptor desc(io);
    desc.assign(fd);

    info() << "Grabbing device";
    io_cmd<EVIOCGRAB, int> grab { 1 };
    desc.io_control(grab);

    return std::move(desc);
}

////////////////////////////////////////////////////////////////////////////////
auto open_output(asio::io_service& io, int n)
{
    info() << "Opening uinput device";
    auto fd = ::open("/dev/uinput", O_WRONLY | O_NONBLOCK);
    if(fd < 0) throw posix::errno_error();

    asio::posix::stream_descriptor desc(io);
    desc.assign(fd);

    ////////////////////
    io_cmd<UI_SET_EVBIT, int> set_ev { EV_KEY };
    desc.io_control(set_ev);

    for(int code = 1; code < 255; ++code)
    {
        io_cmd<UI_SET_KEYBIT, int> set_code { code };
        desc.io_control(set_code);
    }

    io_cmd<UI_DEV_SETUP, uinput_setup*> setup
    {
        {
            { BUS_USB, 0x42, static_cast<__u16>(n), 1 }, // bus, vid, pid, version
            "Logitech"
        }
    };
    desc.io_control(setup);

    io_cmd<UI_DEV_CREATE> create;
    desc.io_control(create);

    return std::move(desc);
}

////////////////////////////////////////////////////////////////////////////////
void send_code(asio::posix::stream_descriptor& uinput, int code)
{
    static input_event event { { 0, 0 }, EV_KEY };
    static const input_event sync { { 0, 0 }, EV_SYN, SYN_REPORT, 0 };

    dbg() << "Sending code " << code;

    event.code = static_cast<__u16>(code);
    event.value = 1;
    asio::write(uinput, asio::buffer(&event, sizeof(event)));
    asio::write(uinput, asio::buffer(&sync, sizeof(sync)));

    event.value = 0;
    asio::write(uinput, asio::buffer(&event, sizeof(event)));
    asio::write(uinput, asio::buffer(&sync, sizeof(sync)));
}

////////////////////////////////////////////////////////////////////////////////
void throw_usage(const std::string& name)
{
    throw std::runtime_error(
        "Usage: " + name + " [-s|--syslog] /path/to/device"
    );
}

////////////////////////////////////////////////////////////////////////////////
int main(int argc, char* argv[])
try
{
    std::string name = argv[0], path;

    for(int n = 1; n < argc; ++n)
    {
        std::string arg = argv[n];
        if(arg == "-s" || arg == "--syslog")
        {
            util::send_to_syslog(true);
            util::send_to_console(false);
        }
        else if(path.size())
            throw_usage(argv[0]);
        else path = arg;
    }
    if(path.empty()) throw_usage(argv[0]);

    ////////////////////
    std::mt19937 rng;
    rng.seed(std::random_device()());
    std::uniform_int_distribution<> dist(0, 9999);

    asio::io_service io;

    auto remote = open_input(io, path);
    auto uinput = open_output(io, dist(rng));

    ////////////////////
    input_event event;
    auto buffer = asio::buffer(&event, sizeof(event));

    std::function<void(const asio::error_code&, std::size_t)> recv_event =
        [&](const asio::error_code& ec, std::size_t)
        {
            if(ec) return;

            if(event.type == EV_KEY && event.value == 1)
            {
                dbg() << "Received code " << event.code;
                switch(event.code)
                {
                case KEY_PAGEUP:
                    send_code(uinput, KEY_UP);
                    send_code(uinput, KEY_F2);
                    break;

                case KEY_PAGEDOWN:
                    send_code(uinput, KEY_DOWN);
                    send_code(uinput, KEY_F2);
                    break;

                case KEY_F5:
                case KEY_ESC:
                    send_code(uinput, KEY_HOME);
                    send_code(uinput, KEY_F2);
                    break;

                case KEY_DOT:
                    send_code(uinput, KEY_END);
                    send_code(uinput, KEY_F2);
                    break;
                }
            }

            asio::async_read(remote, buffer, recv_event);
        };

    asio::async_read(remote, buffer, recv_event);

    ////////////////////
    asio::steady_timer timer(io);
    std::function<void(const asio::error_code&)> stat_remote =
        [&](const asio::error_code& ec)
        {
            if(ec) return;

            struct stat sb;
            if(::stat(path.data(), &sb))
            {
                warn() << "Device " << path << " disappeared";
                std::raise(SIGTERM);
            }
            else
            {
                timer.expires_from_now(1s);
                timer.async_wait(stat_remote);
            }
        };

    timer.expires_from_now(1s);
    timer.async_wait(stat_remote);

    ////////////////////
    asio::signal_set signals(io, SIGINT, SIGTERM);
    signals.async_wait([&](const asio::error_code& ec, int n)
    {
        if(ec) return;

        warn() << "Received signal " << n << " - terminating";
        remote.close();
        timer.cancel();
    });

    ////////////////////
    dbg() << "Starting event loop";
    io.run();
    dbg() << "Exited event loop";

    return 0;
}
catch(std::exception& e)
{
    err() << e.what();
    return 1;
}
