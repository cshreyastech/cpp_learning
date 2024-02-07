
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

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>


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

// struct StructBasicDataType {
//   int int_value;
//   double double_value;

//   template <class Archive>
//   void serialize(Archive& archive) {
//     archive(int_value, double_value);
//   }
// };

// struct StructArray {
//   int int_array[2];
  

//   template <class Archive>
//   void serialize(Archive& archive) {
//     archive(int_array);
//   }
// };


// struct StructMatTest {
//   uint8_t image_array[2][3];
  

//   template <class Archive>
//   void serialize(Archive& archive) {
//     archive(image_array);
//   }
// };


struct StructMat {
  unsigned char left_image_array[336][448];
  unsigned char right_image_array[336][448];
  

  template <class Archive>
  void serialize(Archive& archive) {
    archive(left_image_array, right_image_array);
  }
};



std::unique_ptr<char[]> CompressData(const char* data, size_t data_size, size_t& compressed_size)
{
  size_t max_compressed_size = snappy::MaxCompressedLength(data_size);

  std::unique_ptr<char[]> compressed_buffer(new char[max_compressed_size]);

  snappy::RawCompress(data, data_size, compressed_buffer.get(), &compressed_size);
  if(!snappy::IsValidCompressedBuffer(compressed_buffer.get(), compressed_size))
  {
    std::cerr << "Compression failed!" << std::endl;

    return nullptr;
  }

  return compressed_buffer;
} 


// std::unique_ptr<char[]> DecompressData(const std::vector<char>& compressed_data, size_t uncompressed_size) 
std::unique_ptr<char[]> DecompressData(const char* compressed_data, size_t compressed_size, size_t uncompressed_size) 
{
  std::unique_ptr<char[]> uncompressed_buffer(new char[uncompressed_size]);

  // Attempt to decompress the data
  // if (snappy::RawUncompress(compressed_data.data(), compressed_data.size(), uncompressed_buffer.get())) {
  if (!snappy::RawUncompress(compressed_data, compressed_size, uncompressed_buffer.get())) 
  {
    std::cerr << "Decompression failed!" << std::endl;
    return nullptr;
  }

  return uncompressed_buffer;
}


