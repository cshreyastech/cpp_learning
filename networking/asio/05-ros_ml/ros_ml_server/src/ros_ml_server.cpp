#include "ros_ml_server/ros_ml_server.h"

RosMLServer::RosMLServer(const std::string cloud_file_path, const int n_points, uint16_t nPort) 
	: n_points_(n_points), olc::net::server_interface<GameMsg>(nPort)
{
	vertices_length_ = n_points_ * 6;
  vertices_size_ = vertices_length_ * sizeof(float);


	// vertices_sa_ = new Vertex[n_points_];
	// ParseCloudFromFile(cloud_file_path, vertices_sa_);


	// assert(vertices_sa_[1].Position.v2 == -0.931193f);
	// assert(vertices_sa_[1].Color.v2 == 0.635294f);

	vertices_ = new float[vertices_length_];

	std::string each_value_str;
  int n_values_read_from_file  = 0;

	std::ifstream file_handler(cloud_file_path);
  while(file_handler >> each_value_str)
  {
    std::string each_value_clean_str = 
      each_value_str.substr(0, each_value_str.find("f", 0));

    float value_float = std::stof(each_value_clean_str);

    vertices_[n_values_read_from_file] = value_float;
    n_values_read_from_file++;
  }
  assert(n_points_ == (n_values_read_from_file)/6);
}

RosMLServer::~RosMLServer()
{
	delete[] vertices_;
	// delete[] vertices_sa_;
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
			// Simply bounce update to everyone except incoming client
			// MessageAllClients(msg, client);
			
			sPlayerDescription desc_from_client;
			msg >> desc_from_client;

			char p_vertices[vertices_size_];

			Serialize(p_vertices, vertices_, vertices_length_);

			// Compress
			char* p_vertices_compressed = 
				new char[snappy::MaxCompressedLength(vertices_size_)];

			size_t p_vertices_compressed_length;

			// auto compression_start = std::chrono::high_resolution_clock::now();
			snappy::RawCompress(p_vertices, vertices_size_, 
				p_vertices_compressed, &p_vertices_compressed_length);


			const size_t data_size = sizeof(sPlayerDescription) + p_vertices_compressed_length;

			sPlayerDescription *desc_to_client = 
				reinterpret_cast<sPlayerDescription*>(new char[sizeof(sPlayerDescription) + sizeof(char) * p_vertices_compressed_length - 1]);


			desc_to_client->p_vertices_compressed_length = p_vertices_compressed_length;
			desc_to_client->nUniqueID = desc_from_client.nUniqueID;
			desc_to_client->n_points = n_points_;

			memcpy(&desc_to_client->p_vertices_compressed, 
				p_vertices_compressed, p_vertices_compressed_length);
			WriteMessage(msg, *desc_to_client, data_size);

			MessageAllClients(msg);


			delete[] p_vertices_compressed;

			delete[] reinterpret_cast<char*>(desc_to_client);
			// delete desc_to_client;
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

// void RosMLServer::ParseCloudFromFile(const std::string cloud_file_path, Vertex vertices[])
// {
//   int n_values_read_from_file  = 0;

// 	std::ifstream file_handler(cloud_file_path);
// 	std::string each_value_str;
// 	// std::string each_value_clean_str;
// 	float value_float;

// 	for(int i = 0; i < n_points_; i++)
// 	{
// 		Vertex v;

// 		file_handler >> each_value_str;
// 		v.Position.v0 = std::stof(each_value_str);

// 		file_handler >> each_value_str;
// 		v.Position.v1 = std::stof(each_value_str);

// 		file_handler >> each_value_str;
// 		v.Position.v2 = std::stof(each_value_str);


// 		file_handler >> each_value_str;
// 		v.Color.v0 = std::stof(each_value_str);

// 		file_handler >> each_value_str;
// 		v.Color.v1 = std::stof(each_value_str);

// 		file_handler >> each_value_str;
// 		v.Color.v2 = std::stof(each_value_str);

// 		vertices_sa_[i] = v;
// 	}
// }

int main()
{
	const std::string cloud_file_path = "/home/shreyas/Downloads/cloud_data/induvidual_rows/depth_data_test.txt";
	const int n_points = 14;
	RosMLServer server(cloud_file_path, n_points, 60000);
	
	server.Start();
	while (1)
	{
		server.Update(-1, true);
	}
	return 0;
}