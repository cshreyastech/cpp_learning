#include <iostream>
#include <string>


template<typename T, int N>
class Array
{
private:
  T m_Array[N];

public:
  int GetSize() const { return N; }
};

template<typename T>
void Print(T value)
{
  std::cout << "value: " << value << std::endl;
}

int main()
{
  Print<int>(5);
  Print<std::string>("Hello");
  Print<float>(5.1f);

  Array<int, 5> array_int;
  std::cout << array_int.GetSize() << std::endl;

  Array<std::string, 50> array_string;
  std::cout << array_string.GetSize() << std::endl;
  return 0;
}