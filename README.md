# About:
## Program Summary:
Test new algorithms for shortening data for the telemetry system using a file to communicate between a transmitter and receiver program.

A MATLAB program generates data, which can be one or three-channel, smooth or noisy, and then plots the data in a graph and exports it into a CSV file.

Then the Transmitter C++ program takes the CSV data, processes/compresses(*) it, and writes it via protobuf into a binary file, representing the data sent through radio.

A Receiver C++ program then takes the binary file data, decodes the compression, and exports that to another CSV file.

Finally, the MATLAB program can plot the final CSV data, seeing how it holds up when compared to the original CSV data.

## Processing and Compression Modes: (*)

### Default 

Data that is collected is sent through 

### Offset 

We record the offset, and send (data – offset) through 

- Useful for when data hovers around a point 

### Derivative 

We track the beginning value, and send the difference between the current and last data 

- Useful for when the value itself is large, but collecting the derivative is small 

### Derivative Offset 

Offset but for derivative 

# Set Up

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