#pragma once

#include <cstdint>
#include "snappy-internal.h"
#include "snappy-sinksource.h"
#include "snappy.h"

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
	size_t p_vertices_compressed_length = 0; 
	// char p_vertices_compressed[]; // Flexible array member
};
