
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
#include <memory>
#include <vector>
#include <future>

#define N_POINTS 100000

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
  std::vector<Vertex> vertex;

  template <class Archive>
  void serialize(Archive& archive) {
    archive(vertex);
  }
};

static int foo(int x, int y) {
  return x + y;
}



// static std::mutex s_VertexMutex;
// static void LoadVertices(std::vector<Vertex>& vertices, std::string each_value_str)
// {
//   // std::ifstream file_handler(filepath.c_str());
//   // std::string each_value_str;

//   // for(int i = 0; i < N_POINTS; i++)
//   // {
//     Vertex v;

//     file_handler >> each_value_str;
//     v.Position.v0 = std::stof(each_value_str);

//     file_handler >> each_value_str;
//     v.Position.v1 = std::stof(each_value_str);

//     file_handler >> each_value_str;
//     v.Position.v2 = std::stof(each_value_str);

//     file_handler >> each_value_str;
//     v.Color.v0 = std::stof(each_value_str);

//     file_handler >> each_value_str;
//     v.Color.v1 = std::stof(each_value_str);

//     file_handler >> each_value_str;
//     v.Color.v2 = std::stof(each_value_str);

//     std::lock_guard<std::mutex> lock(s_VertexMutex);
//     vertices.push_back(v);
//   // }
// }

int main()
{ 
  // MyStruct dataToSerialize;
  std::vector<Vertex> vertices;

  std::string cloud_file_path = 
    "/home/shreyas/Downloads/cloud_data/induvidual_rows/depth_data_1-100K.txt";

  std::ifstream file_handler(cloud_file_path);
  std::string each_value_str;
  

  std::future<int> futureResult = std::async(std::launch::async, foo, 10, 20);
  // int result = futureResult.get();
  // std::cout << "Result: " << result << std::endl;

  // std::vector<std::future<void>> m_Futures;
  
  // {
    // Timer timer("Load file");
    // std::async(std::launch::async, LoadVertices, objectsToSerialize, cloud_file_path);
    // for(int i = 0; i < N_POINTS; i++)
    // {
    //   file_handler >> each_value_str;
    //   m_Futures.push_back(std::async(std::launch::async, LoadVertices, vertices, each_value_str));
    // }
    //   Vertex v;

    //   file_handler >> each_value_str;
    //   v.Position.v0 = std::stof(each_value_str);

    //   file_handler >> each_value_str;
    //   v.Position.v1 = std::stof(each_value_str);

    //   file_handler >> each_value_str;
    //   v.Position.v2 = std::stof(each_value_str);


    //   file_handler >> each_value_str;
    //   v.Color.v0 = std::stof(each_value_str);

    //   file_handler >> each_value_str;
    //   v.Color.v1 = std::stof(each_value_str);

    //   file_handler >> each_value_str;
    //   v.Color.v2 = std::stof(each_value_str);

    //   objectsToSerialize[i] = v;
    // }
  // }







  // std::ostringstream oss;
  // {
  //   Timer timer("cereal::BinaryOutputArchive");
  //   cereal::BinaryOutputArchive archive(oss);
  //   archive(dataToSerialize);
  // }
  

  // std::string serializedData = oss.str();
  // size_t compresssed_size = 0;
  // std::unique_ptr<char[]> compressed_buffer;
  // {
  //   Timer timer("snappy::RawCompress");
  //   compressed_buffer = 
  //     CompressData(serializedData.c_str(), (size_t)serializedData.size(), compresssed_size);
  // }

  // // At Client end;

  // std::unique_ptr<char[]> uncompressed_buffer(new char[serializedData.size()]);
  // {
  //   Timer timer("snappy::Uncompress");
  //   uncompressed_buffer = DecompressData(compressed_buffer.get(), compresssed_size, (size_t)(serializedData.size()));
  // }

  // MyStruct receivedStruct;
  // {
  //   Timer timer("cereal::BinaryInputArchive");
  //   // std::istringstream iss(std::string(decompressed_data.begin(), decompressed_data.begin() + decompressed_data.size()));
  //   std::istringstream iss(std::string(uncompressed_buffer.get(), 
  //     uncompressed_buffer.get() + serializedData.size()));

  //   cereal::BinaryInputArchive archive(iss);
  //   archive(receivedStruct);
  // }

  // assert((receivedStruct.vertex[N_POINTS - 1].Color.v0) == 0.160784f);
  // std::cout << " allocations: " << s_AllocCount << std::endl;
  return 0;
}