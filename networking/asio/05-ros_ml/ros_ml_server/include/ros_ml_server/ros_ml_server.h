#ifndef ROS_ML_CLIENT_H
#define ROS_ML_CLIENT_H

#include <iostream>
#include <unordered_map>

#include "ros_ml_server/ros_ml_common.h"
#include "ros_ml_server/ros_ml_network.h"

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <fstream>

class RosMLServer : public olc::net::server_interface<GameMsg>
{
public:
	RosMLServer(uint16_t nPort);
 ~RosMLServer();

private:
  std::unordered_map<uint32_t, sPlayerDescription> m_mapPlayerRoster;
	std::vector<uint32_t> m_vGarbageIDs;
  void Serialize(const char* data, float vertices[], const int vertices_length);
  void Deserialize(const char* data, float vertices[], const int vertices_length);

protected:
	bool OnClientConnect(std::shared_ptr<olc::net::connection<GameMsg>> client) override;
  void OnClientValidated(std::shared_ptr<olc::net::connection<GameMsg>> client) override;
  void OnClientDisconnect(std::shared_ptr<olc::net::connection<GameMsg>> client) override;
  void OnMessage(std::shared_ptr<olc::net::connection<GameMsg>> client, olc::net::message<GameMsg>& msg);
};

#endif