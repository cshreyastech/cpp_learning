#ifndef ROS_ML_COMMON_H
#define ROS_ML_COMMON_H
#include <cstdint>

#include "snappy-internal.h"
#include "snappy-sinksource.h"
#include "snappy.h"

#include "ros_ml_server/visual_benchmarking.h"

#define N_POINTS  2

#define PROFILING 0
#if PROFILING
  #define PROFILE_SCOPE(name) InstrumentationTimer timer##__LINE__(name)
  #define PROFILE_FUNCTION() PROFILE_SCOPE(__PRETTY_FUNCTION__)
#else
  #define PROFILE_SCOPE(name)
	#define PROFILE_FUNCTION()
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

// https://stackoverflow.com/questions/17424731/implementing-flexible-array-members-with-templates-and-base-class
struct sPlayerDescription
{	
	uint32_t nUniqueID = 0;

	float data_from_ml = 0.0f; 
	char p_vertices_compressed[1]; 
};

template<size_t N>
struct CompressedStruct : public sPlayerDescription
{
	char p_vertices_compressed[N - 1];
};
#endif