# About:
## Program Summary:
Test new algorithms for shortening data for the telemetry system using a file to communicate between a transmitter and receiver program.

A MATLAB program generates data, which can be one or three-channel, smooth or noisy, and then plots the data in a graph and exports it into the Generated_Data.csv file.

Then the Transmitter C++ program takes the data from Generated_Data.csv, processes/compresses(*) it into ProcessedData.csv for reading, and writes it via protobuf into the Transmitted_Data.bin file, representing the data sent through radio.

A Receiver C++ program then takes the binary file data from Transmitted_Data.bin, decodes the compression, and exports that to ReceivedData.csv.

Finally, the MATLAB program can plot the final data from ReceivedData.csv, seeing how it holds up when compared to the original data in Generated_Data.csv.

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
Run this
```
bazel build TX
```
Other options
```
bazel build --copt -g --strip=never TX
bazel build Transmitter --verbose_failures
```
#### Receiver
Run this
```
bazel build TR
```
Other options
```
bazel build --copt -g --strip=never TR
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
.\bazel-bin\TX.exe
```
#### Receiver
```
.\bazel-bin\Receiver.exe
.\bazel-bin\TR.exe
```

# Debug
To debug Bazel C++ build in Windows MSVC, use "cppvsdbg: C++ (Windows)" launch cinfiguration type as shown below:
```
{
  "version": "0.2.0",
  "configurations": [
    {
      "name": "Debug Transmitter",
      "type": "cppvsdbg",
      "request": "launch",
      "program": "${workspaceFolder}/bazel-bin/TX.exe",
      "args": [],
      "stopAtEntry": false,
      "cwd": "${fileDirname}",
      "environment": [],
      "console": "externalTerminal"
    },
    {
      "name": "Debug Receiver",
      "type": "cppvsdbg",
      "request": "launch",
      "program": "${workspaceFolder}/bazel-bin/TR.exe",
      "args": [],
      "stopAtEntry": false,
      "cwd": "${fileDirname}",
      "environment": [],
      "console": "externalTerminal"
    },
  ]
}
```