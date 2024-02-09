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
    pose = 3.1414f;

    t_ml_ros = 
      {
        // Head
        // x, y, z, w
        glm::quat(0.029367f, 0.273833f, -0.011948f, 0.961255f),
        glm::vec3(.215041f, -0.124162f, 0.427739f),
      
        // Fix
        glm::quat(-0.014464f, 0.275458f, 0.000942f, 0.961204f),
        glm::vec3(-0.081950f, -0.071199f, -0.264387f)
      };
  }

  void GameEngine::PublishImage(unsigned char left_image_array[][448], unsigned char right_image_array[336][448])
  {
    pose = 3.1414f;

    t_ml_ros = 
      {
        // Head
        // x, y, z, w
        glm::quat(0.029367f, 0.273833f, -0.011948f, 0.961255f),
        glm::vec3(.215041f, -0.124162f, 0.427739f),
      
        // Fix
        glm::quat(-0.014464f, 0.275458f, 0.000942f, 0.961204f),
        glm::vec3(-0.081950f, -0.071199f, -0.264387f)
      };
  }
} // namespace olc
