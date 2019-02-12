////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2018 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#include "posix/error.hpp"

#include <asio.hpp>
#include <functional>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include <fcntl.h>
#include <linux/uinput.h>
#include <sys/stat.h>
#include <sys/types.h>

////////////////////////////////////////////////////////////////////////////////
template<typename T, unsigned n>
struct io_cmd
{
    constexpr auto name() const noexcept { return n; }
    constexpr void* data() noexcept { return &data_; }
    T data_;
};

////////////////////////////////////////////////////////////////////////////////
int main(int argc, char* argv[])
try
{
    ////////////////////
    if(argc != 2) throw std::runtime_error(
        std::string() + "Usage: " + argv[0] + " /path/to/device"
    );

    asio::io_service io;

    ////////////////////
    std::cout << "Opening input device: " << argv[1] << std::endl;
    auto fd = ::open(argv[1], O_RDONLY);
    if(fd < 0) throw posix::errno_error();

    asio::posix::stream_descriptor logi(io);
    logi.assign(fd);

    std::cout << "Grabbing input device" << std::endl;
    io_cmd<int, EVIOCGRAB> cmd { 1 };
    logi.io_control(cmd);

    ////////////////////
    std::cout << "Opening output device: /dev/uinput" << std::endl;
    fd = ::open("/dev/uinput", O_WRONLY | O_NONBLOCK);
    if(fd < 0) throw posix::errno_error();

    asio::posix::stream_descriptor uinput(io);
    uinput.assign(fd);

    ////////////////////
    std::vector<char> buffer(sizeof(input_event));
    std::function<void(const asio::error_code& ec, std::size_t)> read_input;

    read_input = [&](const asio::error_code& ec, std::size_t)
    {
        if(ec) return;

        auto event = reinterpret_cast<input_event*>(buffer.data());
        if(event->type == EV_KEY && event->value == 1)
        {
            std::cout << "Received key code: " << event->code << std::endl;
            switch(event->code)
            {
            case KEY_PAGEUP:
                break;

            case KEY_PAGEDOWN:
                break;

            case KEY_F5:
            case KEY_ESC:
                break;

            case KEY_DOT:
                break;
            }
        }

        asio::async_read(logi, asio::buffer(buffer), read_input);
    };

    asio::async_read(logi, asio::buffer(buffer), read_input);

    ////////////////////
    std::cout << "Starting event loop" << std::endl;
    io.run();

    std::cout << "Exited event loop" << std::endl;
    return 0;
}
catch(std::exception& e)
{
    std::cerr << e.what() << std::endl;
    return 1;
}
