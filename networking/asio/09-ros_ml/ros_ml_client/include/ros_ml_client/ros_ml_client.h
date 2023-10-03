#ifndef ROS_ML_CLIENT_H
#define ROS_ML_CLIENT_H


// #include "../../ros_ml_server/include/ros_ml_server/ros_ml_common.h"
#include "../../ros_ml_server/include/ros_ml_server/ros_ml_network.h"
#include "ros_ml_client/game_engine.h"

#include <unordered_map>
#include <cassert>

class RosMLClient : public olc::GameEngine, olc::net::client_interface<GameMsg>
{
public:
	RosMLClient();
 ~RosMLClient();


private:
	std::unordered_map<uint32_t, sPlayerDescription> mapObjects_;
	uint32_t nPlayerID_ = 0;
	bool bWaitingForConnection_{true};
	sPlayerDescription descPlayer_;
	PointCloud point_cloud_;

public:
	bool OnUserCreate() override;
  bool OnUserUpdate(float fElapsedTime) override;

};

#endif