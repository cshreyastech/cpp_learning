
#include <iostream>
#include <vector>
#include <stdio.h>
#include <fstream>
#include <memory>

#include <cereal/archives/binary.hpp>
#include "snappy-internal.h"
#include "snappy-sinksource.h"
#include "snappy.h"

#define N_POINTS 100000
#define VERTICES_LENGTH 100000 * 6

struct MyStruct {
  float vertex[VERTICES_LENGTH];

  template <class Archive>
  void serialize(Archive& archive) {
    archive(vertex);
  }
};

int main()
{
  MyStruct dataToSerialize;

  const std::string cloud_file_path = 
    "/home/shreyas/Downloads/cloud_data/induvidual_rows/depth_data_100K1-200K.txt";

  std::ifstream file_handler(cloud_file_path);
  std::string each_value_str;
  int n_values_read_from_file  = 0;
  
  while(file_handler >> each_value_str)
  {
  	std::string each_value_clean_str = 
  		each_value_str.substr(0, each_value_str.find("f", 0));

  	float value_float = std::stof(each_value_clean_str);
    // dataToSerialize.vertex[n_values_read_from_file] = value_float;
  	n_values_read_from_file++;
  }
  assert(N_POINTS == (n_values_read_from_file)/6);

  // Serialize to a binary buffer

  std::ostringstream oss;
  cereal::BinaryOutputArchive archive(oss);
  archive(dataToSerialize);

  std::string serializedData = oss.str();

  std::string compressed_data;
  snappy::Compress(serializedData.c_str(), serializedData.size(), &compressed_data);

  // At Client end;
  std::string decompressed_data;
  if (snappy::Uncompress(compressed_data.c_str(), compressed_data.size(), &decompressed_data)) {
    // Decompression succeeded
    std::cout << "Decompression succedded" << std::endl;
  } else {
    // Decompression failed
    std::cerr << "Decompression failed." << std::endl;
  }

  MyStruct receivedStruct;
  {
    std::istringstream iss(std::string(decompressed_data.begin(), decompressed_data.begin() + decompressed_data.size()));
    cereal::BinaryInputArchive archive(iss);
    archive(receivedStruct);
  }

  // assert(receivedStruct.vertex[VERTICES_LENGTH - 1] == 0.619608f);
  return 0;
}