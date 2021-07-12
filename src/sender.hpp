////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2021 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#ifndef SRC_SENDER_HPP
#define SRC_SENDER_HPP

////////////////////////////////////////////////////////////////////////////////
#include <asio.hpp>
#include <string>

////////////////////////////////////////////////////////////////////////////////
namespace src
{

class sender
{
public:
    explicit sender(asio::io_context&);

    void send(int);

private:
    asio::posix::stream_descriptor sd_;
};

////////////////////////////////////////////////////////////////////////////////
}

////////////////////////////////////////////////////////////////////////////////
#endif
