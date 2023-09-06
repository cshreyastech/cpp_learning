#ifndef ROS_ML_CLIENT_H
#define ROS_ML_CLIENT_H


#include "../../ros_ml_server/include/ros_ml_server/ros_ml_common.h"
#include "../../ros_ml_server/include/ros_ml_server/ros_ml_network.h"
#include "ros_ml_client/game_engine.h"

#include <unordered_map>

class RosMLClient : public olc::GameEngine, olc::net::client_interface<GameMsg>
{
public:
	RosMLClient();

private:
	std::unordered_map<uint32_t, sPlayerDescription> mapObjects;
	uint32_t nPlayerID = 0;
	sPlayerDescription descPlayer;

	bool bWaitingForConnection = true;

public:
	bool OnUserCreate() override;
  bool OnUserUpdate(float fElapsedTime);
};

#endif