
#include "ros_ml_client/ros_ml_client.h"


RosMLClient::RosMLClient() : olc::GameEngine() , olc::net::client_interface<GameMsg>()
{
	sAppName = "MMO Client";
}

RosMLClient::~RosMLClient()
{
	delete[] vertices;
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

					const size_t data_size = sizeof(sPlayerDescription) + p_vertices_compressed_length;
					sPlayerDescription *desc_from_server = new sPlayerDescription();
					desc_from_server = (sPlayerDescription*)malloc(data_size);

					ReadMessage(msg, *desc_from_server, data_size);
					
					desc_from_server->cloud_set_for_client = true;
					mapObjects_.insert_or_assign(desc_from_server->nUniqueID, *desc_from_server);
	

					
					const int n_points = desc_from_server->n_points;
					const int vertices_length = n_points * 6;
					const int vertices_size = vertices_length * sizeof(float);

					// size_t p_vertices_compressed_length = desc_from_server->p_vertices_compressed_length;

					char* p_vertices_compressed = 
						new char[p_vertices_compressed_length];
					memcpy(p_vertices_compressed, desc_from_server->p_vertices_compressed, 
						p_vertices_compressed_length);
								
					char* p_vertices = new char[vertices_size];
					bool raw_uncompress = 
						snappy::RawUncompress(p_vertices_compressed, p_vertices_compressed_length,
															p_vertices);

					vertices = new float[vertices_length];
					Deserialize(p_vertices, vertices, vertices_length);
					
					delete[] p_vertices_compressed;

					
					// assert(vertices[vertices_length - 1] == 0.619608f);
					
				




					// {
					// 					//// check tbd ////////////////	
					// 	const int n_points = desc_from_server->n_points;
  
  				// 	const int vertices_length = n_points * 6;
  				// 	const int vertices_size = vertices_length * sizeof(float);

					// 	size_t p_vertices_compressed_length = desc_from_server->p_vertices_compressed_length;

					// 	char* p_vertices_compressed = 
					// 		new char[p_vertices_compressed_length];
					// 	memcpy(p_vertices_compressed, desc_from_server->p_vertices_compressed, 
					// 		p_vertices_compressed_length);

					// 	std::cout << "p_vertices_compressed_length: " << p_vertices_compressed_length 
					// 		<< std::endl;
									
					// 	char* p_vertices = new char[vertices_size];
					// 	bool raw_uncompress = 
					// 		snappy::RawUncompress(p_vertices_compressed, p_vertices_compressed_length,
					// 											p_vertices);
					// 	std::cout << "raw_uncompress: " << raw_uncompress << std::endl;

					// 	float* vertices = new float[vertices_length];
					// 	Deserialize(p_vertices, vertices, vertices_length);
						
					// 	delete[] p_vertices_compressed;
					// 	delete[] p_vertices;
						
					// 	assert(vertices[vertices_length - 1] == 0.031373f);
					// 	delete[] vertices;
					// 	////////////////
					// }

					delete desc_from_server;

					break;
				}
			}
		}
	}

	if (bWaitingForConnection_)
	{
		return true;
	}
	// Get the snappy compressed cloud from server
	// Uncompress it and publish render it in ML
	// size_t p_vertices_compressed_length = mapObjects_[nPlayerID_].p_vertices_compressed_length;
	
	// char *p_vertices_compressed = (char*)mapObjects_[nPlayerID_].p_vertices_compressed;
	// std::cout << "static n_points_: " << RosMLClient::n_points_ << std::endl;

	// {
	// 					//// check tbd ////////////////	
	// 	const int n_points =  mapObjects_[nPlayerID_].n_points;

	// 	const int vertices_length = n_points * 6;
	// 	const int vertices_size = vertices_length * sizeof(float);

	// 	size_t p_vertices_compressed_length = mapObjects_[nPlayerID_].p_vertices_compressed_length;

		
	// 	// char* p_vertices_compressed = 
	// 	// 	new char[p_vertices_compressed_length];
	// 	// memcpy(p_vertices_compressed, desc_from_server->p_vertices_compressed, 
	// 	// 	p_vertices_compressed_length);

	// 	// std::cout << "p_vertices_compressed_length: " << p_vertices_compressed_length 
	// 	// 	<< std::endl;
					
	// 	// char* p_vertices = new char[vertices_size];
	// 	// bool raw_uncompress = 
	// 	// 	snappy::RawUncompress(p_vertices_compressed, p_vertices_compressed_length,
	// 	// 										p_vertices);
	// 	// std::cout << "raw_uncompress: " << raw_uncompress << std::endl;

	// 	// float* vertices = new float[vertices_length];
	// 	// Deserialize(p_vertices, vertices, vertices_length);
		
	// 	// delete[] p_vertices_compressed;
	// 	// delete[] p_vertices;
		
	// 	// assert(vertices[vertices_length - 1] == 0.031373f);
	// 	// delete[] vertices;
	// 	////////////////
	// }

	if(mapObjects_[nPlayerID_].cloud_set_for_client)
	{
		const int n_points =  mapObjects_[nPlayerID_].n_points;
		const int vertices_length = n_points * 6;
		const int vertices_size = vertices_length * sizeof(float);

		assert(vertices[vertices_length - 1] == 0.619608f);
		// assert(vertices[11] == 0.031373f);

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

int main()
{
	RosMLClient ros_ml_client;
	if (ros_ml_client.Construct(800, 600))
		ros_ml_client.Start();
	return 0;
}