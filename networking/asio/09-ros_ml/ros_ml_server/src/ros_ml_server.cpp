#include "ros_ml_server/ros_ml_server.h"

RosMLServer::RosMLServer(const std::string cloud_file_path, uint16_t nPort) 
	: olc::net::server_interface<GameMsg>(nPort)
{
	std::ifstream file_handler(cloud_file_path);
	std::string each_value_str;

	for(int i = 0; i < N_POINTS; i++)
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

		to_serilize_point_cloud_.point_cloud[i] = point;
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
		case(GameMsg::Client_Accepted):
		{
			break;
		}

		case(GameMsg::Client_AssignID):
		{
			break;
		}
		case (GameMsg::Client_RegisterWithServer):
		{
			sPlayerDescription desc;
			msg >> desc;
			desc.nUniqueID = client->GetID();
			m_mapPlayerRoster_.insert_or_assign(desc.nUniqueID, desc);

			olc::net::message<GameMsg> msgSendID;
			msgSendID.header.id = GameMsg::Client_AssignID;

			msgSendID << desc.nUniqueID;
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

		case (GameMsg::Client_UnregisterWithServer):
		{
			break;
		}

		case(GameMsg::Game_AddPlayer):
		{
			break;
		}

		case(GameMsg::Game_RemovePlayer):
		{
			break;
		}
		case (GameMsg::Game_UpdatePlayer):
		{	
			
			// Creating a stack sPlayerDescription to get size of compressed data
			sPlayerDescription desc_from_client;
			{
				// Timer timer;
				msg >> desc_from_client;
			}

			std::ostringstream oss;
			{
				Timer timer("Serialize");
				cereal::BinaryOutputArchive archive(oss);
				archive(to_serilize_point_cloud_);
			}

			std::string serializedData = oss.str();

			std::string compressed_data;
			{
				Timer timer("Compress");
				snappy::Compress(serializedData.c_str(), serializedData.size(), &compressed_data);
			}

			const size_t data_size = sizeof(sPlayerDescription) + compressed_data.size();

			std::unique_ptr<sPlayerDescription, void(*)(sPlayerDescription*)> desc_to_client(
				reinterpret_cast<sPlayerDescription*>(new char[data_size]),
				[](sPlayerDescription* ptr) {
					delete[] reinterpret_cast<char*>(ptr); // Properperly deallocate the memory
				}
			);

			{
				// Timer timer("Memcpy");
				desc_to_client->point_cloud_compressed_length = compressed_data.size();
				desc_to_client->nUniqueID = desc_from_client.nUniqueID;
				memcpy(&desc_to_client->point_cloud_compressed,
					compressed_data.c_str(), compressed_data.size());
			}
			
			{
				Timer timer("WriteMessage");
				WriteMessage(msg, *desc_to_client, data_size);
			}

			{
				Timer timer("MessageAllClients");
				MessageAllClients(msg);
			}
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
	const std::string cloud_file_path = "/home/shreyas/Downloads/cloud_data/induvidual_rows/depth_data_300K1-307200.txt";

	RosMLServer server(cloud_file_path, 60000);
	
	server.Start();
	while (1)
	{
		server.Update(-1, true);
	}
	return 0;
}