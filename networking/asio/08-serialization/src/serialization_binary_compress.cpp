
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
#include <chrono>

class Timer
{
public:
  Timer(std::string caption)
    : caption_{caption}
  {
    m_StartTimePoint = std::chrono::high_resolution_clock::now();
  }

  ~Timer()
  {
    Stop();
  }

  void Stop()
  {
    auto endTimePoint = std::chrono::high_resolution_clock::now();

    auto start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimePoint).time_since_epoch().count();
    auto end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimePoint).time_since_epoch().count();

    auto duration = end - start;
    double ms = duration * 0.001;

    // std::cout << caption_ << ": "<< duration << "us (" << ms << "ms)\n";
    printf("%s: (%f ms)\n", caption_.c_str(), ms);
  }
private:
  std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTimePoint;
  std::string caption_{""};
};

#define N_POINTS 100000

static uint32_t s_AllocCount = 0;
void* operator new(size_t size)
{
  s_AllocCount++;
  // std::cout << "Allocating " << size << " bytes\n";
  return malloc(size);
}

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
  MyStruct dataToSerialize;

  const char* cloud_file_path = 
    "/home/shreyas/Downloads/cloud_data/induvidual_rows/depth_data_1-100K.txt";

  std::ifstream file_handler(cloud_file_path);
  std::string each_value_str;

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
  std::ostringstream oss;
  {
    Timer timer("cereal::BinaryOutputArchive");
    cereal::BinaryOutputArchive archive(oss);
    archive(dataToSerialize);
  }
  

  std::string serializedData = oss.str();

  std::string compressed_data;
  {
    Timer timer("snappy::Compress");
    snappy::Compress(serializedData.c_str(), serializedData.size(), &compressed_data);
  }

  // At Client end;
  std::string decompressed_data;
  {
    Timer timer("snappy::Uncompress");
    if (snappy::Uncompress(compressed_data.c_str(), compressed_data.size(), &decompressed_data)) {
      // Decompression succeeded
      std::cout << "Decompression succedded" << std::endl;
    } else {
      // Decompression failed
      std::cerr << "Decompression failed." << std::endl;
    }
  }

  MyStruct receivedStruct;
  {
    Timer timer("cereal::BinaryInputArchive");
    std::istringstream iss(std::string(decompressed_data.begin(), decompressed_data.begin() + decompressed_data.size()));
    cereal::BinaryInputArchive archive(iss);
    archive(receivedStruct);
  }

  assert((receivedStruct.vertex[N_POINTS - 1].Color.v0) == 0.160784f);
  std::cout << " allocations: " << s_AllocCount << std::endl;
  return 0;
}