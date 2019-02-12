////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2017-2018 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#include "util/logging.hpp"

#include <atomic>
#include <cstdlib>
#include <iostream>

#include <syslog.h>

////////////////////////////////////////////////////////////////////////////////
namespace util
{

////////////////////////////////////////////////////////////////////////////////
namespace { std::atomic<bool> debug_ = std::getenv("DEBUG"); }
void debug(bool enable) noexcept { debug_ = enable; }
bool debug() noexcept { return debug_; }

////////////////////////////////////////////////////////////////////////////////
namespace { std::atomic<bool> trace_ = std::getenv("TRACE"); }
void trace(bool enable) noexcept { trace_ = enable; }
bool trace() noexcept { return trace_; }

////////////////////////////////////////////////////////////////////////////////
namespace { std::atomic<bool> console_ = true; }
void send_to_console(bool enable) noexcept { console_ = enable; }
bool send_to_console() noexcept { return console_; }

////////////////////////////////////////////////////////////////////////////////
namespace { std::atomic<bool> syslog_ = false; }
void send_to_syslog(bool enable) noexcept { syslog_ = enable; }
bool send_to_syslog() noexcept { return syslog_; }

////////////////////////////////////////////////////////////////////////////////
stream::stream(std::string_view tag, level l) : stream(l)
{ if(tag.size()) *this << tag << ": "; }

////////////////////////////////////////////////////////////////////////////////
stream::~stream()
{
    std::ostream* os;
    int prio;

    switch(level_)
    {
    case level::trc:
        if(!trace()) return;
        os = &std::cout; prio = LOG_DEBUG;
        break;

    case level::dbg:
        if(!debug()) return;
        os = &std::cout; prio = LOG_DEBUG;
        break;

    case level::info: os = &std::cout; prio = LOG_INFO; break;
    case level::warn: os = &std::cerr; prio = LOG_WARNING; break;
    case level::err : os = &std::cerr; prio = LOG_ERR; break;
    }

    auto s = str();
    if(send_to_console()) (*os) << s << std::endl;
    if(send_to_syslog()) ::syslog(prio, "%s", s.data());
}

////////////////////////////////////////////////////////////////////////////////
}
