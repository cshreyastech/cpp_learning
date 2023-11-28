#include <iostream>
#include <sstream>
#include <vector>
#include <thread>
#include <mutex>
#include <cereal/archives/binary.hpp>

#include <iostream>
#include <stdio.h>
#include <fstream>
#include <memory>

// #include "snappy-internal.h"
// #include "snappy-sinksource.h"
// #include "snappy.h"
// #include <chrono>
#include <cassert>

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







std::mutex archiveMutex; // Mutex to synchronize access to BinaryOutputArchive
std::string resultString; // String to store the serialized data

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

  // Serialization function
  template <class Archive>
  void serialize(Archive& archive) {
    archive(vertex);
  }
};

void saveDataToStream(std::stringstream& ss, const MyStruct& data) {
  std::lock_guard<std::mutex> lock(archiveMutex); // Lock the mutex
  cereal::BinaryOutputArchive archive(ss);
  archive(data);
  // The lock will be released when the function exits and lock goes out of scope
}

std::size_t currentPosition = 0;
MyStruct deserializedData;

void deserializeData(const std::string& data) {
  std::lock_guard<std::mutex> lock(archiveMutex); // Lock the mutex
  std::istringstream iss(data);
  iss.seekg(currentPosition); // Set the position in the string
  cereal::BinaryInputArchive archive(iss);
  
  // Deserialize your data structure
  // MyStruct deserializedData;
  archive(deserializedData);
  
  // Perform operations with deserialized data
  // ...
  
  // Update the current position in the string
  currentPosition = iss.tellg();
  // The lock will be released when the function exits and lock goes out of scope
}




int main() {
  // Data to be serialized
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

  // Data data;

  // Number of threads to use for serialization
  int numThreads = 4;

  // Launch multiple threads to serialize data concurrently
  std::vector<std::thread> threads;
  std::vector<std::stringstream> threadOutputStreams(numThreads);

  {
    Timer timer("Serialize");
    for (int i = 0; i < numThreads; ++i) {
      threads.emplace_back(saveDataToStream, std::ref(threadOutputStreams[i]), std::cref(dataToSerialize));
    }
  
    // Wait for all threads to finish
    for (auto& thread : threads) {
      thread.join();
    }

    // Combine serialized data from all threads into the resultString
    for (const auto& threadOutputStream : threadOutputStreams) {
      resultString += threadOutputStream.str();
    }
  }
  // ResultString now contains the serialized data from all threads
  // std::cout << "Serialized Data: " << resultString << std::endl;

  {
    Timer timer("Deserialize");
    // Number of threads to use for deserialization
    int numThreads_de = 4;

    // Launch multiple threads to deserialize data concurrently
    std::vector<std::thread> threads_de;
    for (int i = 0; i < numThreads_de; ++i) {
      threads_de.emplace_back(deserializeData, std::cref(resultString));
    }

    // Wait for all threads to finish
    for (auto& thread : threads_de) {
      thread.join();
    }
  }
  // assert(dataToSerialize.vertex[333].Position.v1 == deserializeData.vertex[333].Position.v1);
  std::cout <<  dataToSerialize.vertex[333].Position.v1 << std::endl;
  std::cout <<  deserializedData.vertex[333].Position.v1 << std::endl;
  return 0;
}