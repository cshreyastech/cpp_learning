#include <cereal/archives/binary.hpp>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>

struct MyStruct {
  int intValue1;

  // double doubleValue;

  // Serialization method using smart pointers
  template <class Archive>
  void serialize(Archive& archive) {
    archive(intValue1);
  }
};

int main() {
  // Create an instance of MyStruct using std::shared_ptr
  // std::shared_ptr<MyStruct> myStruct = std::make_shared<MyStruct>();
  std::shared_ptr<MyStruct> myStruct(new MyStruct);
  // std::shared_ptr<MyStruct> myStruct;
  // std::weak_ptr<MyStruct> myStruct = myStruct_shared;

  // std::unique_ptr<MyStruct> myStruct = std::make_unique<MyStruct>();
  // std::unique_ptr<MyStruct> myStruct(new MyStruct);
  // std::unique_ptr<MyStruct> myStruct;
  myStruct->intValue1 = 42;

  // Serialize the std::shared_ptr to a binary file
  {
    std::ofstream ofs("data.bin", std::ios::binary);
    cereal::BinaryOutputArchive archive(ofs);
    archive(myStruct);
  }

  return 0;
}