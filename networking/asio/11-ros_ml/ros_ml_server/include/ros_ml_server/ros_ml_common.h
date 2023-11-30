#ifndef ROS_ML_COMMON_H
#define ROS_ML_COMMON_H

#include <cstdint>
#include <stdexcept>
#include <memory>
#include "ros_ml_server/Timer.h"

#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>

const size_t total_elements = 480 * 640 * 3;

// Endoscope to eye or head transformation
struct T_ML_ROS
{
  glm::quat Q_EC_HEAD;
  glm::vec3 P_EC_HEAD;

  glm::quat Q_EC_FIX;
  glm::vec3 P_EC_FIX;
};


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
	
	unsigned char test_message;
	unsigned char image_data[480 * 640 * 3];
  T_ML_ROS t_ml_ros;
};

#endif