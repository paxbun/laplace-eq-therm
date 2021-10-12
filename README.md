# Laplace Equation Solver

![Example](./Example.png)

Retrieves temperature information from Arduino and computes the (approximate) solution of the Laplace equation problem.

## Prerequisites

- [Rust](https://www.rust-lang.org/tools/install) Nightly 1.57.0 or higher
- [CMake](https://cmake.org/download/) 3.13.0 or higher
- Windows:
  - Latest version of [MSVC](https://visualstudio.microsoft.com) (Building using MinGW or Clang on Windows is not tested)
  - Latest version of [Clang](https://releases.llvm.org/download.html) (Required by [`rust-bindgen`](https://github.com/rust-lang/rust-bindgen), which uses `libclang`)
- Linux:
  - Latest version of GCC (Building using Clang on Linux is not tested)
  - Latest version of `libclang`
- Mac (Building on Mac is not tested)

## How to build

- Install the build dependencies (Windows)

- Install the build dependencies (Linux)

Older versions of GCC can't compile some valid C++17 code. Using GCC 9 or higher is recommended for building this project. On Ubuntu 18.04, you can install GCC 9 using the command below:

```bash
sudo add-apt-repository ppa:ubuntu-toolchain-r/test
sudo apt update
sudo apt install gcc-9 g++-9
```

Before configuring the project, set `CC` and `CXX` to GCC 9.

```bash
export CC=gcc-9
export CXX=g++-9
```

Likewise, older versions of `libclang` can't parse some of the files in this project. Please install `libclang-8` or higher if `rust-bindgen` generates any error.

```bash
sudo apt-get install libclang-8-dev
export BINDGEN_EXTRA_CLANG_ARGS=-std=c++17
```

- Clone this repository

```
git clone https://github.com/paxbun/laplace-eq-therm
```

- Build the entire project using `cargo`

```
cargo build --workspace --release
```

- Move to the directory where the compiled binaries are saved

```
cd target/release
```

## How to run the server

Launch the server by typing the following command:

```
./laplace-eq-therm-server <width of the matrix> <height of the matrix>
```

The server program is using [Rocket](https://github.com/SergioBenitez/Rocket). All configuration parameters supported by Rocket can be set to other values by the users. For detail, please refer [the documentation](https://rocket.rs/v0.4/guide/configuration/). For example, you can change the listening IP address in PowerShell as follows:

```powershell
${env:ROCKET_ADDRESS} = "0.0.0.0";
./laplace-eq-therm-server 3 5
```

or in bash:

```bash
export ROCKET_ADDRESS=0.0.0.0
laplace-eq-therm-server 6 10
```

When the server is launched successfully, you will see a message like:

```
Rocket has launched from http://0.0.0.0:8000
```

Open a web browser and type the URL into the address bar. You must see several tables with colored cells.

## How to set temperature at a point through the web browser

Each point of the input matrix (represented as a cell in the input table) can be in one of three states:

- `Boundary`: This point constitutes the boundary condition.
- `GroundTruth`: This point does not participate in the computation of the solution of the equation. This point is to be compoared with the simulated values.
- `OutOfRange`: This point does not participate in the computation, and its value is not considered valid.

On the website, cells in each state are displayed differently as follows:

- `Boudnary`: Bold, 20px
- `GroundTruth`: Normal, 16px
- `OutOfRange`: Transparent

To toggle between the states, just click the cell you want to change. You will be prompted for the new temperature value of that cell if needed.

## How to connect Arduino to the server

This repo contains another executable, `laplace-eq-therm-client`. This program reads temperature information from your Arduino board and sends it to the server. To see all available Arduino boards connected to your PC, just type as follows:

```
./laplace-eq-therm-client
```

You will see a message as the following:

```
Usage: laplace-eq-therm-client <X> <Y> <Type> <Arduino Serial port> <Server URL>

<Type> is one of the following:
* B: Boundary point
* I: Internal point (Ground truth)

1 Arduino device found
* COM3
```

As you can see, there is a list of name of serial ports which are bound to Arduino boards in the last section. Choose one of them, then compile and upload [`Program.ino`](./Program.ino) to that board. After uplaoding, disconnect the board from Arduino's default serial monitor. Launch the client program following the usage instruction, e.g.:

```
laplace-eq-therm-client 3 10 B COM3 http://localhost:8080
```

Then you will be able to see that the temperature information sent by the client is reflected to the simulation result.

## How to add custom algorithm implementers

The main algorithm implementations reside in the C++ part, not the Rust part. Go to [./server/core/Space.hh](`Space.hh`) and see the definition of `Space`. `Space` is a class which represents an algorithm implementer. Refer to [./server/core/MockSpace.cc](`MockSpace`) to find out what each virtual function must do.

After implementing your own `Space` class, go to [./server/core/Server.cc](`Server.cc`) and find the function `leth_create`. The definition of `leth_create` is as shown below:

```cpp
ServerHandle leth_create(uint16_t width, uint16_t height) noexcept
try
{
    return Server::Make<MockSpace>(width, height);
}
catch (...)
{
    return nullptr;
}
```

To add your implementations, just pass your classes to `Server::Make`. Suppose your `Space` classes are `FooSpace`, `BarSpace`, and `BazSpace`. Add your classes as follows:

```cpp
ServerHandle leth_create(uint16_t width, uint16_t height) noexcept
try
{
    return Server::Make<MockSpace, FooSpace, BarSpace, BazSpace>(width, height);
}
catch (...)
{
    return nullptr;
}
```

The server will automatically run your implementations and emit the results to the clients. Note that `Server::Make` can take any number of classes as parameters.
