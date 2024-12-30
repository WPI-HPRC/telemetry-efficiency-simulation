all: Transmitter.o
	g++ -o a Transmitter.o

Transmitter.o: Transmitter.cpp packet.pb.cc
	g++ -g -c --std=c++23 \
	-I . \
	-I C:\Users\rezha\Downloads\App_Setups+Apps\protobuf-29.2\src \
	-I C:/Users/rezha/Downloads/App_Setups+Apps/abseil-cpp \
	Transmitter.cpp -o Transmitter.o

packet.pb.cc: packet.proto
	protoc --cpp_out=. packet.proto

clean:
	del Transmitter.o a.exe packet.pb.cc packet.pb.h