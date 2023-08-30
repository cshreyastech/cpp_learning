#include <iostream>
#include <thread>

static bool s_Finished = false;

void DoWork()
{
  using namespace std::literals::chrono_literals;
  std::cout << "Started thread id in DoWork= " << std::this_thread::get_id() << std::endl;
  while(!s_Finished)
  {
    std::cout << "Working.. \n";
    std::this_thread::sleep_for(1s);
  }
}

int main()
{
  std::thread worker(DoWork);
  std::cin.get();
  s_Finished = true;
  std::cout << "Started thread id before join = " << std::this_thread::get_id() << std::endl;
  // Wait for thread to join. Wait for the thread to complete its work.
  // Block the current thread until worker thread has completed.
  worker.join();
  std::cout << "Started thread id after join = " << std::this_thread::get_id() << std::endl;
  std::cout << "Completed.. \n";

  return 0;
}