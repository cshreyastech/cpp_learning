#include <cereal/archives/binary.hpp>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>

struct MyStruct {
  int intValue;
  // double doubleValue;

  // Serialization method using smart pointers
  template <class Archive>
  void serialize(Archive& archive) {
    archive(intValue);
  }
};

int main() {
  // Create an instance of MyStruct using std::shared_ptr
  std::shared_ptr<MyStruct> myStruct = std::make_shared<MyStruct>();
  myStruct->intValue = 42;

  // Serialize the std::shared_ptr to a binary file
  {
    std::ofstream ofs("data.bin", std::ios::binary);
    cereal::BinaryOutputArchive archive(ofs);
    archive(myStruct);
  }

  return 0;
}