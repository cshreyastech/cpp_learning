#include <iostream>
#include <string>

struct Vector2
{
  float x, y;

  Vector2(float x, float y)
    : x(x), y(y) {}

  Vector2 Add(const Vector2& other) const
  {
    return Vector2(x + other.x, y + other.y);
  }

  Vector2 operator+(const Vector2& other) const
  {
    return Add(other);
  }

  Vector2 Multiply(const Vector2& other) const
  {
    return Vector2(x * other.x, y * other.y);
  }

  Vector2 operator*(const Vector2& other) const
  {
    return Multiply(other);
  }

  bool operator==(const Vector2& other) const
  {
    return x == other.x && y == other.y;
  }

  bool operator!=(const Vector2& other) const
  {
    return !(*this == other);
    // return !operator==(other);
  }
};

struct Vector2_v2
{
  float x, y;

  Vector2_v2(float x, float y)
    : x(x), y(y) {}

  Vector2_v2 Add(const Vector2_v2& other) const
  {
    return operator+(other);
  }

  Vector2_v2 operator+(const Vector2_v2& other) const
  {
    return Vector2_v2(x + other.x, y + other.y);
  }

};

std::ostream& operator<<(std::ostream& stream, const Vector2& other)
{
  stream << other.x << ", " << other.y;

  return stream;
}

int main()
{
  Vector2 position(4.0f, 4.0f );
  Vector2 speed(0.5f, 1.5f);
  Vector2 powerup(1.1f, 1.1f);

  Vector2 result1 = position.Add(speed.Multiply(powerup));
  Vector2 result2 = position + speed * powerup;

  std::cout << result1.x << result1.y << std::endl;
  std::cout << result2.x << result2.y << std::endl;

  std::cout << result2 << std::endl;
  
  Vector2_v2 position_v2(4.0f, 4.0f );
  Vector2_v2 speed_v2(0.5f, 1.5f);

  Vector2_v2 result2_v2 = position_v2 + speed_v2;

  std::cout << result2_v2.x << result2_v2.y << std::endl;
  return 0;
}