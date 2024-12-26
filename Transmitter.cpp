// C++ Program to Read a file line by line using getline
#include <fstream>
#include <iostream>
#include <cstring>
#include <string>
#include <sstream>
#include <vector>
#include <stdio.h>


int main() {
    // Create an input file stream object named 'file' and
    // open the file "GFG.txt".
    std::ifstream file("Generated_Data.csv");


    // String to store each line of the file.
    std::string line;


    if (file.is_open()) {
        // Keep track of our data index
        int counter = 1;


        // Read each line from the file and store it in the
        // 'line' variable.
        while (getline(file, line)) {
            std::cout << counter << ':' << std::endl;
           
            // Declare our char* for the string
            // Find length of the line we need
            int len = line.length();
            char parsableLine[len+1];
            // Convert string to char*
            strcpy(parsableLine, line.c_str());
            // Returns first token
            char *token = strtok(parsableLine, ",");
       
            // Keep printing tokens while one of the
            // delimiters present in str[].
            while (token != NULL)
            {
                std::cout << token << std::endl;
                token = strtok(NULL, ",");
            }


            counter++;
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
