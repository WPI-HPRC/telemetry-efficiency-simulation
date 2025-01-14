// C++ Program to generate random binary data for experimentation
#include <fstream> 
#include <iostream>
#include <cstring>
#include <string>
#include <sstream>
#include <vector>
#include <stdio.h>
#include <algorithm>
#include "common_variables.h"
#include "packet.pb.h"
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>
#include <cstdio>
#include <cstdlib>
#include <tuple>

using namespace std;

// Function for getting data without needing a wrapper(to save bytes)
int getDataWithoutWrapper(char* fileName);

int main() {
    // Our Variable Options:
    tuple<char*, int, char*> Digit0 = {"RandGen/TestSetZero.bin", 0, "RandGen/TestSetZero.csv"};
    tuple<char*, int, char*> Digit1 = {"RandGen/TestSetTen.bin", 10, "RandGen/TestSetTen.csv"};
    tuple<char*, int, char*> Digit2 = {"RandGen/TestSetHundred.bin", 100, "RandGen/TestSetHundred.csv"};
    tuple<char*, int, char*> Digit3 = {"RandGen/TestSetThousand.bin", 1000, "RandGen/TestSetThousand.csv"};

    // Our Variables We Use:
    tuple<char*, int, char*> ourControls = Digit3;
    char* fileName = get<0>(ourControls);
    int capNum = get<1>(ourControls);
    char* readableFileName = get<2>(ourControls);

    // Random Number Generator: 
    srand(time(NULL));

    // Declare our input CSV
    ofstream infile(readableFileName);

    // Open desired file to clear it
    ofstream ofs;
    ofs.open(fileName, std::ofstream::out | std::ofstream::trunc);

    // For 500 iterations:
    for (int i = 0; i < 500; i++) {
        // Create a packet within the data set
        telemetry::Packet currentPacket;

        // For 3 iterations:
        for (int j = 0; j < 3; j++) {
            // Create each data in the packet
            int ourNum;

            // If the divisor is not 0
            if (capNum != 0) {
                // We take the remainder
                ourNum = rand() % capNum;
            } else {
                // We just make our output 0
                ourNum = 0;
            }
            currentPacket.add_dataset(ourNum);
            // cout << ourNum << " ";
            infile << ourNum;

            if (j != 2) {
                infile << ",";
            } else {
                infile << endl;
            }
        }

        // Add packet data to file
        fstream output(fileName, ios::out | ios::app | ios::binary);
        if (!currentPacket.SerializeToOstream(&output)) {
            cerr << "Failed to write address book." << endl;
            return -1;
        }
    }

    getDataWithoutWrapper(fileName);

    return 0;
}

int getDataWithoutWrapper(char* fileName) {
    // Declare our outfile path
    ofstream outfile("RandGen/NoWrapperData.csv");

    std::ifstream inputFile(fileName, std::ios::binary);

    // Holder for the current packet we are looking for
    telemetry::Packet currentPacket;

    currentPacket.ParseFromIstream(&inputFile);

    std::string textFormat;
    google::protobuf::TextFormat::PrintToString(currentPacket, &textFormat);

    std::istringstream textStream(textFormat);
    std::string line;

    while (std::getline(textStream, line)) {
        // Process each line
        std::cout << line << std::endl; 
    }

    return 0;
};