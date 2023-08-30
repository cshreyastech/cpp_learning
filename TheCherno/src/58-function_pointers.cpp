#include<iostream>
#include <vector>


void PrintValue(int value)
{
  std::cout << "Value: " << value << std::endl;
}

void ForEach(const std::vector<int>& values, void(*func)(int))
{
  for(int value : values)
    func(value);
}


void HelloWorld1()
{
  std::cout << "Hello World1" << std::endl;
}

void HelloWorld2(int id)
{
  std::cout << "Hello World2" << id << std::endl;
}

int main()
{
  auto helloword1_1 = &HelloWorld1;
  helloword1_1();

  void (*helloword1_2)() = HelloWorld1;
  helloword1_2(); 

  void(*helloworld1_3)();
  helloworld1_3 = HelloWorld1;
  helloworld1_3();

  typedef void(*Helloword1Function)();
  Helloword1Function helloworld1_4 = HelloWorld1;
  helloworld1_4();

  typedef void(*Helloword2Function)(int);
  Helloword2Function helloword2_1 = HelloWorld2;
  helloword2_1(8);

  std::vector<int> values = { 1, 2, 3, 4, 5 };
  ForEach(values, PrintValue);

  // [] -> capture
  ForEach(values, [](int value) 
  {
    std::cout << "Value: " << value << std::endl; 
  });

  return 0;
}