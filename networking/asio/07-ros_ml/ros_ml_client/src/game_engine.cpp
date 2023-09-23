#include "ros_ml_client/game_engine.h"
namespace olc
{
  GameEngine::GameEngine()
  {

  }

  GameEngine::~GameEngine()
  {
    std::cout << "Inside ~GameEngine()\n";
    // delete[] vertices;
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
    // const int vertices_length = n_points * 6;
		// const int vertices_size = vertices_length * sizeof(float);

    // assert(vertices[vertices_length - 1] == 0.619608f);

    
    // std::cout << "GameEngine::OnUserUpdate() - validated:" << std::endl;

    // assert(vertices[vertices_length - 1] == 0.619608f);
    // while (!glfwWindowShouldClose(window_))
    // {

    // }

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

  void GameEngine::PublishCoud(float vertices[])
  {
    // const int vertices_length = n_points * 6;
    // std::cout << 
    //   "GameEngine::PublishCoud - vertices[vertices_length - 1] 0.635294f:" << 
    //   vertices[vertices_length - 1] << 
    //   std::endl;

  }
} // namespace olc
