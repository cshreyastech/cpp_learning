#include "ros_ml_server/ros_ml_server.h"

RosMLServer::RosMLServer(const std::string cloud_file_path, uint16_t nPort) 
	: olc::net::server_interface<GameMsg>(nPort)
{
	cloud_file_path_ = cloud_file_path;
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
			ToSerilizePointCloud to_serilize_point_cloud;
			{
			// 	Timer timer("Read Cloud from file");

				std::ifstream file_handler(cloud_file_path_);
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

					to_serilize_point_cloud.point_cloud[i] = point;
				}
			}


      cv::Mat left_endoscope_image_mat = cv::imread("../external/endo_images/task_hard_left_endoscope_video.png", 
        cv::IMREAD_COLOR);

      cv::Mat right_endoscope_image_mat = cv::imread("../external/endo_images/task_hard_right_endoscope_video.png", 
        cv::IMREAD_COLOR);

      const int left_endoscope_image_mat_rows = left_endoscope_image_mat.rows;
      const int left_endoscope_image_mat_cols = left_endoscope_image_mat.cols;

      const int right_endoscope_image_mat_rows = right_endoscope_image_mat.rows;
      const int right_endoscope_image_mat_cols = right_endoscope_image_mat.cols;

      // printf("left_endoscope_image_mat(%d, %d)\n", left_endoscope_image_mat_rows, left_endoscope_image_mat_cols);
      // printf("right_endoscope_image_mat(%d, %d)\n", right_endoscope_image_mat_rows, right_endoscope_image_mat_cols);


      StructMat data_to_serialize_struct_mat;


      memcpy(data_to_serialize_struct_mat.left_image_array, 
      left_endoscope_image_mat.data, sizeof(left_endoscope_image_mat));

      memcpy(data_to_serialize_struct_mat.right_image_array, 
      right_endoscope_image_mat.data, sizeof(right_endoscope_image_mat));


			// Creating a stack sPlayerDescription to get size of compressed data
			sPlayerDescription desc_from_client;
			{
				// Timer timer;
				msg >> desc_from_client;
				// std::cout << "desc_from_client.pose_from_client: " << desc_from_client.pose_from_client << std::endl;
		
				T_ML_ROS t_ml_ros = desc_from_client.t_ml_ros;

				printf("Q_EC_HEAD(%f, %f, %f, %f), P_EC_HEAD(%f, %f, %f)\n", 
					t_ml_ros.Q_EC_HEAD.x, t_ml_ros.Q_EC_HEAD.y, t_ml_ros.Q_EC_HEAD.z, t_ml_ros.Q_EC_HEAD.w,
					t_ml_ros.P_EC_HEAD.x, t_ml_ros.P_EC_HEAD.y, t_ml_ros.P_EC_HEAD.z
				);

				printf("Q_EC_FIX(%f, %f, %f, %f), P_EC_FIX(%f, %f, %f)\n", 
					t_ml_ros.Q_EC_FIX.x, t_ml_ros.Q_EC_FIX.y, t_ml_ros.Q_EC_FIX.z, t_ml_ros.Q_EC_FIX.w,
					t_ml_ros.P_EC_FIX.x, t_ml_ros.P_EC_FIX.y, t_ml_ros.P_EC_FIX.z
				);

			}


			std::ostringstream oss;
			{
				// Timer timer("Serialize");
				cereal::BinaryOutputArchive archive(oss);
				// archive(to_serilize_point_cloud);
        archive(data_to_serialize_struct_mat);
			}

			std::string serializedData = oss.str();

			std::string compressed_data;
			{
			// 	Timer timer("Compress");
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
				// Timer timer("WriteMessage");
				WriteMessage(msg, *desc_to_client, data_size);
			}

			{
				// Timer timer("MessageAllClients");
				MessageAllClients(msg);
			}

			break;
		}
	}
}


int main()
{
	const std::string cloud_file_path = "/home/shreyas/Downloads/cloud_data/induvidual_rows/depth_data_1-100K.txt";

	RosMLServer server(cloud_file_path, 60000);
	
	server.Start();
	while (1)
	{
		server.Update(-1, true);
	}
	return 0;
}