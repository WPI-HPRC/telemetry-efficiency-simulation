// C++ Program to Read a file line by line using getline
#include <fstream>
#include <iostream>
#include <cstring>
#include <string>
#include <sstream>
#include <vector>
#include <stdio.h>
#include <algorithm>
// #include "Transmitter.h"
#include "common_variables.h"
#include "packet.pb.h"
#include <cstdio>
#include <cstdlib>

using namespace std;

// Storing our different dataset states
int current_mode[num_vars] = {DEFAULT, DEFAULT, DEFAULT};

// Data memory
int last_dataset[num_vars]; // Set to all zeroes later on
int current_dataset[num_vars];
float offset = 0;

// FOR OFFSET MODE:
const int num_iterations = 5; // Number of iterations to include in setting offset value
int mean_mem[num_iterations + 2][num_vars] = {}; // Our memory data for past iterations, mean, and current offset for all data channels
int meanSumIndex = num_iterations; // Index of our mean sum
int curr_OffsetIndex = num_iterations + 1; // Index of our current offset value
int threshold = 3; // if mean is 3 less/more than the current offset, we change current offset to mean
int iter_counter = 0; // Counts which index of the array to mutate/draw from
int additionalInfo[num_vars*2]; // Appends the new offset value to the end of the CSV line
int additionalInfoUseIndex = 0; // The index of which to append the new offset value to

// Initial variable setup if necessary
void setup();
// Removes file if the file already exists
int removeFile(char* file_name);
// Creates the binary file to store data in
int createFile(char* file_name, ofstream* file);
// Takes the Generated_Data.csv file and parses the data to the protobuf file
int parse_CSV(string file_name);
// PARSE_CSV DEPENDENCIES:
// Reads existing binary file data into current dataSet
int readAllData(telemetry::FullData* data);
// Appends new packet to existing binary file data
int appendNewData(telemetry::FullData* data);
// Append packet to end of data
int append_data(telemetry::Packet* packet, telemetry::Packet desiredPacket);

void setup() {
    // Fill last_dataset as all zeros
    //    This way, the first variable for all offset will be the first value
    for (int i = 0; i < num_vars; i++) {
        last_dataset[i] = 0;
    }
}

int main() {
    setup();
    // Remove existing binary data
    removeFile("Transmitted_Data.bin");

    // Create binary file
    ofstream transmitFile;
    createFile("Transmitted_Data.bin", &transmitFile);

    // Get CSV data into binary file
    parse_CSV("Generated_Data.csv");

    // Close the file to free up resources.
    cout << "Transmission File created successfully." << endl;
    transmitFile.close();

    return 0;
}

int removeFile(char* file_name) {
    ifstream existingfile(file_name);

    if (existingfile.good()) {
        remove(file_name);
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
    cout << "Creating Transmission File..." << endl;
    
    return 0;
}

int parse_CSV(string file_name) {
    ifstream file(file_name);
    string line;

    ofstream outfile("ProcessedData.csv");

    int lineNum = 1;

    // FOR EACH LINE:
    while (getline(file, line)) {
        istringstream dataString(line);
        vector<string> fields;

        telemetry::FullData dataSet;
        telemetry::Packet currentPacket;

        // Read the existing binary file data
        readAllData(&dataSet);

        int counter = 0;

        // FOR EACH DATA:
        while(dataString.good()) {
            string rawData;
            getline(dataString, rawData, ',');
            fields.push_back(rawData);
            // Convert string data to int shifted a few bits for data compression
            int data = stof(rawData) * COMPRESSION_FACTOR;

            // Add data to current dataset
            current_dataset[counter] = data;

            // Actual data to send
            int payload;

            // Decide which payload to send
            switch (global_current_mode) {
                case 1:
                    // Default - send the data as is
                    payload = data;
                    break;
                case 2:
                    // Derivative - send the slope of currentData - lastData
                    payload = data - last_dataset[counter];
                    break;
                case 3:
                    // Offset - send the data - offsetValue
                    // Data collection
                    int oldVal;
                    oldVal = mean_mem[iter_counter][counter]; // Get old value
                    mean_mem[iter_counter][counter] = data; // Replace with new value
                    // Mean Mutation
                    int mean;
                    mean_mem[meanSumIndex][counter] = mean_mem[meanSumIndex][counter] - oldVal + data; // Update our mean sum value
                    mean = mean_mem[meanSumIndex][counter] / num_iterations; // Get our mean value

                    if (counter == 2) {
                        cout << lineNum << ": Data: " << data << " New mean calculated.";
                    }

                    // Determine whether to change our offset value or not  
                    if (abs(mean - mean_mem[curr_OffsetIndex][counter]) > threshold) {
                        mean_mem[curr_OffsetIndex][counter] = mean; // Change our offset value
                        // Put the changed offset value into the CSV
                        additionalInfo[additionalInfoUseIndex] = counter; // Which column has changed offset
                        additionalInfo[additionalInfoUseIndex + 1] = mean_mem[curr_OffsetIndex][counter]; // Changed offset value
                        additionalInfoUseIndex = additionalInfoUseIndex + 2; // Update our additional info use index

                        if (counter == 2) {
                            cout << "Offset value changed.";
                        }
                    }

                    if (counter == 2) {
                        cout << " Offset Value: " << mean_mem[curr_OffsetIndex][counter]<< endl;
                    }

                    // Subtract our current value by the offset
                    payload = data - mean_mem[curr_OffsetIndex][counter];

                    // Increment our iteration counter
                    iter_counter = (iter_counter + 1) % num_iterations;
                    break;
                case 4:
                    // Derivative Offset - Explanation TBD
                    // Increment our iteration counter
                    iter_counter = (iter_counter + 1) % num_iterations;
                    break;
            }

            // Record the data to the CSV
            outfile << payload;
            if (counter != num_vars-1) {
                outfile << ",";
            } else {
                outfile << endl;
            }

            // Append data to the protobuf
            currentPacket.add_dataset(payload);

            // FOR OFFSET MODES:
            if (global_current_mode == OFFSET) {
                 // If this is the last dataset AND we have updated offset info:
                if ((counter == (num_vars - 1)) && (additionalInfoUseIndex != 0)) {
                    // Add our offset info to the protobuf packet
                    for (int i = 0; i < additionalInfoUseIndex; i++) {
                        currentPacket.add_dataset(additionalInfo[i]);
                    }
                }
            }

            // Current dataset is now last dataset
            memcpy(last_dataset, current_dataset, sizeof(current_dataset));
            // Increment counter
            counter++;
        }

        // Add new data to the binary file
        dataSet.add_packet()->CopyFrom(currentPacket);

        // Write the existing + new data to the binary file
        appendNewData(&dataSet);

        // Terminal Progress Bar:
        // For each 10% increase
        // if ((lineNum % num_packets/10) == 0) {
        //     int percentage = lineNum * 100 / num_packets;
        //     cout << "Transmission File" << percentage << "% Completed." << endl;
        // }

        // Increment our line number
        lineNum++;
    }

    outfile.close();
    file.close();
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

int appendNewData(telemetry::FullData* data) {
    // Write the existing + new data to the binary file
    fstream output("Transmitted_Data.bin", ios::out | ios::trunc | ios::binary);
    if (!(*data).SerializeToOstream(&output)) {
        cerr << "Failed to write address book." << endl;
        return -1;
    }
}

