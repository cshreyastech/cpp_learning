#include "ros_ml_server/ros_ml_server.h"

RosMLServer::RosMLServer(uint16_t nPort) : olc::net::server_interface<GameMsg>(nPort)
{

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
			desc.n_points = 2;
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

			
			const int n_points = 2;

			const int vertices_length = n_points * 6;
			const int vertices_size = vertices_length * sizeof(float);

			float vertices[vertices_length] = 
			{
				0.024249f, 0.034523f, -0.085764f, 1.000000f, 0.141176f, 0.031373f, 
				0.024413f, 0.034542f, -0.085810f, 1.000000f, 0.133333f, 0.031373f, 
			};
			const char* p_vertices = (const char*)malloc(vertices_size);
			Serialize(p_vertices, vertices, vertices_length);

			// Compress
			char* p_vertices_compressed = 
				new char[snappy::MaxCompressedLength(vertices_size)];
			size_t p_vertices_compressed_length;

			// auto compression_start = std::chrono::high_resolution_clock::now();
			snappy::RawCompress(p_vertices, vertices_size, 
				p_vertices_compressed, &p_vertices_compressed_length);
			


			// char p_vertices_compressed[]  = { 'a', 'b', 'c', 'd', 'e' };
			// size_t p_vertices_compressed_length = sizeof(p_vertices_compressed);

			const size_t data_size = sizeof(sPlayerDescription) + p_vertices_compressed_length;
			sPlayerDescription *desc_to_client = new sPlayerDescription();
			desc_to_client = (sPlayerDescription*)malloc(data_size);

			desc_to_client->p_vertices_compressed_length = p_vertices_compressed_length;
			desc_to_client->nUniqueID = desc_from_client.nUniqueID;
			memcpy(&desc_to_client->p_vertices_compressed, 
				p_vertices_compressed, p_vertices_compressed_length);
			WriteMessage(msg, *desc_to_client, data_size);
			MessageAllClients(msg);


			delete[] p_vertices;
			delete[] p_vertices_compressed;
			delete desc_to_client;
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
	RosMLServer server(60000);
	server.Start();

	while (1)
	{
		server.Update(-1, true);
	}
	return 0;
}