
#include "ros_ml_client/ros_ml_client.h"


RosMLClient::RosMLClient() : olc::GameEngine() , olc::net::client_interface<GameMsg>()
{
	sAppName = "MMO Client";
}

RosMLClient::~RosMLClient()
{

}

bool RosMLClient::OnUserCreate()
{
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
					sPlayerDescription *desc_from_server = new sPlayerDescription();
					desc_from_server = 
					(sPlayerDescription*)malloc(sizeof(sPlayerDescription) + 5);
					ReadMessage(msg, *desc_from_server, 24 + 5);
					mapObjects.insert_or_assign(desc_from_server->nUniqueID, *desc_from_server);

					{
						size_t p_vertices_compressed_length_check;
						p_vertices_compressed_length_check = desc_from_server->p_vertices_compressed_length;
						char p_vertices_compressed_check[p_vertices_compressed_length_check];
						
						memcpy(&p_vertices_compressed_check, desc_from_server->p_vertices_compressed,
							desc_from_server->p_vertices_compressed_length);
						std::string p_vertices_compressed_str_check(p_vertices_compressed_check);

						std::cout << "p_vertices_compressed_length_check: " 
							<< p_vertices_compressed_length_check 
							<< ", p_vertices_compressed_str_check: "
							<< p_vertices_compressed_str_check
							<< std::endl;


						for(int i = 0; i < p_vertices_compressed_length_check; i++)
						{
							std::cout << "i: " << i << ", p_vertices_compressed_check: " 
								<< p_vertices_compressed_check[i] << std::endl;
						}
						std::cout << std::endl;
					}

					delete desc_from_server;

					break;
				}
			}
		}
	}

	if (bWaitingForConnection)
	{
		return true;
	}
	// Get the snappy compressed cloud from server
	// Uncompress it and publish render it in ML
	// size_t p_vertices_compressed_length = mapObjects[nPlayerID].p_vertices_compressed_length;
	

	// char *p_vertices_compressed = (char*)mapObjects[nPlayerID].p_vertices_compressed;
	// std::cout << *p_vertices_ptr << std::endl; **p_vertices_ptr++;
	// std::cout << *p_vertices_ptr << std::endl; **p_vertices_ptr++;
	// std::cout << *p_vertices_compressed << std::endl; p_vertices_compressed++;
	// std::cout << *p_vertices_compressed << std::endl; p_vertices_compressed++;
	// std::cout << *p_vertices_compressed << std::endl;

  // float *q = (float*)data;
  // for(int i = 0; i < vertices_length; i++)
  // {
  //   vertices[i] = *q; q++;
  // }

	// char p_vertices_compressed[p_vertices_compressed_length];
	// memcpy(&p_vertices_compressed, mapObjects[nPlayerID].p_vertices_compressed,
	// 					p_vertices_compressed_length);

	// std::string p_vertices_compressed_str(p_vertices_compressed);

	// std::cout << "p_vertices_compressed_length: " 
	// 	<< p_vertices_compressed_length 
	// 	<< ", p_vertices_compressed_str: "
	// 	<< p_vertices_compressed_str
	// 	<< std::endl;


	// for(int i = 0; i < p_vertices_compressed_length; i++)
	// {
	// 	std::cout << "i: " << i << ", p_vertices_compressed: " 
	// 		<< p_vertices_compressed[i] << std::endl;
	// }
	// std::cout << std::endl;













	// char* p_vertices_compressed = 
	// 				new char[p_vertices_compressed_length];
	// memcpy(p_vertices_compressed, mapObjects[nPlayerID].p_vertices_compressed, 
	// 				p_vertices_compressed_length);

	// const int n_points = 2;
  // const int vertices_length = n_points * 6;
  // const int vertices_size = vertices_length * sizeof(float);

	// char* p_vertices = new char[vertices_size];
	// bool raw_uncompress = 
	// 	snappy::RawUncompress(p_vertices_compressed, p_vertices_compressed_length,
  //                      p_vertices);
	// // std::cout << "raw_uncompress: " << raw_uncompress << std::endl;

	// float* vertices = new float[vertices_length];
	// Deserialize(p_vertices, vertices, vertices_length);

	// std::cout << "vertices[11] = 0.031373f: " << vertices[11] << std::endl;
	// // PublishCloud(vertices);

	// delete[] p_vertices_compressed;
	// delete[] p_vertices;
	// delete[] vertices;




	// Get head and eye pose from ML and send it back to server
	mapObjects[nPlayerID].data_from_ml = 1.001f;


	// Send player description
	olc::net::message<GameMsg> msg;
	msg.header.id = GameMsg::Game_UpdatePlayer;
	msg << mapObjects[nPlayerID];

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

int main()
{
	RosMLClient ros_ml_client;
	if (ros_ml_client.Construct(800, 600))
		ros_ml_client.Start();
	return 0;
}