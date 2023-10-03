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
    const int vertices_length = n_points * 6;
		const int vertices_size = vertices_length * sizeof(float);

    // assert(vertices[vertices_length - 1] == 0.635294f);

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

  void GameEngine::PublishCloud(const PointCloud point_cloud, const int n_points)
  {
    assert((point_cloud.points[13].Color.v0) == 0.635294f);
  }

} // namespace olc
