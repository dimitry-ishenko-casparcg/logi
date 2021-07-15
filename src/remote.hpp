////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2021 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#ifndef SRC_REMOTE_HPP
#define SRC_REMOTE_HPP

////////////////////////////////////////////////////////////////////////////////
#include <asio.hpp>
#include <filesystem>
#include <functional>
#include <memory>

namespace fs = std::filesystem;

struct input_event;

////////////////////////////////////////////////////////////////////////////////
namespace src
{

enum button { prev, next, start, stop, black };

using press_callback = std::function<void(button)>;

////////////////////////////////////////////////////////////////////////////////
class remote
{
public:
    remote(asio::io_context&, fs::path);
    ~remote();

    void on_press(press_callback cb) { cb_ = std::move(cb); }

private:
    fs::path path_;
    asio::posix::stream_descriptor sd_;

    std::unique_ptr<input_event> ev_;
    void sched_read();

    press_callback cb_;

    asio::steady_timer timer_;
    void sched_check();
};

////////////////////////////////////////////////////////////////////////////////
}

////////////////////////////////////////////////////////////////////////////////
#endif
