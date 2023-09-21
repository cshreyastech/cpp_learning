
#include "ros_ml_client/ros_ml_client.h"
#include <thread>
#include <algorithm>
#include <iterator>
#include <iostream>
#include <cassert>

RosMLClient::RosMLClient() : olc::GameEngine() , olc::net::client_interface<GameMsg>()
{
	sAppName = "MMO Client";
}

RosMLClient::~RosMLClient()
{
	std::cout << "inside ~RosMLClient()\n";
	// delete[] vertices;
}

bool RosMLClient::OnUserCreate()
{
	mapObjects_[0].nUniqueID = 0;
	//mapObjects_[0].vPos = { 3.0f, 3.0f };
	mapObjects_[0].n_points = 0;
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
					// descPlayer_.vPos = { 3.0f, 3.0f };
					descPlayer_.n_points = 0;
					msg << descPlayer_;
					Send(msg);
					break;
				}

				case(GameMsg::Client_AssignID):
				{
					// Server is assigning us OUR id
					uint32_t n_points;
					msg >> n_points;
					msg >> nPlayerID_;

					std::cout << "Assigned Client ID = " << nPlayerID_ << "\n";
					std::cout << "n_points = " << n_points << "\n";
					
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
					
					// const int n_points = desc.n_points;
					// const int vertices_length = n_points * 6;
					// assert(desc.vertices[vertices_length - 1] == 0.031373f);
					
					break;
				}
			}
		}
	}

	if (bWaitingForConnection_)
	{
		return true;
	}

	const int n_points = mapObjects_[nPlayerID_].n_points;
	const int vertices_length = n_points * 6;
	PublishCoud(mapObjects_[nPlayerID_].vertices, n_points);

	// std::cout 
	// 	<< "mapObjects_[nPlayerID_].vertices[vertices_length - 1] 0.031373f:" 
	// 	<< mapObjects_[nPlayerID_].vertices[vertices_length - 1] 
	// 	<< std::endl;

	// assert(mapObjects_[nPlayerID_].vertices[vertices_length - 1] == 0.031373f);

	// Send player description
	olc::net::message<GameMsg> msg;
	msg.header.id = GameMsg::Game_UpdatePlayer;
	msg << mapObjects_[nPlayerID_];


	Send(msg);

	return true;
}

void RosMLClient::Deserialize(const char* data, float vertices[], const int vertices_length)
{
  float *q = (float*)data;
  for(int i = 0; i < vertices_length; i++)
  {
    vertices[i] = *q; q++;
  }
}


int main(void)
{
	RosMLClient ros_ml_client;
	if (ros_ml_client.Construct(800, 600))
		ros_ml_client.Start();
	return 0;
}