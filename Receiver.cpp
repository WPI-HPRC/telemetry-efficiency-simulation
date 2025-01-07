// C++ Program to Read a binary file and print its contents to a CSV file
#include <fstream>
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

// const int num_vars = 3;

int current_mode[3] = {DEFAULT, DEFAULT, DEFAULT};
int global_current_mode = DERIVATIVE;

float last_dataset[num_vars];
float current_dataset[num_vars];

void setup() {
    // Fill last_dataset as all zeros
    //    This way, the first variable for all offset will be the first value
    for (int i = 0; i < num_vars; i++) {
        last_dataset[i] = 0;
    }
}

int main() {
  ofstream outfile("ReceivedData.csv");

  // Check if the file is open
  if (!outfile.is_open()) {
      cerr << "Error opening file!" << endl;
      return 1;
  }

  telemetry::FullData receivedData;
  
  // Read in the transmission file
  fstream input("Transmitted_Data.bin", ios::in | ios::binary);
  if (!receivedData.ParseFromIstream(&input)) {
    cerr << "Failed to parse address book." << endl;
    return -1;
  }

  // Display the received data
  for (int i = 0; i < receivedData.packet_size(); i++) {
    // For the first iteration:
    // Setup our last variables
    const telemetry::Packet& packet = receivedData.packet(i);

    // Write data to CSV
    for (int j = 0; j < packet.dataset_size(); j++) {
      const float data = packet.dataset(j);
      float payload;

      // Decide which payload to send
      switch (global_current_mode) {
          case 1:
              // Default - send the data as is
              payload = data;
              break;
          case 2:
              // Derivative - send the slope of currentData - lastData
              payload = last_dataset[j] + data;
              cout << "Last Dataset: " << last_dataset[j] << ", Data: " << data << ", Payload: " << payload << endl;
              break;
          case 3:
              // Offset - send the data - offsetValue
              break;
          case 4:
              // Derivative Offset - Explanation TBD
              break;
      }

      // Set current data
      current_dataset[j] = payload;

      outfile << payload;

      if (j != packet.dataset_size()-1) {
        outfile << ",";
      } else {
        outfile << endl;
      }

      // Current dataset is now last dataset
      memcpy(last_dataset, current_dataset, sizeof(current_dataset));
    }
  }

  // Close the file
  outfile.close();

  return 0;
}