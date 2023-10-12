#include "ros_ml_client/game_engine.h"

namespace olc
{
  GameEngine::GameEngine()
  {

  }

  GameEngine::~GameEngine()
  {
    std::cout << "Inside ~GameEngine()\n";
  }

  olc::rcode GameEngine::Construct(int32_t screen_w, int32_t screen_h)
  {
  // glfw: initialize and configure
  // ------------------------------


    return olc::rcode::OK;
  }

  bool GameEngine::OnUserCreate()
  {
    return false;
  }

  bool GameEngine::OnUserUpdate(float fElaspedTime)
  {
    return false;
  }

  bool GameEngine::OnUserDestroy()
	{
		return true;
	}

  olc::rcode GameEngine::Start()
  {
    OnUserCreate();

    while(true)
      OnUserUpdate(0.0f);
    
    return olc::rcode::OK;
  }

  void GameEngine::PublishCloud(const Point point_cloud[])
  {
    // assert((point_cloud[n_points - 1].Color.v0) == 0.619608f);
    // std::cout << "point_cloud[N_POINTS - 1].Color.v0: " << point_cloud[N_POINTS - 1].Color.v0 
    //   << std::endl;

    // Get Fixation and Head pose from Magic leap and publish it back to ROS
    // t_ml_ros.Q_EC_HEAD = 
		// 					glm::quat(1.1f, 1.2f, 1.2f, 1.3f);
    pose = 3.1414f;

    t_ml_ros = 
      {
    //     // Head
        glm::quat(1.1f, 1.2f, 1.2f, 1.3f),
        glm::vec3(1.11f, 1.22f, 1.33f),
      
        // Fix
        glm::quat(2.1f, 2.2f, 2.2f, 2.3f),
        glm::vec3(2.11f, 2.22f, 2.33f)
      };
  }

} // namespace olc
