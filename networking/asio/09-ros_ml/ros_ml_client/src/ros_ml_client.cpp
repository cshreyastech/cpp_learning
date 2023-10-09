
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
					size_t* q = (size_t*)msg.body.data();
					size_t point_cloud_compressed_length = *q;

					const size_t data_size = sizeof(sPlayerDescription) + point_cloud_compressed_length;

					std::unique_ptr<sPlayerDescription, void(*)(sPlayerDescription*)> desc_from_server(
						reinterpret_cast<sPlayerDescription*>(new char[data_size]),
						[](sPlayerDescription* ptr) {
						delete[] reinterpret_cast<char*>(ptr); // Properperly deallocate the memory
						}
					);
					
					{					
						Timer timer("ReadMessage");
						ReadMessage(msg, *desc_from_server, data_size);
					}
					sPlayerDescription desc_from_server_stack;

					desc_from_server_stack.nUniqueID = desc_from_server->nUniqueID;
					desc_from_server_stack.point_cloud_compressed_length = 
						desc_from_server->point_cloud_compressed_length;
					desc_from_server_stack.data_from_ml = desc_from_server->data_from_ml;

					std::string decompressed_data;
					
					{
						Timer timer("Uncompress");
						if (!snappy::Uncompress(desc_from_server->point_cloud_compressed, 
							desc_from_server->point_cloud_compressed_length, &decompressed_data))
								throw std::invalid_argument("Decompression failed.");
					}


					std::istringstream iss(std::string(decompressed_data.begin(), decompressed_data.begin() + decompressed_data.size()));
					{
						Timer timer("Deserialize");
						cereal::BinaryInputArchive archive(iss);
						archive(to_serilize_point_cloud_);
					}

					mapObjects_.insert_or_assign(desc_from_server->nUniqueID, desc_from_server_stack);

					break;
				}
			}
		}
	}

	if (bWaitingForConnection_)
	{
		return true;
	}

	{
		// Timer timer("PublishCloud");
		GameEngine::PublishCloud(to_serilize_point_cloud_.point_cloud);
	}

	// Get head and eye pose from ML and send it back to server
	mapObjects_[nPlayerID_].data_from_ml = 1.001f;

	// Send player description
	olc::net::message<GameMsg> msg;
	msg.header.id = GameMsg::Game_UpdatePlayer;
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