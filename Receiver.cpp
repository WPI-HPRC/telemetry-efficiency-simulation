// C++ Program to Read a binary file and print its contents to a CSV file
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

int main() {
  std::ofstream outfile("ReceivedData.csv");

  // Check if the file is open
  if (!outfile.is_open()) {
      std::cerr << "Error opening file!" << std::endl;
      return 1;
  }

  // // Write header row
  // outfile << "data1,data2,data3" << std::endl;

  // // Write some data rows
  // outfile << "John,25,New York" << std::endl;
  // outfile << "Mary,30,London" << std::endl;
  // outfile << "Bob,20,Tokyo" << std::endl;

  telemetry::FullData receivedData;
  
  // Read in the transmission file
  fstream input("Transmitted_Data.bin", ios::in | ios::binary);
  if (!receivedData.ParseFromIstream(&input)) {
    cerr << "Failed to parse address book." << endl;
    return -1;
  }

  // Display the received data
  for (int i = 0; i < receivedData.packet_size(); i++) {
    const telemetry::Packet& packet = receivedData.packet(i);

    // Write data to CSV
    for (int j = 0; j < packet.dataset_size(); j++) {
      const float data = packet.dataset(j);
      outfile << data;
      if (j != packet.dataset_size()-1) {
        outfile << ",";
      } else {
        outfile << std::endl;
      }
    }
  }

  // Close the file
  outfile.close();

  return 0;
}