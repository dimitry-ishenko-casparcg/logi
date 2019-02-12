////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2017-2018 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#ifndef UTIL_LOGGING_HPP
#define UTIL_LOGGING_HPP

////////////////////////////////////////////////////////////////////////////////
#include <sstream>
#include <string>
#include <string_view>
#include <utility>

////////////////////////////////////////////////////////////////////////////////
namespace util
{

////////////////////////////////////////////////////////////////////////////////
// Set trace mode.
//
// Unless explicitly disabled by trace(false), the trace mode will also be
// enabled if the TRACE environment variable has been defined.
//
void trace(bool) noexcept;
bool trace() noexcept;

////////////////////////////////////////////////////////////////////////////////
// Set debug mode.
//
// Unless explicitly disabled by debug(false), the debug mode will also be
// enabled if the DEBUG environment variable has been defined.
//
void debug(bool) noexcept;
bool debug() noexcept;

////////////////////////////////////////////////////////////////////////////////
// Send log messages to console.
//
// When enabled, trc, dbg and info messages will be sent to std::cout;
// and warn and err messages will be sent to std::cerr.
//
// trc and dbg messages will only be logged, when their respective modes
// (trace and debug) have been enabled.
//
void send_to_console(bool) noexcept;
bool send_to_console() noexcept;

////////////////////////////////////////////////////////////////////////////////
// Send log messages to syslog.
//
// trc and dbg messages will only be logged, when their respective modes
// (trace and debug) have been enabled.
//
void send_to_syslog(bool) noexcept;
bool send_to_syslog() noexcept;

////////////////////////////////////////////////////////////////////////////////
// log levels
enum class level { trc, dbg, info, warn, err };

////////////////////////////////////////////////////////////////////////////////
// logging stream
//
// Used by the trc(), dbg(), info(), warn(), err() and log() functions.
//
class stream : private std::ostringstream
{
    using base = std::ostringstream;

public:
    ////////////////////
    explicit stream(level l) : level_(l) { }
    stream(std::string_view tag, level);
    ~stream();

    stream(const stream&) = delete;
    stream(stream&& rhs) : base(std::move(rhs)), level_(rhs.level_) { }

    stream& operator=(const stream&) = delete;
    stream& operator=(stream&& rhs)
    {
        *static_cast<base*>(this) = std::move(rhs);
        level_ = rhs.level_;
        return *this;
    }

    ////////////////////
    template<typename T>
    stream& operator<<(T&& value) &
    {
        *static_cast<base*>(this) << std::forward<T>(value);
        return *this;
    }

    template<typename T>
    stream&& operator<<(T&& value) &&
    {
        *static_cast<base*>(this) << std::forward<T>(value);
        return std::move(*this);
    }

    stream& write(const char* s, std::streamsize n) &
    {
        base::write(s, n);
        return *this;
    }

    stream&& write(const char* s, std::streamsize n) &&
    {
        base::write(s, n);
        return std::move(*this);
    }

private:
    ////////////////////
    level level_;
};

////////////////////////////////////////////////////////////////////////////////
// logging functions
//
namespace logging
{

inline auto  trc() { return stream(level::trc ); }
inline auto  dbg() { return stream(level::dbg ); }
inline auto info() { return stream(level::info); }
inline auto warn() { return stream(level::warn); }
inline auto  err() { return stream(level::err ); }

// log with user-defined level
inline auto log(level l) { return stream(l); }

}

////////////////////////////////////////////////////////////////////////////////
// logger base class
//
// Provides logging functions with specific tag to derived classes.
//
class logger
{
public:
    ////////////////////
    logger(const logger&) = default;
    logger(logger&&) = default;

    logger& operator=(const logger&) = default;
    logger& operator=(logger&&) = default;

protected:
    ////////////////////
    explicit logger(std::string tag = std::string()) { this->tag(tag); }

    ////////////////////
    auto const& tag() const noexcept { return tag_; }
    void tag(std::string tag) noexcept { tag_ = std::move(tag); }

    auto  trc() { return stream(tag_, level::trc ); }
    auto  dbg() { return stream(tag_, level::dbg ); }
    auto info() { return stream(tag_, level::info); }
    auto warn() { return stream(tag_, level::warn); }
    auto  err() { return stream(tag_, level::err ); }

    // log with user-defined level
    auto log(util::level level) { return stream(tag_, level); }

private:
    ////////////////////
    std::string tag_;
};

////////////////////////////////////////////////////////////////////////////////
}

////////////////////////////////////////////////////////////////////////////////
#endif
