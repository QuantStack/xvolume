# ![xvolume](docs/source/xvolume.svg)

[![Travis](https://travis-ci.org/QuantStack/xvolume.svg?branch=master)](https://travis-ci.org/QuantStack/xvolume)
[![Appveyor](https://ci.appveyor.com/api/projects/status/733j3qm6kn3sh0b4?svg=true)](https://ci.appveyor.com/project/QuantStack/xvolume)
[![Documentation](http://readthedocs.org/projects/xvolume/badge/?version=latest)](https://xvolume.readthedocs.io/en/latest/?badge=latest)
[![Binder](https://img.shields.io/badge/launch-binder-brightgreen.svg)](https://mybinder.org/v2/gh/QuantStack/xvolume/stable?filepath=notebooks)
[![Join the Gitter Chat](https://badges.gitter.im/Join%20Chat.svg)](https://gitter.im/QuantStack/Lobby?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge)

C++ backend for the bqplot 2-D plotting library.

**xvolume is an early developer preview, and is not suitable for general usage yet. Features and implementation are subject to change.**

## Introduction

## Installation

We provide a package for the conda package manager.

- Installing `xvolume` and the C++ kernel

```bash
conda install xeus-cling xvolume -c QuantStack -c conda-forge
```

Then, the front-end extension must be installed for either the classic notebook
or JupyterLab.

- Installing the extensions for the classic notebook

```
conda install widgetsnbextension -c conda-forge
conda install bqplot -c conda-forge
```

- Installing the JupyterLab extension

```
jupyter labextension install @jupyter-widgets/jupyterlab-manager
jupyter labextension install bqplot
```

Or you can directly install it from the sources if you have all the
dependencies already installed:

```bash
cmake -D CMAKE_INSTALL_PREFIX=your_install_prefix
make install
```

## Trying it online

To try out xvolume interactively in your web browser, just click on the binder
link:
<!-- 
[![Binder](docs/source/binder-logo.svg)](https://mybinder.org/v2/gh/QuantStack/xvolume/stable?filepath=notebooks)
 -->

## Documentation

To get started with using `xvolume`, check out the full documentation

http://xvolume.readthedocs.io/

## Usage

...

## Dependencies

All the dependencies of xvolume are available for the conda package manager.

| `xvolume` | `xwidgets`  |  `xeus`         |
|-----------|-------------|-----------------|

## License

We use a shared copyright model that enables all contributors to maintain the
copyright on their contributions.

This software is licensed under the BSD-3-Clause license. See the [LICENSE](LICENSE) file for details.

