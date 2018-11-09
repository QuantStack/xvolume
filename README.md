# ![xvolume](docs/source/xvolume.svg)

[![Travis](https://travis-ci.org/QuantStack/xvolume.svg?branch=master)](https://travis-ci.org/QuantStack/xvolume)
[![Appveyor](https://ci.appveyor.com/api/projects/status/733j3qm6kn3sh0b4?svg=true)](https://ci.appveyor.com/project/QuantStack/xvolume)
[![Join the Gitter Chat](https://badges.gitter.im/Join%20Chat.svg)](https://gitter.im/QuantStack/Lobby?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge)

C++ backend for the [ipyvolume](https://github.com/maartenbreddels/ipyvolume) library.

**xvolume is an early developer preview, and is not suitable for general usage yet. Features and implementation are subject to change.**

## Installation

From sources (if you have all the dependencies already installed):

```bash
cmake -D CMAKE_INSTALL_PREFIX=your_install_prefix
make install
```

## Dependencies

All the dependencies of xvolume are available for the conda package manager.

| `xvolume` | `xwidgets`    |  `xeus`         |
|-----------|---------------|-----------------|
|   master  | >=0.14.0<0.15 | >=0.15.0<0.16   |

## License

We use a shared copyright model that enables all contributors to maintain the
copyright on their contributions.

This software is licensed under the BSD-3-Clause license. See the [LICENSE](LICENSE) file for details.

