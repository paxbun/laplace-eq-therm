# Laplace Equation Solver

![Example](./example.png)

Retrieves temperature information from Arduino and computes (approximate) solution of the Laplace equation problem.

## Prerequisites

- [Rust](https://www.rust-lang.org/tools/install) Nightly 1.57.0 or higher
- [CMake](https://cmake.org/download/) 3.13.0 or higher
- Proper C++ compilers such as GCC, [Clang](https://releases.llvm.org/download.html), or [MSVC](https://visualstudio.microsoft.com/)

## How to build

- Clone this repository

```
git clone https://github.com
```

- Build the entire project using `cargo`

```
cargo build --workspace --release
```

- Move to the directory where the compile results are saved

```
cd target/release
```

## How to run the server

The server program is using [Rocket](https://github.com/SergioBenitez/Rocket).

```
Usage: ./laplace-eq-therm-server <width of the matrix> <height of the matrix>
```
