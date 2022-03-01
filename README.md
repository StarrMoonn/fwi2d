# FWI2D: **_C++_** library for 2D modeling and inversion of seismic waves

- [Description](#Description)
- [Prerequisites](#Prerequisites)
- [Installation](#Installation)
- [Data format](#Data-format)

## Description

This library performs two-dimensional modeling and inversion of seismic data using fourth-order summation-by-parts finite-difference operators on a regular cartesian grid. Four main configurations are available: acoustic (variable density), elastic isotropic, elastic VTI, and isotropic acoustic-elastic coupled medium with a flat interface. Auxiliary programs are also provided such as wavelet generation, spectra computation, B-spline smoothing etc ...

Key features include:

* incorporated linear and non-linear solvers (including non-linear CG and l-BFGS)
* many practical options for full-waveform inversion (FWI)
* auxiliary programs such as wavelet generation, spectra computation, B-spline smoothing etc ...
* **_MPI_** support for parallelization over shots
* **_GPU_** acceleration with **_CUDA_** (currently for the elastic isotropic and VTI configurations only)

## Prerequisites

The **fwi2d** library is written almost entirely in **_C++_** and **_CUDA_** and has been built on Linux environments (**centos 7**) using **_CMAKE_**. A portion of the code is compiled using **_ISPC_** which needs to be installed *a priori*. Downloading the binary directly from [here](https://ispc.github.io/) should suffice. Moreover, **_Python_** and **_Jupyter_** are only required for data format conversion and examples generation.

## Installation

Start by downloading **_ISPC_** binary (or building it from source code) and copying it to the desired location `path_to_ispc_binary`
```
# for Linux 64 bit
wget https://github.com/ispc/ispc/releases/download/v1.17.0/ispc-v1.17.0-linux.tar.gz
tar -xvf ispc-v1.17.0-linux.tar.gz
cp ispc-v1.17.0-linux/bin/ispc path_to_ispc_binary/ispc
```

Then clone and install the main **fwi2d** library
```
# get the code
git clone http://cees-gitlab.stanford.edu/nmbader/fwi2d.git
cd fwi2d

# create subdirectories
mkdir build local

# Build the external SEPlib library needed for the IO
cd external/SEP
bash ./buildit.sh

# When it is done, build the main library
cd ../../build
cmake -DCMAKE_INSTALL_PREFIX=../local -DISPC=path_to_ispc_binary/ispc ../
make -j12
make install

# clean up the build directory
rm -rf *
```
By default, the **fwi2d** library is built in single precision. For double precision, add the flag `-DDOUBLE_PRECISION=1`
 to the **_cmake_** command. 

All executables (and python scripts) will be installed into the subdirectory *fwi2d/local/bin*. It will be more convenient to add this path to the environment variable `PATH`.

If the host machine has **_CUDA_** enabled, add the flag `-DGPU=1`. In this case, the wave propagation and inversion executables will expect an available GPU device. It is recommended to install the **_CUDA_** enabled code in a separate location (e.g. *fwi2d/local_gpu*) so that the CPU-only code can still be used. 

## Data format

By default, the main executables read and write data in *SEPlib* format. Alternatively, regular binary format is also accepted provided that a description file is built (see [Examples](http://cees-gitlab.stanford.edu/nmbader/fwi2d)). A **_C++_** executable is provided to convert between these two formats. Moreover, a python script is provided to convert to/from *SEPlib* from/to **_numpy_**.
