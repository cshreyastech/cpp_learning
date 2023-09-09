#include <iostream>
#include <unordered_map>

#include "ros_ml_server/ros_ml_common.h"
#include "ros_ml_server/ros_ml_network.h"
#include <stdio.h>
#include <stdlib.h>
#include <string>

class GameServer : public olc::net::server_interface<GameMsg>
{
public:
	GameServer(uint16_t nPort) : olc::net::server_interface<GameMsg>(nPort)
	{
	}

	std::unordered_map<uint32_t, sPlayerDescription> m_mapPlayerRoster;
	std::vector<uint32_t> m_vGarbageIDs;

protected:
	bool OnClientConnect(std::shared_ptr<olc::net::connection<GameMsg>> client) override
	{
		// For now we will allow all 
		return true;
	}

	void OnClientValidated(std::shared_ptr<olc::net::connection<GameMsg>> client) override
	{
		// Client passed validation check, so send them a message informing
		// them they can continue to communicate
		olc::net::message<GameMsg> msg;
		msg.header.id = GameMsg::Client_Accepted;
		client->Send(msg);
	}

	void OnClientDisconnect(std::shared_ptr<olc::net::connection<GameMsg>> client) override
	{
		if (client)
		{
			if (m_mapPlayerRoster.find(client->GetID()) == m_mapPlayerRoster.end())
			{
				// client never added to roster, so just let it disappear
			}
			else
			{
				auto& pd = m_mapPlayerRoster[client->GetID()];
				std::cout << "[UNGRACEFUL REMOVAL]:" + std::to_string(pd.nUniqueID) + "\n";
				m_mapPlayerRoster.erase(client->GetID());
				m_vGarbageIDs.push_back(client->GetID());
			}
		}

	}

	void OnMessage(std::shared_ptr<olc::net::connection<GameMsg>> client, olc::net::message<GameMsg>& msg) override
	{
		if (!m_vGarbageIDs.empty())
		{
			for (auto pid : m_vGarbageIDs)
			{
				olc::net::message<GameMsg> m;
				m.header.id = GameMsg::Game_RemovePlayer;
				m << pid;
				std::cout << "Removing " << pid << "\n";
				MessageAllClients(m);
			}
			m_vGarbageIDs.clear();
		}



		switch (msg.header.id)
		{
			case GameMsg::Client_RegisterWithServer:
			{
				sPlayerDescription desc;
				msg >> desc;
				desc.nUniqueID = client->GetID();
				desc.n_points = 2;
				m_mapPlayerRoster.insert_or_assign(desc.nUniqueID, desc);

				olc::net::message<GameMsg> msgSendID;
				msgSendID.header.id = GameMsg::Client_AssignID;

				msgSendID << desc.nUniqueID;
				msgSendID << desc.n_points;
				MessageClient(client, msgSendID);

				olc::net::message<GameMsg> msgAddPlayer;
				msgAddPlayer.header.id = GameMsg::Game_AddPlayer;
				msgAddPlayer << desc;
				MessageAllClients(msgAddPlayer);

				for (const auto& player : m_mapPlayerRoster)
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

				/*
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
				*/

				sPlayerDescription *desc_to_client = new sPlayerDescription();
				desc_to_client = 
					(sPlayerDescription*)malloc(sizeof(sPlayerDescription));

				// std::cout << "sizeof(uint32_t): " << sizeof(uint32_t) << std::endl;
				// std::cout << "sizeof(float): " << sizeof(float) << std::endl;
				// std::cout << "sizeof(size_t): " << sizeof(size_t) << std::endl;
				
				// std::cout << "sizeof(sPlayerDescription): " << sizeof(sPlayerDescription) << std::endl;
					// (sPlayerDescription*)malloc(sizeof(sPlayerDescription) + p_vertices_compressed_length + 1);



				// desc = 
				// 	(sPlayerDescription*)malloc(sizeof(sPlayerDescription));
				// desc->p_vertices_compressed_length = p_vertices_compressed_length;
				desc_to_client->nUniqueID = desc_from_client.nUniqueID;
				desc_to_client->p_vertices_compressed_length = 110;
				// desc_to_client->n_points = 10;
				// desc.p_vertices_compressed_length = 100;
				// strcpy(desc->p_vertices_compressed, p_vertices_compressed);

				/*
				//// check tbd ////////////////	
				size_t p_vertices_compressed_length_check = desc->p_vertices_compressed_length;

				char* p_vertices_compressed_check = 
					new char[p_vertices_compressed_length_check];
				memcpy(p_vertices_compressed_check, desc->p_vertices_compressed, 
					p_vertices_compressed_length_check);

				std::cout << "p_vertices_compressed_length_check: " << p_vertices_compressed_length_check 
					<< std::endl;
							
				char* p_vertices_check = new char[vertices_size];
				bool raw_uncompress = 
					snappy::RawUncompress(p_vertices_compressed, p_vertices_compressed_length_check,
														p_vertices_check);
				std::cout << "raw_uncompress: " << raw_uncompress << std::endl;

				float* vertices_check = new float[vertices_length];
				Deserialize(p_vertices_check, vertices_check, vertices_length);
				
				delete[] p_vertices_compressed_check;
				delete[] p_vertices_check;
				std::cout << "vertices[11] = 0.031373f: " << vertices[11] << std::endl;
				////////////////
				*/

				desc_from_client.p_vertices_compressed_length = 110;
				sPlayerDescription desc_validate;

				// WriteMessage(msg, desc_from_client);
				
				
				WriteMessage(msg, *desc_to_client);
				printf("ros_ml_server - msg.header.size: %d, msg.size(): %ld\n", msg.header.size, msg.size());
				// msg >> desc_validate;
				// std::cout << "ros_ml_server-desc_validate: " 
				// 	<< desc_validate.p_vertices_compressed_length << std::endl;


				/*
				// Check objects
				sPlayerDescription *desc_ptr = new sPlayerDescription();
				desc_ptr = 
					(sPlayerDescription*)malloc(sizeof(sPlayerDescription));

				delete desc_ptr;
				olc::net::message<GameMsg> msg1;
				olc::net::message<GameMsg> msg2;
				/////// End of Check objects ///////////
				*/

				std::cout << "Before MessageAllClientsV2\n";
				// MessageAllClients(msg);
				MessageAllClientsV2(msg);
				std::cout << "-------After MessageAllClientsV2--------\n";
				// delete[] p_vertices;
				// delete[] p_vertices_compressed;
				delete desc_to_client;
				break;
			}

		}

	}

	void Serialize(const char* data, float vertices[], const int vertices_length)
	{
		float *q = (float*)data;
		for(int i = 0; i < vertices_length; i++)
		{
			*q = vertices[i]; q++;
		}
	}

	void Deserialize(const char* data, float vertices[], const int vertices_length)
	{
		float *q = (float*)data;
		for(int i = 0; i < vertices_length; i++)
		{
			vertices[i] = *q; q++;
		}
	}
};



int main()
{
	GameServer server(60000);
	server.Start();

	while (1)
	{
		server.Update(-1, true);
	}
	return 0;
}