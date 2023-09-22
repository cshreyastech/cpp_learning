#ifndef ROS_ML_COMMON_H
#define ROS_ML_COMMON_H
#include <cstdint>

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
	uint32_t n_points = 0;

	float data_from_ml = 0.0f; 
	// bool cloud_set_for_client = false;
	float vertices[150000 * 6]; 
};

#endif