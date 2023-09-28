
#include <iostream>
#include <vector>
#include <stdio.h>

#include <cereal/archives/binary.hpp>
#include "snappy-internal.h"
#include "snappy-sinksource.h"
#include "snappy.h"

struct MyStruct {
  float vertex[6];

  template <class Archive>
  void serialize(Archive& archive) {
    archive(vertex);
  }
};

int main()
{
  MyStruct dataToSerialize = 
  { 
    1.1f, 1.2f, 1.3f, 2.1f, 2.2f, 2.3f
  };

  // Serialize to a binary buffer
  std::vector<char> binaryData;
  {
    std::ostringstream oss;
    cereal::BinaryOutputArchive archive(oss);
    archive(dataToSerialize);

    std::string serializedData = oss.str();
    binaryData.assign(serializedData.begin(), serializedData.end());
  }

  // Compress
  char* p_vertices_compressed = 
    new char[snappy::MaxCompressedLength(binaryData.size())];

  size_t p_vertices_compressed_length;

  // auto compression_start = std::chrono::high_resolution_clock::now();
  snappy::RawCompress(binaryData.data(), binaryData.size(), 
    p_vertices_compressed, &p_vertices_compressed_length);
  // Send p_vertices_compressed to client though asio as usual.

  // At Client end;
  char p_vertices[binaryData.size()];

  bool raw_uncompress = 
    snappy::RawUncompress(p_vertices_compressed, 
      p_vertices_compressed_length,
      p_vertices);


  MyStruct receivedStruct;
  {
    // std::istringstream iss(std::string(binaryData.begin(), binaryData.begin() + binaryData.size()));
    std::istringstream iss(p_vertices);
    cereal::BinaryInputArchive archive(iss);
    archive(receivedStruct);
  }

  // Use the deserialized data
  printf("receivedStruct(%f, %f, %f, %f, %f, %f)\n", 
    receivedStruct.vertex[0], receivedStruct.vertex[1], receivedStruct.vertex[2], 
    receivedStruct.vertex[3], receivedStruct.vertex[4], receivedStruct.vertex[5] 
  );

  return 0;
}