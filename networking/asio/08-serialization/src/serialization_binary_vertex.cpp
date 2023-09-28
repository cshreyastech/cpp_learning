#include <cereal/archives/binary.hpp>
#include <iostream>
#include <vector>
#include <stdio.h>

struct Vec3
{
	float v0, v1, v2;	
};

struct Vertex
{
	Vec3 Position;
	// Vec3 Color;
};



struct MyStruct {
  // int intValue;
  // double doubleValue;
  // Vertex vertex;
  float x;
  float y;
  float z;

  float r;
  float g;
  float b;

  template <class Archive>
  void serialize(Archive& archive) {
    // archive(intValue, doubleValue);
    // archive(vertex);
    archive(x, y, z, r, g, b);
  }
};



int main()
{

  // Vertex v;
  // v.Position.v0 = 1.1f;
  // v.Position.v1 = 1.2f;
  // v.Position.v2 = 1.3f;

  // v.Color.v0 = 2.1f;
  // v.Color.v1 = 2.2f;
  // v.Color.v2 = 2.3f;

  
  MyStruct dataToSerialize = { 1.1f, 1.2f, 1.3f, 2.1f, 2.2f, 2.3f };
  // dataToSerialize.vertex.Position = {1.1f, 1.2f, 1.3f};
  // dataToSerialize.vertex.Color = {2.1f, 2.2f, 2.3f};


  // Serialize to a binary buffer
  std::vector<char> binaryData;
  {
    std::ostringstream oss;
    cereal::BinaryOutputArchive archive(oss);
    archive(dataToSerialize);

    std::string serializedData = oss.str();
    binaryData.assign(serializedData.begin(), serializedData.end());
  }

  MyStruct receivedStruct;
  {
    std::istringstream iss(std::string(binaryData.begin(), binaryData.begin() + binaryData.size()));
    cereal::BinaryInputArchive archive(iss);
    archive(receivedStruct);
  }

  // Use the deserialized data
  printf("receivedStruct(%f, %f, %f, %f, %f, %f)\n", 
    receivedStruct.x, receivedStruct.y, receivedStruct.z, 
    receivedStruct.r, receivedStruct.g, receivedStruct.b 
  );
  
  return 0;
}