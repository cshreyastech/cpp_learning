#ifndef ROS_ML_COMMON_H
#define ROS_ML_COMMON_H

#include <cstdint>
#include "snappy-internal.h"
#include "snappy-sinksource.h"
#include "snappy.h"
#include <cereal/archives/binary.hpp>
#include <stdexcept>
#include <memory>
#include "ros_ml_server/Timer.h"

#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>

// Endoscope to eye or head transformation
struct T_ML_ROS
{
  glm::quat Q_EC_HEAD;
  glm::vec3 P_EC_HEAD;

  glm::quat Q_EC_FIX;
  glm::vec3 P_EC_FIX;
};

const int N_POINTS = 100000;

enum class GameMsg : uint32_t
{
	Client_Accepted,
	Client_AssignID,
	Client_RegisterWithServer,
	Client_UnregisterWithServer,

	Game_AddPlayer,
	Game_RemovePlayer,
	Game_UpdatePlayer
};

struct sPlayerDescription
{
	// point_cloud_compressed_length should be the first element of the struct
	// as this is extracted by the client for creating heap variable. 
	size_t point_cloud_compressed_length = 0;	
	uint32_t nUniqueID = 0;

  T_ML_ROS t_ml_ros;
  // glm::quat Q_EC_HEAD;
  // glm::vec3 P_EC_HEAD;

  // glm::quat Q_EC_FIX;
  // glm::vec3 P_EC_FIX;

  // float pose_from_client = 0;
	char point_cloud_compressed[1]; // Flexible array member
};

struct Vec3
{
  float v0, v1, v2;

  template <class Archive>
  void serialize(Archive& archive) {
    archive(v0, v1, v2);
  }
};

struct Point
{
  Vec3 Position;
  Vec3 Color;
  
  template <class Archive>
  void serialize(Archive& archive) {
    archive(Position, Color);
  }
};

struct ToSerilizePointCloud {
	// number of points
  Point point_cloud[N_POINTS];

  template <class Archive>
  void serialize(Archive& archive) {
    archive(point_cloud);
  }
};

#endif