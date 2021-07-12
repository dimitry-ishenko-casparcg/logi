////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2021 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#include "sender.hpp"

#include <cerrno>
#include <iostream>
#include <random>
#include <system_error>

#include <linux/uinput.h>

////////////////////////////////////////////////////////////////////////////////
namespace src
{

////////////////////////////////////////////////////////////////////////////////
namespace
{

struct enable_send_keys
{
    auto name() { return UI_SET_EVBIT; }
    auto data() { return reinterpret_cast<void*>(EV_KEY); }
};

struct enable_send_key
{
    int code;
    auto name() { return UI_SET_KEYBIT; }
    auto data() { return reinterpret_cast<void*>(code); }
};

struct setup_device
{
    uinput_setup setup;
    auto name() { return UI_DEV_SETUP; }
    auto data() { return reinterpret_cast<void*>(&setup); }
};

struct create_device
{
    auto name() { return UI_DEV_CREATE; }
    auto data() { return reinterpret_cast<void*>(0); }
};

}

////////////////////////////////////////////////////////////////////////////////
sender::sender(asio::io_context& io) :
    sd_{ io }
{
    std::cout << "Opening device \"/dev/uinput\"." << std::endl;
    auto fd = ::open("/dev/uinput", O_RDONLY);

    if(fd == -1) throw std::system_error{
        std::error_code{ errno, std::generic_category() }
    };
    sd_.assign(fd);

    enable_send_keys cmd;
    sd_.io_control(cmd);

    for(auto code = 1; code < 255; ++code)
    {
        enable_send_key cmd{ code };
        sd_.io_control(cmd);
    }

    std::mt19937 rng;
    rng.seed(std::random_device{ }());
    std::uniform_int_distribution<> dist{ 0, 9999 };

    std::uint16_t vid = 0x42, pid = dist(rng), ver = 1;

    setup_device cmd2{ { { BUS_USB, vid, pid, ver }, "logi", 0 } };
    sd_.io_control(cmd2);

    create_device cmd3;
    sd_.io_control(cmd3);
}

////////////////////////////////////////////////////////////////////////////////
void sender::send(int code)
{
    input_event event{ { }, EV_KEY, static_cast<std::uint16_t>(code), 1 };
    asio::write(sd_, asio::buffer(&event, sizeof(event)));

    input_event sync{ { }, EV_SYN, SYN_REPORT, 0 };
    asio::write(sd_, asio::buffer(&sync, sizeof(sync)));

    event.value = 0;
    asio::write(sd_, asio::buffer(&event, sizeof(event)));
    asio::write(sd_, asio::buffer(&sync, sizeof(sync)));
}

////////////////////////////////////////////////////////////////////////////////
}
