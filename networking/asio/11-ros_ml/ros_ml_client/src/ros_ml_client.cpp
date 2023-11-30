
#include "ros_ml_client/ros_ml_client.h"
#include <thread>
#include <algorithm>
#include <iterator>
#include <iostream>

RosMLClient::RosMLClient() : olc::GameEngine() , olc::net::client_interface<GameMsg>()
{
	sAppName = "MMO Client";
}

RosMLClient::~RosMLClient()
{
	std::cout << "inside ~RosMLClient()\n";
}

bool RosMLClient::OnUserCreate()
{
	mapObjects_[0].nUniqueID = 0;

	if (Connect("127.0.0.1", 60000))
	{
		return true;
	}

	return false;
}

bool RosMLClient::OnUserUpdate(float fElapsedTime)
{
	// Check for incoming network messages
	if (IsConnected())
	{
		while (!Incoming().empty())
		{
			auto msg = Incoming().pop_front().msg;

			switch (msg.header.id)
			{
				case(GameMsg::Client_Accepted):
				{
					std::cout << "Server accepted client - you're in!\n";
					olc::net::message<GameMsg> msg;
					msg.header.id = GameMsg::Client_RegisterWithServer;

					msg << descPlayer_;
					Send(msg);
					break;
				}

				case(GameMsg::Client_AssignID):
				{
					// Server is assigning us OUR id
					msg >> nPlayerID_;
					std::cout << "Assigned Client ID = " << nPlayerID_ << "\n";

					break;
				}

				case(GameMsg::Client_RegisterWithServer):
				{
					break;
				}

				case(GameMsg::Client_UnregisterWithServer):
				{
					break;
				}

				case(GameMsg::Game_AddPlayer):
				{
					sPlayerDescription desc;
					msg >> desc;

					mapObjects_.insert_or_assign(desc.nUniqueID, desc);

					if (desc.nUniqueID == nPlayerID_)
					{
						// Now we exist in game world
						bWaitingForConnection_ = false;
					}
					break;
				}

				case(GameMsg::Game_RemovePlayer):
				{
					uint32_t nRemovalID = 0;
					msg >> nRemovalID;
					mapObjects_.erase(nRemovalID);
					break;
				}

				case(GameMsg::Game_UpdatePlayer):
				{
					sPlayerDescription desc;
					msg >> desc;
					
					
					mapObjects_.insert_or_assign(desc.nUniqueID, desc);
					
					std::cout << "test_message: " << desc.test_message << std::endl;
					std::cout << desc.image_data[100] << std::endl;
					GameEngine::PublishImage(desc.image_data);

					break;
				}
			}
		}
	}

	if (bWaitingForConnection_)
	{
		return true;
	}

	olc::net::message<GameMsg> msg;
	// Send player description
	msg.header.id = GameMsg::Game_UpdatePlayer;

	mapObjects_[nPlayerID_].t_ml_ros = t_ml_ros;
	
	msg << mapObjects_[nPlayerID_];

	Send(msg);
	return true;
}

int main(void)
{
	RosMLClient ros_ml_client;
	if (ros_ml_client.Construct(800, 600))
		ros_ml_client.Start();
	return 0;
}