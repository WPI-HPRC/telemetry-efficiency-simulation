// C++ Program to Read a file line by line using getline
#include <fstream>
#include <iostream>
#include <cstring>
#include <string>
#include <sstream>
#include <vector>
#include <stdio.h>
#include "packet.pb.h"
#include <cstdio>
#include <cstdlib>

using namespace std;

// Removes file if the file already exists
int removeFile(char* file_name);
// Creates the binary file to store data in
int createFile(char* file_name, ofstream* file);
// Takes the Generated_Data.csv file and parses the data to the protobuf file
int parse_CSV(string file_name);
// Append packet to end of data
int append_data(telemetry::Packet* packet, telemetry::Packet desiredPacket);

int main() {
    // Remove existing binary data
    removeFile("Transmitted_Data.bin");

    // Create binary file
    ofstream transmitFile;
    createFile("Transmitted_Data.bin", &transmitFile);

    // Get CSV data into binary file
    parse_CSV("Generated_Data.csv");

    // Close the file to free up resources.
    transmitFile.close();

    return 0;
}

int removeFile(char* file_name) {
    std::ifstream existingfile(file_name);

    if (existingfile.good()) {
        std::remove(file_name);
    }

    return 0;
}

int createFile(char* file_name, ofstream* file) {
    //using ofstream for output file operations.
    // ofstream transmitfile;

    // Opening file "Gfg.txt" in write mode.
    (*file).open("Transmitted_Data.bin");

    // Check if the file was successfully created.
    if (!(*file).is_open())
    {
        cout << "Error in creating file!" << endl;
       // Return a non-zero value to indicate an error.
        return 1;
    }
    cout << "File created successfully." << endl;
    
    return 0;
}

int parse_CSV(string file_name) {
    ifstream file(file_name);
    string line;

    while (getline(file, line)) {
        istringstream iss(line);
        vector<string> fields;

        telemetry::FullData dataSet;
        telemetry::Packet currentPacket;

        // Read the existing binary file data
        fstream input("Transmitted_Data.bin", ios::in | ios::binary);
        if (!input) {
        cout << "Transmitted_Data.bin" << ": File not found.  Creating a new file." << endl;
        } else if (!dataSet.ParseFromIstream(&input)) {
        cerr << "Failed to parse address book." << endl;
        return -1;
        }

        // Parse each line field by field
        while(iss.good()) {
            string field;
            getline(iss, field, ',');
            fields.push_back(field);

            currentPacket.add_dataset(stof(field));
        }

        // Display protobuf packet
        cout << currentPacket.DebugString() << endl;

        // Add new data to the binary file
        // append_data(dataSet.add_packet(), currentPacket);
        dataSet.add_packet()->CopyFrom(currentPacket);

        // Write the existing + new data to the binary file
        fstream output("Transmitted_Data.bin", ios::out | ios::trunc | ios::binary);
        if (!dataSet.SerializeToOstream(&output)) {
            cerr << "Failed to write address book." << endl;
            return -1;
        }
    }

    file.close();
    return 0;
}

int append_data(telemetry::Packet packet, telemetry::Packet desiredPacket) {
    packet.CopyFrom(desiredPacket);

    return 0;
}

