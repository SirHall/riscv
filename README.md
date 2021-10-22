# RISC-V
The current name for the project before I come up with something more creative. :)

# What is this?
This is an in-development C++ implementation of a RISC-V CPU, aiming to provide
a blazingly fast simulation of RISC-V.
Currently `RV32I` has been implemented with exception of the `CSR` instructions.

# Why would I use this?
Well currently don't, this library needs a lot more work before it becomes useable,
but there are a few key reasons why this library would be of use in the future.

## Performance
This library has a performance-first design philosophy, even redesigning the
RISC-V memory instruction binary layout to allow many branches to be removed.
In turn this also simplifies the main simulation loop ideally allowing compilers
to more easily form platform-specific optimizations.

It is planned to also eventually allow reading in and transforming of
standard-compliant binaries into a form that is understood by this simulator.

## Built on C++20 modules
This project was my excuse to test out using CMake with C++20 modules,
as other platforms complete their support for C++20 modules
(and MSVC removes the `.ixx` extension requirement) the library will be
refactored to ensure cross-platform compatiblity with
MSVC on Windows, and GCC on Linux primarily.

# Why shouldn't I use this?
Besides the previously mentioned progress issue, this library does not use the
standard instruction binary layout, the instruction binary layout used by this
project can be seen below.

In turn, this can mean that programs that modify themselves during
execution or set the CPU to execute runtime-generated code will not function.

## Built on C++20 modules
This project is built on C++20 modules, which will for the large majority of
cases not be fully supported (as of the writing of this page) on all platforms
using all compilers, on all build systems - currently even CMake's support of
modules is in early development.

It may be better to wait until better support for modules is setup, and also
see if this can be integrated into your own projects.

# Why is the git commit log so short?
This project was actually developed initially as a smaller part of a different
currently private project.
For modularity's sake it has been split off into a separate repository,
which then allows me to develop this as a separate library which can be used
openly.

# Instruction Binary Layout
![Binary Layout](https://ovi.sh/riscv_mem.png)
