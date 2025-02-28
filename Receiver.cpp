// C++ Program to Read a binary file and print its contents to a CSV file
#include <fstream>
#include <filesystem>
#include <iostream>
#include <cstring>
#include <string>
#include <sstream>
#include <vector>
#include <stdio.h>
#include "common_variables.h"
#include "packet.pb.h"
#include <cstdio>
#include <cstdlib>

using namespace std;

// Check to see if a file is empty
bool isFileEmpty(const std::string& filePath);
// Clear file
int clearFile(char* file_name);
// Create a file with said name
int createFile(char* file_name, ofstream* file);
// Reads existing binary file data into current dataSet
int readAllData(telemetry::FullData* data);
// Truncates full data to existing binary file data
int truncateNewData(telemetry::FullData* data);
// Appends new packet to existing binary file data
int appendNewData(telemetry::Packet* data);

int main() {
    // Create binary files for the data record and for data sizing up
    ofstream transmitFile;
    ofstream sizeCheckFile;
    createFile("Transmitted_Data.bin", &transmitFile);
    createFile("Measurable_Data.bin", &sizeCheckFile);

    // Creates file to have TX/RX communication
    ofstream commandFile;
    createFile("Command_File.txt", &commandFile);

    cout << "Receiver started. Looking for packets..." << endl;

    // Keep waiting for data to come
    while (true) {
        // When a fresh set of data is sent
        // See if anything is sent as a command
        ifstream incomingCommand("Command_File.txt", ios::in | ios::binary);
        string command;
        getline(incomingCommand, command);

        // Clear command file
        clearFile("Command_File.txt");

        if (command == "Started") {
            // Clear all of our data storage files
            clearFile("Wire.bin");
            clearFile("Transmitted_Data.bin");
            clearFile("Measurable_Data.bin");

            // Reset our datanum variable
            dataNum = 1;

            // Log the data clean
            cout << "**************************ALL FILES CLEARED**************************" << endl;

            // transmission start is over
            transmissionStart = 0;
        }

        // Guard for when there is no data in Wire.bin
        if (isFileEmpty("Wire.bin")) {
            continue;
        }

        // If there IS data:
        // Extract the protobuf from Wire.bin
        telemetry::Packet receivedPacket;
  
        // Read in the data
        fstream input("Wire.bin", ios::in | ios::binary);
        if (!receivedPacket.ParseFromIstream(&input)) {
            cerr << "Failed to read data." << endl;
            return -1;
        }
        cout << "DATA " << dataNum << ": Packet acquired." << endl;

        // Delete all data from Wire.bin
        clearFile("Wire.bin");

        // Put usable protobufs into Transmitted_Data.bin
        //      This is so we have the data in an accessible format
        telemetry::FullData accumulatedData;
        // Read previous data in
        readAllData(&accumulatedData);
        // Add our new packet to the packet array
        accumulatedData.add_packet()->CopyFrom(receivedPacket);
        // Record our new packet array
        truncateNewData(&accumulatedData);

        // Put the raw protobufs without wrapper int Measurable_Data.bin
        //      This is so we can find the accumulated number of bytes sent through the wire
        appendNewData(&receivedPacket);

        dataNum++;
    }

    return 0;
}

int clearFile(char* file_name) {
    std::ofstream file(file_name, std::ios::out | std::ios::trunc);

    if (file.is_open()) {
        // File is now empty (or newly created)
        file.close();
        if (file_name == "Wire.bin") {
            std::cout << "DATA " << dataNum << ": Packet cleared." << std::endl;
        }
    } else {
        std::cerr << "Error opening file." << std::endl;
    }

    return 0;
}

bool isFileEmpty(const std::string& filePath) {
    std::ifstream file(filePath);
    return file.peek() == std::ifstream::traits_type::eof();
}

int createFile(char* file_name, ofstream* file) {
    //using ofstream for output file operations.
    // ofstream transmitfile;

    // Opening file "Gfg.txt" in write mode.
    (*file).open(file_name);

    // Check if the file was successfully created.
    if (!(*file).is_open())
    {
        cout << "Error in creating file!" << endl;
       // Return a non-zero value to indicate an error.
        return 1;
    }
    // cout << "Creating Transmission File..." << endl;
    
    return 0;
}

int readAllData(telemetry::FullData* data) {
    // Read the existing binary file data
    fstream input("Transmitted_Data.bin", ios::in | ios::binary);

    if (!input) {
        cout << "Transmitted_Data.bin" << ": File not found.  Creating a new file." << endl;
    } else if (!(*data).ParseFromIstream(&input)) {
        cerr << "Failed to parse address book." << endl;
        return -1;
    }

    return 0;
}


int truncateNewData(telemetry::FullData* data) {
    // Write the existing + new data to the binary file
    // truncate for Transmitted_Data and Wire, append for Measurable_Data
    fstream truncatedOutput("Transmitted_Data.bin", ios::out | ios::trunc | ios::binary);
    
    // newDataAvailable = 1;
    if (!(*data).SerializeToOstream(&truncatedOutput)) {
        cerr << "Failed to write address book." << endl;
        return -1;
    }
}

int appendNewData(telemetry::Packet* data) {
    // Write the existing + new data to the binary file
    // truncate for Transmitted_Data and Wire, append for Measurable_Data
    fstream appendedOutput("Measurable_Data.bin", ios::out | ios::app | ios::binary);
    
    // newDataAvailable = 1;
    if (!(*data).SerializeToOstream(&appendedOutput)) {
        cerr << "Failed to write address book." << endl;
        return -1;
    }
}