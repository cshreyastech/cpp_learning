#include "ros_ml_server/ros_ml_server.h"

RosMLServer::RosMLServer(const std::string cloud_file_path, const int n_points, uint16_t nPort) 
	: n_points_(n_points), olc::net::server_interface<GameMsg>(nPort)
{
	vertices_length_ = n_points_ * 6;
  vertices_size_ = vertices_length_ * sizeof(float);

	std::ifstream file_handler(cloud_file_path);
	std::string each_value_str;
	// std::string each_value_clean_str;
	float value_float;

	for(int i = 0; i < n_points; i++)
	{
		Point point;

		file_handler >> each_value_str;
		point.Position.v0 = std::stof(each_value_str);

		file_handler >> each_value_str;
		point.Position.v1 = std::stof(each_value_str);

		file_handler >> each_value_str;
		point.Position.v2 = std::stof(each_value_str);


		file_handler >> each_value_str;
		point.Color.v0 = std::stof(each_value_str);

		file_handler >> each_value_str;
		point.Color.v1 = std::stof(each_value_str);

		file_handler >> each_value_str;
		point.Color.v2 = std::stof(each_value_str);

		point_cloud_.points[i] = point;
	}
}

RosMLServer::~RosMLServer()
{
	
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
			desc.n_points = n_points_;
			m_mapPlayerRoster_.insert_or_assign(desc.nUniqueID, desc);

			olc::net::message<GameMsg> msgSendID;
			msgSendID.header.id = GameMsg::Client_AssignID;

			msgSendID << desc.nUniqueID;
			msgSendID << desc.n_points;
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
			// Creating a stack sPlayerDescription to get size of compressed data
			sPlayerDescription desc_from_client;
			msg >> desc_from_client;

			std::ostringstream oss;
			cereal::BinaryOutputArchive archive(oss);
			archive(point_cloud_);

			std::string serializedData = oss.str();

			std::string compressed_data;
			snappy::Compress(serializedData.c_str(), serializedData.size(), &compressed_data);

			const size_t data_size = sizeof(sPlayerDescription) + compressed_data.size();

			sPlayerDescription *desc_to_client = 
				reinterpret_cast<sPlayerDescription*>(new char[sizeof(sPlayerDescription) + sizeof(char) * compressed_data.size() - 1]);

			desc_to_client->point_cloud_compressed_length = compressed_data.size();
			desc_to_client->nUniqueID = desc_from_client.nUniqueID;
			desc_to_client->n_points = n_points_;

			memcpy(&desc_to_client->point_cloud_compressed, 
				compressed_data.c_str(), compressed_data.size());
			WriteMessage(msg, *desc_to_client, data_size);

			MessageAllClients(msg);


			delete[] reinterpret_cast<char*>(desc_to_client);
			

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

void RosMLServer::Deserialize(const char* data, float vertices[], const int vertices_length)
{
	float *q = (float*)data;
	for(int i = 0; i < vertices_length; i++)
	{
		vertices[i] = *q; q++;
	}
}


int main()
{
	const std::string cloud_file_path = "/home/shreyas/Downloads/cloud_data/induvidual_rows/depth_data_0.txt";
	const int n_points = 307200;
	RosMLServer server(cloud_file_path, n_points, 60000);
	
	server.Start();
	while (1)
	{
		server.Update(-1, true);
	}
	return 0;
}