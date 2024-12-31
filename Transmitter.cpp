// C++ Program to Read a file line by line using getline
#include <fstream>
#include <iostream>
#include <cstring>
#include <string>
#include <sstream>
#include <vector>
#include <stdio.h>
#include "packet.pb.h"

using namespace std;


int read_line_by_line(string file_name) {
    ifstream file(file_name);
    string line;

    while (getline(file, line)) {
        istringstream iss(line);
        vector<string> fields;

        telemetry::Packet currentPacket;

        // Parse each line field by field
        while(iss.good()) {
            string field;
            getline(iss, field, ',');
            fields.push_back(field);

            currentPacket.add_dataset(stof(field));
        }

        cout << currentPacket.DebugString() << endl;
    }

    file.close();
    return 0;
}

int main() {
    read_line_by_line("Generated_Data.csv");
}

// int main() {
//     // Create an input file stream object named 'file' and
//     // open the file "GFG.txt".
//     ifstream file("Generated_Data.csv");


//     // String to store each line of the file.
//     string line;


//     if (file.is_open()) {
//         // Keep track of our data index
//         int counter = 1;


//         // For each line in the CSV file
//         while (getline(file, line)) {
//             // Print index
//             cout << counter << ':' << endl;
           
//             // CREATE OUR VALUE HOLDER
//             // Declare our char* for the string
//             // Find length of the line we need
//             int len = line.length();
//             char parsableLine[len+1];
//             // Convert string to char*
//             strcpy(parsableLine, line.c_str());

//             // CREATE OUR PROTOBUF
//             // telemetry::Packet currentPacket;
            
//             // COLLECT OUR DATA
//             // Returns first token
//             char *token = strtok(parsableLine, ",");
//             // currentPacket.add_dataset(atoi(token));
       
//             // Returns next tokens
//             while (token != NULL)
//             {
//                 // cout << token << endl;
//                 // currentPacket.add_dataset(atoi(token));
//                 token = strtok(NULL, ",");
//             }

//             // Print what we have in the protobuf
//             // cout << currentPacket << endl;

//             counter++;
//         }


//         // Close the file stream once all lines have been
//         // read.
//         file.close();
//     }
//     else {
//         // Print an error message to the standard error
//         // stream if the file cannot be opened.
//         cerr << "Unable to open file!" << endl;
//     }


//     return 0;
// }

