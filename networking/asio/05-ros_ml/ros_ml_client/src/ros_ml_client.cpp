
#include "ros_ml_client/ros_ml_client.h"


RosMLClient::RosMLClient() : olc::GameEngine() , olc::net::client_interface<GameMsg>()
{
	sAppName = "MMO Client";
}

bool RosMLClient::OnUserCreate()
{
	std::cout << "Inside RosMLClient::OnUserCreate()\n";

	mapObjects[0].nUniqueID = 0;
	//mapObjects[0].vPos = { 3.0f, 3.0f };
	mapObjects[0].n_points = 0;
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
					// descPlayer.vPos = { 3.0f, 3.0f };
					descPlayer.n_points = 0;
					msg << descPlayer;
					Send(msg);
					break;
				}

				case(GameMsg::Client_AssignID):
				{
					// Server is assigning us OUR id
					uint32_t n_points;
					msg >> n_points;
					msg >> nPlayerID;

					std::cout << "Assigned Client ID = " << nPlayerID << "\n";
					std::cout << "n_points = " << n_points << "\n";
					break;
				}

				case(GameMsg::Game_AddPlayer):
				{
					sPlayerDescription desc;
					msg >> desc;
					mapObjects.insert_or_assign(desc.nUniqueID, desc);

					if (desc.nUniqueID == nPlayerID)
					{
						// Now we exist in game world
						bWaitingForConnection = false;
					}
					break;
				}

				case(GameMsg::Game_RemovePlayer):
				{
					uint32_t nRemovalID = 0;
					msg >> nRemovalID;
					mapObjects.erase(nRemovalID);
					break;
				}

				case(GameMsg::Game_UpdatePlayer):
				{
					sPlayerDescription desc;
					msg >> desc;
					mapObjects.insert_or_assign(desc.nUniqueID, desc);
					break;
				}
			}
		}
	}

	if (bWaitingForConnection)
	{
		return true;
	}
	mapObjects[nPlayerID].data_from_ml = 1.001f;

	// std::cout << "vertices[10] = 0.133333f: " << mapObjects[nPlayerID].vertices[10] << std::endl;
	PublishCloud(mapObjects[nPlayerID].vertices);

	// Send player description
	olc::net::message<GameMsg> msg;
	msg.header.id = GameMsg::Game_UpdatePlayer;
	msg << mapObjects[nPlayerID];

	Send(msg);
	return true;
}

int main()
{
	RosMLClient ros_ml_client;
	if (ros_ml_client.Construct(800, 600))
		ros_ml_client.Start();
	return 0;
}