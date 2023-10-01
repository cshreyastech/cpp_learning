#ifndef ROS_ML_COMMON_H
#define ROS_ML_COMMON_H
#include <cstdint>
#include "ros_ml_server/visual_benchmarking.h"
#include <cereal/archives/binary.hpp>

#define PROFILING 1
#if PROFILING
  #define PROFILE_SCOPE(name) InstrumentationTimer timer##__LINE__(name)
  #define PROFILE_FUNCTION() PROFILE_SCOPE(__PRETTY_FUNCTION__)
#else
  #define PROFILE_SCOPE(name)
#endif

enum class GameMsg : uint32_t
{
	Server_GetStatus,
	Server_GetPing,

	Client_Accepted,
	Client_AssignID,
	Client_RegisterWithServer,
	Client_UnregisterWithServer,

	Game_AddPlayer,
	Game_RemovePlayer,
	Game_UpdatePlayer,
};

struct sPlayerDescription
{	
	uint32_t nUniqueID = 0;
	uint32_t n_points = 0;

	float data_from_ml = 0.0f; 
	float vertices[14 * 6]; 
};

#endif