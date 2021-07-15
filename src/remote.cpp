////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2021 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#include "remote.hpp"

#include <cerrno>
#include <chrono>
#include <csignal>
#include <iostream>
#include <system_error>

#include <fcntl.h> // open
#include <linux/input.h>

using namespace std::chrono_literals;

////////////////////////////////////////////////////////////////////////////////
namespace src
{

////////////////////////////////////////////////////////////////////////////////
namespace
{

struct grab_device
{
    auto name() { return EVIOCGRAB; }
    auto data() { return reinterpret_cast<void*>(1); }
};

struct release_device
{
    auto name() { return EVIOCGRAB; }
    auto data() { return reinterpret_cast<void*>(0); }
};

}

////////////////////////////////////////////////////////////////////////////////
remote::remote(asio::io_context& io, fs::path path) :
    path_{ std::move(path) }, sd_{ io }, ev_{ new input_event{ } }, timer_{ io }
{
    std::cout << "Opening device " << path_ << "." << std::endl;
    auto fd = ::open(path_.c_str(), O_RDONLY);

    if(fd == -1) throw std::system_error{
        std::error_code{ errno, std::generic_category() }
    };
    sd_.assign(fd);

    grab_device cmd;
    sd_.io_control(cmd);

    sched_read();
    sched_check();
}

////////////////////////////////////////////////////////////////////////////////
remote::~remote()
{
    release_device cmd;
    asio::error_code ec;

    sd_.io_control(cmd, ec);
}

////////////////////////////////////////////////////////////////////////////////
void remote::sched_read()
{
    auto buffer = asio::buffer(&*ev_, sizeof(*ev_));
    asio::async_read(sd_, buffer, [&](const asio::error_code& ec, std::size_t)
    {
        if(ec) return;
        if(ev_->type == EV_KEY && ev_->value == 1 && cb_)
            switch(ev_->code)
            {
            case KEY_PAGEUP  : cb_(prev ); break;
            case KEY_PAGEDOWN: cb_(next ); break;
            case KEY_F5      : cb_(start); break;
            case KEY_ESC     : cb_(stop ); break;
            case KEY_DOT     : cb_(black); break;
            }

        sched_read();
    });
}

////////////////////////////////////////////////////////////////////////////////
void remote::sched_check()
{
    timer_.expires_from_now(1000ms);
    timer_.async_wait([&](const asio::error_code& ec)
    {
        if(ec) return;

        if(!fs::exists(path_))
        {
            std::cout << "Device " << path_ << " no longer exists." << std::endl;
            std::raise(SIGTERM);
        }
        else sched_check();
    });
}

////////////////////////////////////////////////////////////////////////////////
}
