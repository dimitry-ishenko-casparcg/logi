# Logitech R800 control for CasparCG Client

This program enables control of the [CasparCG Client](https://github.com/CasparCG/client) using the
[Logitech R800](https://www.logitech.com/en-us/products/presenters/r800-professional-presenter.html) remote control.

## Installation

### Binary

Debian/Ubuntu/etc:

```shell
$ ver=0.0
$ url=https://github.com/dimitry-ishenko-casparcg/logi/releases/download/v${ver}
$ wget ${url}/logi_${ver}_amd64.deb
$ sudo apt install ./logi_${ver}_amd64.deb
```

RaspberryPi:

```shell
$ ver=0.0
$ url=https://github.com/dimitry-ishenko-casparcg/logi/releases/download/v${ver}
$ wget ${url}/logi_${ver}_armhf.deb
$ sudo apt install ./logi_${ver}_armhf.deb
```

### From source

Stable version (requires [CMake](https://cmake.org/) >= 3.1 and [asio](https://think-async.com/Asio/)):

```shell
$ ver=0.0
$ wget https://github.com/dimitry-ishenko-casparcg/logi/archive/v${ver}.tar.gz
$ tar xzf v${ver}.tar.gz
$ mkdir logi-${ver}/build
$ cd logi-${ver}/build
$ cmake ..
$ make
$ sudo make install
```

Latest master (requires [git](https://git-scm.com/), [CMake](https://cmake.org/) >= 3.1 and [asio](https://think-async.com/Asio/)):

```shell
$ git clone https://github.com/dimitry-ishenko-casparcg/logi.git
$ mkdir logi/build
$ cd logi/build
$ cmake ..
$ make
$ sudo make install
```

## Authors

* **Dimitry Ishenko** - dimitry (dot) ishenko (at) (gee) mail (dot) com

## License

This project is distributed under the GNU GPL license. See the
[LICENSE.md](LICENSE.md) file for details.