int main()
{ 

  // cv::Mat left_endoscope_image = cv::imread("../external/endo_images/task_hard_left_endoscope_video.png", 
  //     cv::IMREAD_COLOR);

  /*
  // Working
  StructBasicDataType dataToSerialize = {42, 3.14};


  std::ostringstream oss;
  {
    Timer timer("cereal::BinaryOutputArchive");
    cereal::BinaryOutputArchive archive(oss);
    archive(dataToSerialize);
  }
  

  std::string serializedData = oss.str();
  size_t compresssed_size = 0;
  std::unique_ptr<char[]> compressed_buffer;
  {
    Timer timer("snappy::RawCompress");
    compressed_buffer = 
      CompressData(serializedData.c_str(), (size_t)serializedData.size(), compresssed_size);
  }

  // At Client end;

  std::unique_ptr<char[]> uncompressed_buffer(new char[serializedData.size()]);
  {
    Timer timer("snappy::Uncompress");
    uncompressed_buffer = DecompressData(compressed_buffer.get(), compresssed_size, (size_t)(serializedData.size()));
  }

  StructBasicDataType receivedStruct;
  {
    Timer timer("cereal::BinaryInputArchive");
    // std::istringstream iss(std::string(decompressed_data.begin(), decompressed_data.begin() + decompressed_data.size()));
    std::istringstream iss(std::string(uncompressed_buffer.get(), 
      uncompressed_buffer.get() + serializedData.size()));

    cereal::BinaryInputArchive archive(iss);
    archive(receivedStruct);
  }

  assert(receivedStruct.int_value == dataToSerialize.int_value);
  assert(receivedStruct.double_value == dataToSerialize.double_value);
  */


  ////////////////////////////////////////////
  // Working on StructArray
  // StructArray data_to_serialize_struct_array;
  // int int_array[2] = { 0, 1};
  // memcpy(data_to_serialize_struct_array.int_array, int_array, sizeof(int_array));


  // std::ostringstream oss_struct_array;
  // {
  //   Timer timer("cereal::BinaryOutputArchive");
  //   cereal::BinaryOutputArchive archive(oss_struct_array);
  //   archive(data_to_serialize_struct_array);
  // }

  // std::string serialized_data_str_struct_array = oss_struct_array.str();
  // size_t compressed_size_struct_array = 0;
  // std::unique_ptr<char[]> compressed_buffer_ptr_struct_array;
  // {
  //   Timer timer("snappy::RawCompress");
  //   compressed_buffer_ptr_struct_array = 
  //     CompressData(serialized_data_str_struct_array.c_str(), (size_t)serialized_data_str_struct_array.size(), 
  //     compressed_size_struct_array);

  // }



  // // At Client end;

  // std::unique_ptr<char[]> uncompressed_buffer_struct_array(new char[serialized_data_str_struct_array.size()]);
  // {
  //   Timer timer("snappy::Uncompress");
  //   uncompressed_buffer_struct_array = DecompressData(compressed_buffer_ptr_struct_array.get(), 
  //     compressed_size_struct_array, (size_t)(serialized_data_str_struct_array.size()));
  // }

  // StructArray received_struct_struct_array;
  // {
  //   Timer timer("cereal::BinaryInputArchive");
  //   std::istringstream iss(std::string(uncompressed_buffer_struct_array.get(), 
  //     uncompressed_buffer_struct_array.get() + serialized_data_str_struct_array.size()));

  //   cereal::BinaryInputArchive archive(iss);
  //   archive(received_struct_struct_array);
  // }

  // assert(received_struct_struct_array.int_array[0] == data_to_serialize_struct_array.int_array[0]);
  // assert(received_struct_struct_array.int_array[1] == data_to_serialize_struct_array.int_array[1]);
  ///////////////////////////////////////////////////////////////////////////////////////////////
  // // Working on StructMatTest
  // StructMatTest data_to_serialize_struct_mat;
  // uint8_t image_array[2][3] =
  // {
  //   { 0, 1, 2},
  //   { 3, 4, 5}
  // };

  // memcpy(data_to_serialize_struct_mat.image_array, image_array, sizeof(image_array));

  // std::ostringstream oss_struct_mat;
  // {
  //   Timer timer("cereal::BinaryOutputArchive");
  //   cereal::BinaryOutputArchive archive(oss_struct_mat);
  //   archive(data_to_serialize_struct_mat);
  // }


  // std::string serialized_data_str_struct_mat = oss_struct_mat.str();
  // size_t compressed_size_struct_mat = 0;
  // std::unique_ptr<char[]> compressed_buffer_ptr_struct_mat;
  // {
  //   Timer timer("snappy::RawCompress");
  //   compressed_buffer_ptr_struct_mat = 
  //     CompressData(serialized_data_str_struct_mat.c_str(), (size_t)serialized_data_str_struct_mat.size(), 
  //     compressed_size_struct_mat);
  // }

  // // At Client end;

  // std::unique_ptr<char[]> uncompressed_buffer_struct_mat(new char[serialized_data_str_struct_mat.size()]);
  // {
  //   Timer timer("snappy::Uncompress");
  //   uncompressed_buffer_struct_mat = DecompressData(compressed_buffer_ptr_struct_mat.get(), 
  //     compressed_size_struct_mat, (size_t)(serialized_data_str_struct_mat.size()));
  // }

  // StructMatTest received_struct_struct_mat;
  // {
  //   Timer timer("cereal::BinaryInputArchive");
  //   std::istringstream iss(std::string(uncompressed_buffer_struct_mat.get(), 
  //     uncompressed_buffer_struct_mat.get() + serialized_data_str_struct_mat.size()));

  //   cereal::BinaryInputArchive archive(iss);
  //   archive(received_struct_struct_mat);
  // }


  // assert(received_struct_struct_mat.image_array[0][2] == data_to_serialize_struct_mat.image_array[0][2]);
  // assert(received_struct_struct_mat.image_array[1][2] == data_to_serialize_struct_mat.image_array[1][2]);
  // for(int row = 0; row < 2; row++)
  // {
  //   for(int col = 0; col < 3; col++)
  //   {
  //     assert(received_struct_struct_mat.image_array[row][col] == data_to_serialize_struct_mat.image_array[row][col]); 
  //   }
  // }

  ///////////////////////////////////////////////////////////////////////////////////////////////
  // Working on StructMatactualImage
  cv::Mat left_endoscope_image_mat = cv::imread("../external/endo_images/task_hard_left_endoscope_video.png", 
    cv::IMREAD_COLOR);

  cv::Mat right_endoscope_image_mat = cv::imread("../external/endo_images/task_hard_right_endoscope_video.png", 
    cv::IMREAD_COLOR);

  const int left_endoscope_image_mat_rows = left_endoscope_image_mat.rows;
  const int left_endoscope_image_mat_cols = left_endoscope_image_mat.cols;

  const int right_endoscope_image_mat_rows = right_endoscope_image_mat.rows;
  const int right_endoscope_image_mat_cols = right_endoscope_image_mat.cols;

  printf("left_endoscope_image_mat(%d, %d)\n", left_endoscope_image_mat_rows, left_endoscope_image_mat_cols);
  printf("right_endoscope_image_mat(%d, %d)\n", right_endoscope_image_mat_rows, right_endoscope_image_mat_cols);


  StructMat data_to_serialize_struct_mat;
 

  memcpy(data_to_serialize_struct_mat.left_image_array, 
    left_endoscope_image_mat.data, sizeof(left_endoscope_image_mat));

  memcpy(data_to_serialize_struct_mat.right_image_array, 
    right_endoscope_image_mat.data, sizeof(right_endoscope_image_mat));


  std::ostringstream oss_struct_mat;
  {
    Timer timer("cereal::BinaryOutputArchive");
    cereal::BinaryOutputArchive archive(oss_struct_mat);
    archive(data_to_serialize_struct_mat);
  }


  std::string serialized_data_str_struct_mat = oss_struct_mat.str();
  size_t compressed_size_struct_mat = 0;
  std::unique_ptr<char[]> compressed_buffer_ptr_struct_mat;
  {
    Timer timer("snappy::RawCompress");
    compressed_buffer_ptr_struct_mat = 
      CompressData(serialized_data_str_struct_mat.c_str(), (size_t)serialized_data_str_struct_mat.size(), 
      compressed_size_struct_mat);
  }

  // At Client end;

  std::unique_ptr<char[]> uncompressed_buffer_struct_mat(new char[serialized_data_str_struct_mat.size()]);
  {
    Timer timer("snappy::Uncompress");
    uncompressed_buffer_struct_mat = DecompressData(compressed_buffer_ptr_struct_mat.get(), 
      compressed_size_struct_mat, (size_t)(serialized_data_str_struct_mat.size()));
  }

  StructMat received_struct_struct_mat;
  {
    Timer timer("cereal::BinaryInputArchive");
    std::istringstream iss(std::string(uncompressed_buffer_struct_mat.get(), 
      uncompressed_buffer_struct_mat.get() + serialized_data_str_struct_mat.size()));

    cereal::BinaryInputArchive archive(iss);
    archive(received_struct_struct_mat);
  }


  // assert(received_struct_struct_mat.left_image_array[0][2] == data_to_serialize_struct_mat.left_image_array[0][2]);
  // assert(received_struct_struct_mat.left_image_array[100][2] == data_to_serialize_struct_mat.left_image_array[100][2]);
  // assert(received_struct_struct_mat.left_image_array[100][447] == data_to_serialize_struct_mat.left_image_array[100][447]);
  // assert(received_struct_struct_mat.left_image_array[200][447] == data_to_serialize_struct_mat.left_image_array[200][447]);
  // assert(received_struct_struct_mat.left_image_array[250][447] == data_to_serialize_struct_mat.left_image_array[250][447]);
  for(int row = 0; row < left_endoscope_image_mat_rows; row++)
  {
    for(int col = 0; col < left_endoscope_image_mat_cols; col++)
    {
      assert(received_struct_struct_mat.left_image_array[row][col] == 
        data_to_serialize_struct_mat.left_image_array[row][col]); 

      assert(received_struct_struct_mat.right_image_array[row][col] == 
        data_to_serialize_struct_mat.right_image_array[row][col]); 
    }
  }

  // cv::Mat received_mat = cv::Mat(left_endoscope_image_mat_rows, 
  //   left_endoscope_image_mat_cols, CV_8UC1, left_endoscope_image_mat.data);

  // cv::imshow("OPENCV_LEFT ENDO ACTUAL_WINDOW", left_endoscope_image_mat);
  // cv::imshow("OPENCV_LEFT ENDO RECIEVED_WINDOW", received_mat);
  // cv::waitKey(0);


  std::cout << " allocations: " << s_AllocCount << std::endl;
  return 0;
}