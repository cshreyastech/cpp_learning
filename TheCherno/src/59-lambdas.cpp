// https://en.cppreference.com/w/cpp/language/lambda

#include<iostream>
#include <vector>
#include <functional>
#include <algorithm> // std::find_if

void ForEach(const std::vector<int>& values, void(*func)(int))
{
  for(int value : values)
    func(value);
}

void ForEach2(const std::vector<int>& values, const std::function<void(int)>& func)
{
  for(int value : values)
    func(value);
}

int main()
{
  std::vector<int> values = { 1, 2, 3, 4, 5 };

  // [] -> capture
  ForEach(values, [](int value) 
  {
    std::cout << "Value: " << value << std::endl; 
  });

  auto lambda = [](int value) 
  {
    std::cout << "Value: " << value << std::endl; 
  };
  ForEach(values, lambda);  

  int a = 5;
  auto lambda2 = [=](int value) { std::cout << "Value: " << a << std::endl; };
  ForEach2(values, lambda2);

  auto lambda3 = [=](int value) mutable { a = 10; std::cout << "Value: " << a << std::endl; };
  ForEach2(values, lambda3);

  auto it = std::find_if(values.begin(), values.end(), [](int value) { return value > 3; });
  std::cout << *it << std::endl;
  
  return 0;
}