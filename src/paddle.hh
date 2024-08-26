#include <raylib.h>

#include "vector2.hh"

class Paddle
{
  private:
	int speed;
	Vector2E<int> m_keys;
	Vector2E<float> m_size;
	Vector2E<float> position;

  public:
	Paddle(int speed, const Vector2E<int>& keys, const Vector2E<float>& m_size, const Vector2E<float>& m_position);
	Paddle& draw();
	Paddle& update();

	operator Rectangle();
};
