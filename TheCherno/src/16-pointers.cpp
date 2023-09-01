#include<iostream>
#include <cstring>

#define LOG(x) std::cout << x << std::endl;

int main()
{
  int var = 8;
  int* ptr = &var;

  *ptr = 10;
  LOG(var)

  char* buffer = new char[8];
  memset(buffer, 0, 8);

  char** ptr2 = &buffer;

  LOG(buffer)
  LOG(ptr2)
  delete[] buffer;

  return 0;
}