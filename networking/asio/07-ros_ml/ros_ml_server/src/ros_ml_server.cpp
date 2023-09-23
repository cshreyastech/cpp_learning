#include "ros_ml_server/ros_ml_server.h"
#include <cassert>
#include <algorithm>
#include <iterator>
#include <iostream>

RosMLServer::RosMLServer(const std::string cloud_file_path, uint16_t nPort) 
	:  olc::net::server_interface<GameMsg>(nPort)
{
	cloud_file_path_ = cloud_file_path;
}

RosMLServer::~RosMLServer()
{
	// delete[] vertices_;
}

bool RosMLServer::OnClientConnect(std::shared_ptr<olc::net::connection<GameMsg>> client)
{
	// For now we will allow all 
	return true;
}

void RosMLServer::OnClientValidated(std::shared_ptr<olc::net::connection<GameMsg>> client)
{
	// Client passed validation check, so send them a message informing
	// them they can continue to communicate
	olc::net::message<GameMsg> msg;
	msg.header.id = GameMsg::Client_Accepted;
	client->Send(msg);
}

void RosMLServer::OnClientDisconnect(std::shared_ptr<olc::net::connection<GameMsg>> client)
{
	if (client)
	{
		if (m_mapPlayerRoster_.find(client->GetID()) == m_mapPlayerRoster_.end())
		{
			// client never added to roster, so just let it disappear
		}
		else
		{
			auto& pd = m_mapPlayerRoster_[client->GetID()];
			std::cout << "[UNGRACEFUL REMOVAL]:" + std::to_string(pd.nUniqueID) + "\n";
			m_mapPlayerRoster_.erase(client->GetID());
			m_vGarbageIDs_.push_back(client->GetID());
		}
	}

}

void RosMLServer::OnMessage(std::shared_ptr<olc::net::connection<GameMsg>> client, olc::net::message<GameMsg>& msg)
{
	PROFILE_FUNCTION();
	if (!m_vGarbageIDs_.empty())
	{
		for (auto pid : m_vGarbageIDs_)
		{
			olc::net::message<GameMsg> m;
			m.header.id = GameMsg::Game_RemovePlayer;
			m << pid;
			std::cout << "Removing " << pid << "\n";
			MessageAllClients(m);
		}
		m_vGarbageIDs_.clear();
	}



	switch (msg.header.id)
	{
		case GameMsg::Client_RegisterWithServer:
		{
			sPlayerDescription desc;
			msg >> desc;
			desc.nUniqueID = client->GetID();
			// desc.n_points = n_points_;
			m_mapPlayerRoster_.insert_or_assign(desc.nUniqueID, desc);

			olc::net::message<GameMsg> msgSendID;
			msgSendID.header.id = GameMsg::Client_AssignID;

			msgSendID << desc.nUniqueID;
			// msgSendID << desc.n_points;
			MessageClient(client, msgSendID);

			olc::net::message<GameMsg> msgAddPlayer;
			msgAddPlayer.header.id = GameMsg::Game_AddPlayer;
			msgAddPlayer << desc;
			MessageAllClients(msgAddPlayer);

			for (const auto& player : m_mapPlayerRoster_)
			{
				olc::net::message<GameMsg> msgAddOtherPlayers;
				msgAddOtherPlayers.header.id = GameMsg::Game_AddPlayer;
				msgAddOtherPlayers << player.second;
				MessageClient(client, msgAddOtherPlayers);
			}

			break;
		}

		case GameMsg::Client_UnregisterWithServer:
		{
			break;
		}

		case GameMsg::Game_UpdatePlayer:
		{
			// Simply bounce update to everyone except incoming client
			// MessageAllClients(msg, client);
			// Get new set of vector from ROS after integration

			const int n_pts = 2;
			const int vertices_length = n_pts * 6;
			const int vertices_size = vertices_length * sizeof(float);
			// VerticesStruct<vertices_length> vertices_struct;
			sPlayerDescription desc;
			msg >> desc;

			float vertices[N_POINTS * 6] = 
			{
				-0.524111f, -0.392878f, -0.931184f, 0.635294f, 0.635294f, 0.635294f, 
				-0.522476f, -0.392882f, -0.931193f, 0.635294f, 0.635294f, 0.635294f, 
			};

			char p_vertices[vertices_size];

			Serialize(p_vertices, vertices, vertices_length);

			// Compress
			char* p_vertices_compressed = 
				new char[snappy::MaxCompressedLength(vertices_size)];

			size_t p_vertices_compressed_length_temp;

			// auto compression_start = std::chrono::high_resolution_clock::now();
			snappy::RawCompress(p_vertices, vertices_size, 
				p_vertices_compressed, &p_vertices_compressed_length_temp);

			constexpr size_t p_vertices_compressed_length = p_vertices_compressed_length_temp;
			// CompressedStruct <p_vertices_compressed_length>p_vertices_compressed_struct;

			// const size_t data_size = sizeof(sPlayerDescription) + p_vertices_compressed_length;



			// std::ifstream file_handler(cloud_file_path_);
			// std::string each_value_str;
			// int n_values_read_from_file  = 0;
			
			// while(file_handler >> each_value_str)
			// {
			// 	std::string each_value_clean_str = 
			// 		each_value_str.substr(0, each_value_str.find("f", 0));

			// 	float value_float = std::stof(each_value_clean_str);

			// 	vertices_source[n_values_read_from_file] = value_float;
			// 	n_values_read_from_file++;
			// }
			// assert(n_points_ == (n_values_read_from_file)/6);

			// std::copy(std::begin(vertices_source), 
			// 	std::end(vertices_source), std::begin(desc.vertices));

			msg << desc;
			MessageAllClients(msg);

			/// Validate
			// sPlayerDescription desc_test;
			// float vertices_test[N_POINTS * 6];
			// msg >> desc_test;
			// std::copy(std::begin(desc_test.vertices), 
			// 	std::begin(desc_test.vertices) + (N_POINTS * 6), std::begin(vertices_test));

			// // std::cout << "vertices[10] = 0.133333f: " << desc_test.vertices[10] << std::endl;
			// assert(vertices_test[11] == 0.635294f);

			// ////

			
			break;
		}

	}

}

void RosMLServer::Serialize(const char* data, float vertices[], const int vertices_length)
{
	float *q = (float*)data;
	for(int i = 0; i < vertices_length; i++)
	{
		*q = vertices[i]; q++;
	}
}

// void RosMLServer::Deserialize(const char* data, float vertices[], const int vertices_length)
// {
// 	float *q = (float*)data;
// 	for(int i = 0; i < vertices_length; i++)
// 	{
// 		vertices[i] = *q; q++;
// 	}
// }


int main()
{
	const std::string cloud_file_path = "/home/shreyas/Downloads/cloud_data/induvidual_rows/depth_data_test.txt";
	// const int n_points = 2;

	Instrumentor::Get().BeginSession("Profile");
	RosMLServer server(cloud_file_path, 60000);
	server.Start();

	while (1)
	{
		server.Update(-1, true);
	}

	Instrumentor::Get().EndSession();
	return 0;
}