# Set Up
Test new algorithms for shortening data for the telemetry system using a file to communicate between a transmitter and receiver program

https://www.msys2.org/
latest version msys2-x86_64-20230526
C:\msys64\ucrt64\bin\g++

- choco
- make
- protoc

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
```
bazel build Transmitter --verbose_failures
```

## Clean Build
```
bazel clean
```

# Run

```
.\bazel-bin\Transmitter.exe
```