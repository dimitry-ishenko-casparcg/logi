////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2018-2021 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#include "pgm/args.hpp"
#include "remote.hpp"
#include "util.hpp"

#include <asio.hpp>
#include <exception>
#include <filesystem>
#include <iostream>
#include <osc++.hpp>
#include <string>

namespace fs = std::filesystem;

#if !defined(VERSION)
#  define VERSION "0"
#endif

////////////////////////////////////////////////////////////////////////////////
auto to_address(const std::string& s)
{
    asio::error_code ec;
    auto address = asio::ip::make_address(s, ec);

    if(!ec) return address;
    else throw pgm::invalid_argument{ "Invalid IP address", s };
}

////////////////////////////////////////////////////////////////////////////////
auto to_port(const std::string& s)
{
    char* end;
    auto ul = std::strtoul(s.data(), &end, 0);

    if(ul <= UINT16_MAX && end == (s.data() + s.size()))
        return static_cast<std::uint16_t>(ul);
    else throw pgm::invalid_argument{ "Invalid port number", s };
}

////////////////////////////////////////////////////////////////////////////////
int main(int argc, char* argv[])
try
{
    auto name = fs::path{ argv[0] }.filename();

    std::string def_address = "127.0.0.1";
    std::string def_port = "6260";

    pgm::args args
    {{
        { "-a", "--address", "addr", "Specify OSC server IP address to send messages to.\n"
                                     "Default: " + def_address + "."    },
        { "-p", "--port", "N",       "Specify OSC server port number. Default: " + def_port + "." },
        { "-h", "--help",            "Print this help screen and exit." },
        { "-v", "--version",         "Show version number and exit."    },

        { "path",                    "Path to Logitech R800 device."    },
    }};

    // delay exception handling to process --help and --version
    std::exception_ptr ep;
    try { args.parse(argc, argv); }
    catch(...) { ep = std::current_exception(); }

    if(args["--help"])
    {
        std::cout << "\n" << args.usage(name) << "\n" << std::endl;
    }
    else if(args["--version"])
    {
        std::cout << name.string() << " version " << VERSION << std::endl;
    }
    else if(ep)
    {
        std::rethrow_exception(ep);
    }
    else
    {
        fs::path path{ args["path"].value() };

        asio::ip::udp::endpoint ep{
            to_address(args["--address"].value_or(def_address)),
            to_port(args["--port"].value_or(def_port))
        };

        asio::io_context io;
        asio::ip::udp::socket socket{ io };
        socket.open(asio::ip::udp::v4());

        src::remote remote{ io, path };
        remote.on_press([&](src::button b)
        {
            osc::message msg{ "/remote/logitech/r800/press" };

            switch(b)
            {
            case src::prev : msg <<  "prev"; break;
            case src::next : msg <<  "next"; break;
            case src::start: msg << "start"; break;
            case src::stop : msg <<  "stop"; break;
            case src::black: msg << "black"; break;
            default: return;
            }

            auto packet = msg.to_packet();
            socket.send_to(asio::buffer(packet.data(), packet.size()), ep);
        });

        src::on_interrupt([&](int signal)
        {
            std::cout << "Received signal " << signal << " - exiting." << std::endl;
            io.stop();
        });

        std::cout << "Starting event loop." << std::endl;
        io.run();
    }

    return 0;
}
catch(std::exception& e)
{
    std::cerr << e.what() << std::endl;
    return 1;
}
catch(...)
{
    std::cerr << "???" << std::endl;
    return 1;
}
