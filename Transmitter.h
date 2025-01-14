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
#include <cstdio>
#include <cstdlib>

// Storing our different dataset states
int current_mode[num_vars] = {DEFAULT, DEFAULT, DEFAULT};

// Data memory
int last_dataset[num_vars]; // Set to all zeroes later on
int current_dataset[num_vars];
float offset = 0;

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