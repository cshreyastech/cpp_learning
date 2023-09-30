#include "ros_ml_server/ros_ml_server.h"
#include<cassert>

RosMLServer::RosMLServer(const std::string cloud_file_path, const int n_points, uint16_t nPort) 
	: n_points_(n_points), olc::net::server_interface<GameMsg>(nPort)
{
	vertices_length_ = n_points_ * 6;
  vertices_size_ = vertices_length_ * sizeof(float);
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
			// Get new set of vector from ROS after integration

			sPlayerDescription desc;
			msg >> desc;

			std::string each_value_str;
			int n_values_read_from_file  = 0;

			std::ifstream file_handler(cloud_file_path_);
			while(file_handler >> each_value_str)
			{
				std::string each_value_clean_str = 
					each_value_str.substr(0, each_value_str.find("f", 0));

				float value_float = std::stof(each_value_clean_str);

				desc.vertices[n_values_read_from_file] = value_float;
				n_values_read_from_file++;
			}
			assert(n_points_ == (n_values_read_from_file)/6);



			msg << desc;
			MessageAllClients(msg);

			// /// Validate
			// sPlayerDescription desc_test;
			// msg >> desc_test;
			// std::cout << "vertices[10] = 0.133333f: " << desc_test.vertices[10] << std::endl;
			// // assert(desc_test.vertices[11] == 0.031373f);

			// ////

			
			break;
		}

	}

}


int main()
{
	const std::string cloud_file_path = "/home/shreyas/Downloads/cloud_data/induvidual_rows/depth_data_100K1-200K.txt";
	const int n_points = 100000;

	Instrumentor::Get().BeginSession("Profile");
	RosMLServer server(cloud_file_path, n_points, 60000);
	server.Start();

	while (1)
	{
		server.Update(-1, true);
	}

	Instrumentor::Get().EndSession();
	return 0;
}