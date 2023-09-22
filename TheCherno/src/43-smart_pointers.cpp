#include <iostream>
#include <string>
#include <memory>

class Entity
{
public:
  Entity()
  {
    std::cout << "Created Entity!" << std::endl;
  }

  ~Entity()
  {
    std::cout << "Destroyed Entity!" << std::endl;
  }

  void Print() {}
};
int main()
{
  {
    std::unique_ptr<Entity> entity(new Entity());
    entity->Print();
  }

  {
    // Automated exception hadling. better to use this.
    std::unique_ptr<Entity> entity = std::make_unique<Entity>();
    entity->Print();
  }

  {
    std::shared_ptr<Entity> e0;
    {
      std::shared_ptr<Entity> sharedEntity = std::make_shared<Entity>();
      std::weak_ptr<Entity> weakEntity = sharedEntity;
      e0 = sharedEntity;
    }
  }

  {
    std::weak_ptr<Entity> e0;
    {
      std::shared_ptr<Entity> sharedEntity = std::make_shared<Entity>();
      std::weak_ptr<Entity> weakEntity = sharedEntity;
      e0 = sharedEntity;
    }
  }
  return 0;
}