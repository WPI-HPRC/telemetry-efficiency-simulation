# Set Up
Test new algorithms for shortening data for the telemetry system using a file to communicate between a transmitter and receiver program

https://www.msys2.org/
latest version msys2-x86_64-20230526
C:\msys64\ucrt64\bin\g++

- choco
- make
- protoc

It also may be a good idea to go into environment variables to link necessary paths.

For Bazel C++ compiler: https://stackoverflow.com/a/69962808 solution

### Visual Studio
installed visual studio community edition
use basel to build/run

### cmake
cmake
https://cmake.org/download/

### Protobuf
git clone https://github.com/protocolbuffers/protobuf
latest version v29.2

### Abseil
git clone https://github.com/abseil/abseil-cpp
latest version 20240722.0

# Build
## Compile and Build
#### Protobuf Files
```
bazel build packet_proto packet_cc_proto --verbose_failures
```
#### Transmitter
```
bazel build Transmitter --verbose_failures
```
#### Receiver
```
bazel build Receiver --verbose_failures
```

## Clean Build
```
bazel clean
```

# Run
#### Transmitter
```
.\bazel-bin\Transmitter.exe
```
#### Receiver
```
.\bazel-bin\Receiver.exe
```