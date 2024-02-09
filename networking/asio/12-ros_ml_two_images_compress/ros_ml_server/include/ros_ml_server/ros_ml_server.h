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
#include <cassert>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

class RosMLServer : public olc::net::server_interface<GameMsg>
{
public:
	RosMLServer(const std::string cloud_file_path, uint16_t nPort);
 ~RosMLServer();

protected:
	bool OnClientConnect(std::shared_ptr<olc::net::connection<GameMsg>> client) override;
  void OnClientValidated(std::shared_ptr<olc::net::connection<GameMsg>> client) override;
  void OnClientDisconnect(std::shared_ptr<olc::net::connection<GameMsg>> client) override;
  void OnMessage(std::shared_ptr<olc::net::connection<GameMsg>> client, olc::net::message<GameMsg>& msg) override;

private:
  std::unordered_map<uint32_t, sPlayerDescription> m_mapPlayerRoster_;
	std::vector<uint32_t> m_vGarbageIDs_; 
  std::string cloud_file_path_{""};
  std::string left_endoscope_image_path_{""};
  std::string right_endoscope_image_path_{""};
};

#endif