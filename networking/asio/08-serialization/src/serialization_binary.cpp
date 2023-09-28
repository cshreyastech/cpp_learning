#include <cereal/archives/binary.hpp>
#include <iostream>
#include <vector>

struct MyStruct {
  int intValue;
  double doubleValue;

  template <class Archive>
  void serialize(Archive& archive) {
    archive(intValue, doubleValue);
  }
};

struct Vec3
{
	float v0, v1, v2;	
};

struct Vertex
{
	Vec3 Position;
	Vec3 Color;
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

  
  MyStruct dataToSerialize = {42, 3.14};

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
  std::cout << "Received intValue: " << receivedStruct.intValue << std::endl;
  std::cout << "Received doubleValue: " << receivedStruct.doubleValue << std::endl;

  return 0;
}