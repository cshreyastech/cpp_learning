#include<iostream>
#include<string>

class Entity
{
public:
  Entity()
  {
    std::cout << "Created Entity\n";
  }

  ~Entity()
  {
    std::cout << "Destroyed Entity\n";
  }
};

// Unique pointer example
class ScopedPtr
{
private:
  Entity* m_Ptr;
public:
  ScopedPtr(Entity* ptr)
    : m_Ptr(ptr)
    {
    }

  ~ScopedPtr()
  {
    delete m_Ptr;
  }
};

int main()
{
  {
    Entity e;
  }

  {
    Entity* e = new Entity();

    delete e;
  }

  {
    // Will get destroyed once it goes out of scope
    ScopedPtr e = new Entity();
    
    // Same as above
    // ScopedPtr e(new Entity());
  }

  return 0;
}