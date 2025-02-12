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
    for (int j = 0; j < num_vars; j++) {
      // Declare our data holder
      int payload;

      // // Guard for if data is nan
      // if (isnan()) {
      //   // Record no data for the end file
      //   outfile << "NaN";
      //   if (j != num_vars-1) {
      //     outfile << ",";
      //   } else {
      //     outfile << endl;
      //   }
      //   // Keep moving; don't execute later code
      //   continue;
      // }

      // Retrieve raw data from the protobuf channels
      switch (j) {
        case 0:
            payload = packet.data1();
            break;
        case 1:
            payload = packet.data2();
            break;
        case 2:
            payload = packet.data3();
            break;
        case 3:
            payload = packet.data4();
            break;
        case 4:
            payload = packet.data5();
            break;
        case 5:
            payload = packet.data6();
            break;
      }
      // const int payload = packet.dataset(j);
      // Get the float-translated data
      float data = float(payload)/COMPRESSION_FACTOR;
      // Our actual final data
      float endValue;

      // Decide which payload to send
      switch (global_current_mode) {
          case 1:
              // Default - send the data as is
              endValue = data;
              break;
          case 2:
              // Derivative - send the slope of currentData - lastData
              endValue = last_dataset[j] + data;
              // cout << "Last Dataset: " << last_dataset[j] << ", Data: " << data << ", Payload: " << payload << endl;
              break;
          case 3:
              // Offset - send the data - offsetValue
              break;
          case 4:
              // Derivative Offset - Explanation TBD
              break;
      }

      // Set current data
      current_dataset[j] = endValue;

      outfile << endValue;
      cout << endValue;

      if (j != num_vars-1) {
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