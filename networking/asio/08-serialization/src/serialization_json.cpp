#include <cereal/cereal.hpp>
#include <cereal/archives/json.hpp>

struct MyStruct {
  int intValue;
  double doubleValue;
  std::string stringValue;

  template <class Archive>
  void serialize(Archive& archive) {
    archive(intValue, doubleValue, stringValue);
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

  
  std::stringstream oss;
  // Serialize to JSON
  {
    MyStruct data = { 42, 3.14, "Hello Cereal!" };
    cereal::JSONOutputArchive archive(oss);
    archive(data);
  }

  
  // De-Serialize from JSON
  {
    MyStruct loadedData;
    cereal::JSONInputArchive archive(oss);
    archive(loadedData);

    // Use the deserialized data
    std::cout << "intValue: " << loadedData.intValue << std::endl;
    std::cout << "doubleValue: " << loadedData.doubleValue << std::endl;
    std::cout << "stringValue: " << loadedData.stringValue << std::endl;
  }



  return 0;
}