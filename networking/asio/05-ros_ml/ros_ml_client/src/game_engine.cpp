#include "ros_ml_client/game_engine.h"
namespace olc
{
  GameEngine::GameEngine()
  {

  }

  GameEngine::~GameEngine()
  {

  }

  olc::rcode GameEngine::Construct(int32_t screen_w, int32_t screen_h)
  {
  // glfw: initialize and configure
  // ------------------------------
  // glfwInit();
  // glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  // glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // #ifdef __APPLE__
  //   glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  // #endif

  // // glfw window creation
  // // --------------------
  // window_ = glfwCreateWindow(screen_w, screen_h, "LearnOpenGL", NULL, NULL);
  // if (window_ == NULL)
  // {
  //   std::cout << "Failed to create GLFW window" << std::endl;
  //   glfwTerminate();
  //   return olc::rcode::FAIL;
  // }
  // glfwMakeContextCurrent(window_);
  // // glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  // // glad: load all OpenGL function pointers
  // // ---------------------------------------
  // if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  // {
  //   std::cout << "Failed to initialize GLAD" << std::endl;
  //   return olc::rcode::FAIL;
  // }

    return olc::rcode::OK;
  }

  bool GameEngine::OnUserCreate()
  {
    return false;
  }

  bool GameEngine::OnUserUpdate(float fElaspedTime)
  {
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

  olc::rcode GameEngine::PublishCloud(float vertices[])
  {
    std::cout << "Inside GameEngine::PublishCloud()\n";
    std::cout << "vertices[10] = 0.133333f: " << vertices[10] << std::endl;

    return olc::rcode::OK;
  }

} // namespace olc
