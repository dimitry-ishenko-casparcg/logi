////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2020-2021 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#include "util.hpp"

#include <csignal>
#include <cstdlib>

////////////////////////////////////////////////////////////////////////////////
namespace src
{

////////////////////////////////////////////////////////////////////////////////
void on_interrupt(interrupt_callback cb)
{
    static interrupt_callback cb_;

    if(!cb_)
    {
        std::signal(SIGINT, [](int signal) { cb_(signal); });
        std::signal(SIGTERM, [](int signal) { cb_(signal); });
    }

    cb_ = std::move(cb);

    if(!cb_)
    {
        std::signal(SIGINT, SIG_DFL);
        std::signal(SIGTERM, SIG_DFL);
    }
}

////////////////////////////////////////////////////////////////////////////////
}
