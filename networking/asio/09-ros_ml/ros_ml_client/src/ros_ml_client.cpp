
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
					size_t* q = (size_t*)msg.body.data();
					size_t p_vertices_compressed_length = *q;

					std::cout << "p_vertices_compressed_length: " << p_vertices_compressed_length << 
					std::endl;

					const size_t data_size = sizeof(sPlayerDescription) + p_vertices_compressed_length;
					sPlayerDescription *desc_from_server = new sPlayerDescription();
					desc_from_server = (sPlayerDescription*)malloc(data_size);
					// sPlayerDescription *desc_from_server = 
					// reinterpret_cast<sPlayerDescription*>(new char[sizeof(sPlayerDescription) + sizeof(char) * p_vertices_compressed_length - 1]);


					ReadMessage(msg, *desc_from_server, data_size);
					sPlayerDescription desc_from_server_stack;

					desc_from_server_stack.p_vertices_compressed_length = desc_from_server->p_vertices_compressed_length;
					desc_from_server_stack.nUniqueID = desc_from_server->nUniqueID;
					desc_from_server_stack.n_points = desc_from_server->n_points;
					desc_from_server_stack.data_from_ml = desc_from_server->data_from_ml;
					desc_from_server_stack.cloud_set_for_client = desc_from_server->cloud_set_for_client;
					desc_from_server_stack.cloud_set_for_client = desc_from_server->cloud_set_for_client;


					// At Client end;
					std::string decompressed_data;
					
					if (snappy::Uncompress(desc_from_server->p_vertices_compressed, 
						desc_from_server->p_vertices_compressed_length, &decompressed_data)) {
						// Decompression succeeded
						std::cout << "Decompression succedded" << std::endl;
					} else {
						// Decompression failed
						std::cerr << "Decompression failed." << std::endl;
					}

					// PointCloud point_cloud;
					{
						std::istringstream iss(std::string(decompressed_data.begin(), decompressed_data.begin() + decompressed_data.size()));
						cereal::BinaryInputArchive archive(iss);
						archive(point_cloud_);
					}

					// Row number 99961 in text file
					// assert((point_cloud_.points[13].Color.v0) == 0.635294f);






					mapObjects_.insert_or_assign(desc_from_server->nUniqueID, desc_from_server_stack);
	
					const int n_points = desc_from_server->n_points;
					const int vertices_length = n_points * 6;
					const int vertices_size = vertices_length * sizeof(float);

					char p_vertices[vertices_size];

					bool raw_uncompress = 
						snappy::RawUncompress(desc_from_server->p_vertices_compressed, 
							p_vertices_compressed_length,
							p_vertices);

					// vertices = new float[vertices_length];
					// // float vertices[vertices_length];
					// Deserialize(p_vertices, vertices, vertices_length);



					// assert(vertices[vertices_length - 1] == 0.619608f);

					delete[] desc_from_server;
					break;
				}
			}
		}
	}

	if (bWaitingForConnection_)
	{
		return true;
	}


	// remove this condition
	if(mapObjects_[nPlayerID_].cloud_set_for_client)
	{
		n_points =  mapObjects_[nPlayerID_].n_points;
		// const int vertices_length = n_points * 6;
		// const int vertices_size = vertices_length * sizeof(float);

		// assert(vertices[vertices_length - 1] == 0.031373f);
		// assert(vertices[vertices_length - 1] == 0.619608f);
		// PublishCloud(mapObjects_[nPlayerID_]., const int n_points)
		GameEngine::PublishCloud(point_cloud_, n_points);
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