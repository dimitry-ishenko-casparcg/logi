# Logitech R800 control for CasparCG Client

This program enables control of the [CasparCG Client](https://github.com/CasparCG/client) using the
[Logitech R800](https://www.logitech.com/en-us/products/presenters/r800-professional-presenter.html) remote control.

## Installation

### Binary

Debian/Ubuntu/etc:

```shell
$ ver=0.0
$ url=https://github.com/.../.../releases/download/v${ver}
$ wget ${url}/..._${ver}_amd64.deb
$ sudo apt install ./..._${ver}_amd64.deb
```

RaspberryPi:

```shell
$ ver=0.0
$ url=https://github.com/.../.../releases/download/v${ver}
$ wget ${url}/..._${ver}_armhf.deb
$ sudo apt install ./..._${ver}_armhf.deb
```

### From source

Stable version (requires [CMake](https://cmake.org/) >= 3.1 and [asio](https://think-async.com/Asio/)):

```shell
$ ver=0.0
$ wget https://github.com/.../.../archive/v${ver}.tar.gz
$ tar xzf v${ver}.tar.gz
$ mkdir ...-${ver}/build
$ cd ...-${ver}/build
$ cmake ..
$ make
$ sudo make install
```

Latest master (requires [git](https://git-scm.com/), [CMake](https://cmake.org/) >= 3.1 and [asio](https://think-async.com/Asio/)):

```shell
$ git clone https://github.com/.../....git
$ mkdir .../build
$ cd .../build
$ cmake ..
$ make
$ sudo make install
```

## Authors

* **Dimitry Ishenko** - dimitry (dot) ishenko (at) (gee) mail (dot) com

## License

This project is distributed under the GNU GPL license. See the
[LICENSE.md](LICENSE.md) file for details.
