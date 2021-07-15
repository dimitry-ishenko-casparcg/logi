////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2018-2021 Dimitry Ishenko
// Contact: dimitry (dot) ishenko (at) (gee) mail (dot) com
//
// Distributed under the GNU GPL license. See the LICENSE.md file for details.

////////////////////////////////////////////////////////////////////////////////
#include "pgm/args.hpp"
#include "remote.hpp"
#include "sender.hpp"
#include "util.hpp"

#include <asio.hpp>
#include <exception>
#include <filesystem>
#include <iostream>
#include <string>

#include <linux/input.h>

namespace fs = std::filesystem;

#if !defined(VERSION)
#  define VERSION "0"
#endif

////////////////////////////////////////////////////////////////////////////////
int main(int argc, char* argv[])
try
{
    auto name{ fs::path(argv[0]).filename() };

    pgm::args args
    {
        { "-h", "--help",    "Print this help screen and exit." },
        { "-v", "--version", "Show version number and exit."    },
        { "path",            "Path to Logitech R800 device."    },
    };

    // delay exception handling to process --help and --version
    std::exception_ptr ep;
    try { args.parse(argc, argv); }
    catch(...) { ep = std::current_exception(); }

    if(args["--help"])
    {
        std::cout << '\n' << args.usage(name) << '\n' << std::endl;
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

        asio::io_context io;

        src::remote remote{ io, path };
        src::sender sender{ io };

        remote.on_press([&](src::button b)
        {
            switch(b)
            {
            case src::prev :
                sender.send(KEY_UP);
                sender.send(KEY_F2);
                break;

            case src::next :
                sender.send(KEY_DOWN);
                sender.send(KEY_F2);
                break;

            case src::start:
            case src::stop :
                sender.send(KEY_HOME);
                sender.send(KEY_F2);
                break;

            case src::black:
                sender.send(KEY_END);
                sender.send(KEY_F2);
                break;
            }
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
