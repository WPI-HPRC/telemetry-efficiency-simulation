// C++ Program to Read a file line by line using getline
#include <fstream>
#include <iostream>
#include <cstring>
#include <string>
#include <sstream>
#include <vector>
#include <stdio.h>
#include "packet.pb.h"


int main() {
    // Create an input file stream object named 'file' and
    // open the file "GFG.txt".
    std::ifstream file("Generated_Data.csv");


    // String to store each line of the file.
    std::string line;


    if (file.is_open()) {
        // Keep track of our data index
        int counter = 1;


        // For each line in the CSV file
        while (getline(file, line)) {
            // Print index
            // std::cout << counter << ':' << std::endl;
           
            // // CREATE OUR VALUE HOLDER
            // // Declare our char* for the string
            // // Find length of the line we need
            // int len = line.length();
            // char parsableLine[len+1];
            // // Convert string to char*
            // strcpy(parsableLine, line.c_str());

            // // CREATE OUR PROTOBUF
            // telemetry::Packet currentPacket;
            
            // // COLLECT OUR DATA
            // // Returns first token
            // char *token = strtok(parsableLine, ",");
            // currentPacket.add_dataset(token);
       
            // // Returns next tokens
            // while (token != NULL)
            // {
            //     // std::cout << token << std::endl;
            //     currentPacket.add_dataset(token);
            //     token = strtok(NULL, ",");
            // }

            // // Print what we have in the protobuf
            // std::cout << currentPacket << std::endl;

            // counter++;
        }


        // Close the file stream once all lines have been
        // read.
        file.close();
    }
    else {
        // Print an error message to the standard error
        // stream if the file cannot be opened.
        std::cerr << "Unable to open file!" << std::endl;
    }


    return 0;
}
