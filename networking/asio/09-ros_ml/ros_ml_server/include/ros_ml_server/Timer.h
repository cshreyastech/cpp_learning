#ifndef TIMER_H
#define TIMER_H

#include <iostream>
#include <chrono>

// RAII example
class Timer
{
public:
  Timer(std::string caption)
    : caption_{caption}
  {
    m_StartTimePoint = std::chrono::high_resolution_clock::now();
  }

  ~Timer()
  {
    Stop();
  }

  void Stop()
  {
    auto endTimePoint = std::chrono::high_resolution_clock::now();

    auto start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimePoint).time_since_epoch().count();
    auto end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimePoint).time_since_epoch().count();

    auto duration = end - start;
    double ms = duration * 0.001;

    std::cout << caption_ << ": "<< duration << "us (" << ms << "ms)\n";
  }
private:
  std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTimePoint;
  std::string caption_{""};
};

#endif