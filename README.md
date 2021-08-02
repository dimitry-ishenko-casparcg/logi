# logi - Logitech R800 to OSC translator

**logi** intercepts button presses from
[Logitech R800](https://www.logitech.com/en-us/products/presenters/r800-professional-presenter.html)
remote control and translates them into OSC messages as follows:

|Button | OSC message                           |
|:-----:|---------------------------------------|
| ` < ` | `/remote/logitech/r800/press` `prev`  |
| ` > ` | `/remote/logitech/r800/press` `next`  |
| ` 🞂 ` | `/remote/logitech/r800/press` `start` |
| ` 🞂 ` | `/remote/logitech/r800/press` `stop`  |
| ` ⎚ ` | `/remote/logitech/r800/press` `black` |

NB: On each press the ` 🞂 ` button on R800 alternates between the `start` and
the `stop` commands.

By default **logi** will send messages to an OSC server on IP address
`127.0.0.1` and port `6260`. These can be changed with the `--address` and
`--port` options respectively.

In order to do that, you have to override them in the `logi@.service` file. For
example:

```shell
sudo systemctl edit logi@.service
```

and add the following lines:

```ini
[Service]
Environment="args=--address=10.0.42.123 --port=4567"
```

## Installation

### Binary

Requires [libosc++](https://github.com/dimitry-ishenko-cpp/liboscpp) >= 1.0.

Debian/Ubuntu/etc:

```shell
$ p=logi
$ v=3.0
$ wget https://github.com/dimitry-ishenko-casparcg/${p}/releases/download/v${v}/${p}_${v}_amd64.deb
$ sudo apt install ./${p}_${v}_amd64.deb
```

RaspberryPi:

```shell
$ p=logi
$ v=3.0
$ wget https://github.com/dimitry-ishenko-casparcg/${p}/releases/download/v${v}/${p}_${v}_armhf.deb
$ sudo apt install ./${p}_${v}_armhf.deb
```

### From source

Stable version (requires [CMake](https://cmake.org/) >= 3.1,
[asio](https://think-async.com/Asio/) and
[libosc++-dev](https://github.com/dimitry-ishenko-cpp/liboscpp) >= 1.0):

```shell
$ p=logi
$ v=3.0
$ wget https://github.com/dimitry-ishenko-casparcg/${p}/releases/download/v${v}/${p}-${v}.tar.bz2
$ tar xjf ${p}-${v}.tar.bz2
$ mkdir ${p}-${v}/build
$ cd ${p}-${v}/build
$ cmake ..
$ make
$ sudo make install
```

Latest master (requires [git](https://git-scm.com/),
[CMake](https://cmake.org/) >= 3.1, [asio](https://think-async.com/Asio/) and
[libosc++-dev](https://github.com/dimitry-ishenko-cpp/liboscpp) >= 1.0):

```shell
$ p=logi
$ git clone --recursive https://github.com/dimitry-ishenko-casparcg/${p}.git
$ mkdir ${p}/build
$ cd ${p}/build
$ cmake ..
$ make
$ sudo make install
```

## Authors

* **Dimitry Ishenko** - dimitry (dot) ishenko (at) (gee) mail (dot) com

## License

This project is distributed under the GNU GPL license. See the
[LICENSE.md](LICENSE.md) file for details.
