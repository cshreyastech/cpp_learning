#include <iostream>
#include <string>

static uint32_t s_AllocCount = 0;
void* operator new(size_t size)
{
  s_AllocCount++;
  std::cout << "Allocating " << size << " bytes\n";
  return malloc(size);
}

#define STRING_VIEW 1
#if STRING_VIEW
static void PrintName(std::string_view name)
{
  std::cout << name << std::endl;
}
#else
static void PrintName(const std::string& name)
{
  std::cout << name << std::endl;
}
#endif
int main()
{
  const char* name = "Yan Chernikov";
  
#if 0
  std::string firstName = name.substr(0, 3);
  std::string lastName = name.substr(4, 9);
  PrintName(name);
#else
  std::string_view first_name(name, 3);
  std::string_view last_name(name + 4, 9);
  PrintName(name);
#endif

  

  std::cout << s_AllocCount << " allocations" << std::endl;
  return 0;
}