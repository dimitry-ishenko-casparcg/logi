# Logitech R800 control for CasparCG Client

This program enables control of [CasparCG Client](https://casparcg.com/) with the Logitech R800 remote control.

## Getting Started

### Prerequisites

Compilation:
* [asio C++ Library](https://think-async.com/) >= 1.10.10
* CMake >= 3.1

NB: asio 1.12.1 has a bug that may cause SIGSEGV due to null pointer deference, when using callbacks. It is recommended that you install an unofficial version 1.10.10 from [here](https://github.com/dimitry-ishenko-cpp/asio/releases/tag/asio-1-10-10).

Operation:
* [CasparCG Client](https://casparcg.com/)

### Installation

Binary (Debian/Ubuntu/etc):
```console
$ wget https://github.com/dimitry-ishenko-casparcg/logi/releases/download/v1.1/logi_1.1_Linux_x86_64.deb
$ sudo apt install ./logi_1.1_Linux_x86_64.deb
```

Compile from source:
```console
$ wget https://github.com/dimitry-ishenko-casparcg/logi/releases/download/v1.1/logi-1.1-Source.tar.bz2
$ tar xjf logi-1.1-Source.tar.bz2
$ mkdir logi-1.1-Source/build
$ cd logi-1.1-Source/build
$ cmake .. -DCMAKE_INSTALL_PREFIX=/usr
$ make
$ sudo make install
```

Clone and compile from repository:
```console
$ git clone https://github.com/dimitry-ishenko-casparcg/logi.git
$ mkdir logi/build
$ cd logi/build
$ cmake .. -DCMAKE_INSTALL_PREFIX=/usr
$ make
$ sudo make install
```

### Usage

1. Launch CasparCG Client.
2. Load desired playlist.
3. Turn on R800.
4. Control CasparCG Client as follows:

| R800     | CasparCG  |
|:--------:|:---------:|
| (left)   | Up + F2   |
| (right)  | Down + F2 |
| (play)   | Home + F2 |
| (screen) | End + F2  |

## Authors

* **Dimitry Ishenko** - dimitry (dot) ishenko (at) (gee) mail (dot) com

## License

This project is distributed under the GNU GPL license. See the
[LICENSE.md](LICENSE.md) file for details.
