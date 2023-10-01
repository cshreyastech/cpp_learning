
#include <iostream>
#include <vector>
#include <stdio.h>
#include <fstream>
#include <memory>

#include <cereal/archives/binary.hpp>
#include "snappy-internal.h"
#include "snappy-sinksource.h"
#include "snappy.h"
#include <stdio.h>

#define N_POINTS 100000
#define VERTICES_LENGTH 100000 * 6


struct Vec3
{
  float v0, v1, v2;

  template <class Archive>
  void serialize(Archive& archive) {
    archive(v0, v1, v2);
  }
};

struct Vertex
{
  Vec3 Position;
  Vec3 Color;
  
  template <class Archive>
  void serialize(Archive& archive) {
    archive(Position, Color);
  }
};

struct MyStruct {
  Vertex vertex[N_POINTS];

  template <class Archive>
  void serialize(Archive& archive) {
    archive(vertex);
  }
};

int main()
{ 
  // MyStruct dataToSerialize = 
  // { 
  //   { 1.1, 1.2, 1.3, 2.1, 2.2, 2.3 }
  // };

  // // Serialize to a binary buffer
  // std::vector<char> binaryData;
  // {
  //   std::ostringstream oss;
  //   cereal::BinaryOutputArchive archive(oss);
  //   archive(dataToSerialize);

  //   std::string serializedData = oss.str();
  //   binaryData.assign(serializedData.begin(), serializedData.end());
  // }

  // MyStruct receivedStruct;
  // {
  //   std::istringstream iss(std::string(binaryData.begin(), binaryData.begin() + binaryData.size()));
  //   cereal::BinaryInputArchive archive(iss);
  //   archive(receivedStruct);
  // }

  // // Use the deserialized data
  // printf("receivedStruct - Position(%f, %f, %f), Color(%f, %f, %f)\n", 
  //   receivedStruct.vertex.Position.v0, receivedStruct.vertex.Position.v1, receivedStruct.vertex.Position.v2,
  //   receivedStruct.vertex.Position.v0, receivedStruct.vertex.Position.v1, receivedStruct.vertex.Position.v2
  //   );




  MyStruct dataToSerialize;

  const std::string cloud_file_path = 
    "/home/shreyas/Downloads/cloud_data/induvidual_rows/depth_data_100K1-200K.txt";

  // std::ifstream file_handler(cloud_file_path);
  // std::string each_value_str;
  // int n_values_read_from_file  = 0;
  
  // while(file_handler >> each_value_str)
  // {
  // 	std::string each_value_clean_str = 
  // 		each_value_str.substr(0, each_value_str.find("f", 0));

  // 	// float value_float = std::stof(each_value_clean_str);
  //   dataToSerialize.vertex[n_values_read_from_file] = 
  //   {
  //     std::stof(each_value_clean_str),

  //   }
  //   // value_float;
  // 	n_values_read_from_file++;
  // }

  // int n_values_read_from_file  = 0;

  std::ifstream file_handler(cloud_file_path);
  std::string each_value_str;
  // std::string each_value_clean_str;
  float value_float;

  for(int i = 0; i < N_POINTS; i++)
  {
    Vertex v;

    file_handler >> each_value_str;
    v.Position.v0 = std::stof(each_value_str);

    file_handler >> each_value_str;
    v.Position.v1 = std::stof(each_value_str);

    file_handler >> each_value_str;
    v.Position.v2 = std::stof(each_value_str);


    file_handler >> each_value_str;
    v.Color.v0 = std::stof(each_value_str);

    file_handler >> each_value_str;
    v.Color.v1 = std::stof(each_value_str);

    file_handler >> each_value_str;
    v.Color.v2 = std::stof(each_value_str);

    dataToSerialize.vertex[i] = v;
  }
  // assert(N_POINTS == (n_values_read_from_file)/6);

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

  // Row number 99961 in text file
  assert((receivedStruct.vertex[99960].Color.v0) == 0.729412f);
  return 0;
}