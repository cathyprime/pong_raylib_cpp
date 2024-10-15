#pragma once
#include <raylib.h>

#include "vector2.hh"

class Ball
{
  private:
	int m_radius;
	Vector2E<float> m_start_pos;
	Vector2E<float> m_position;
	Vector2E<int> m_speed;
	int *p1_score, *p2_score;

  public:
	~Ball() = default;
	Ball(int* player1_score, int* player2_score, int radius, const Vector2E<float>& position, const Vector2E<int>& speed);

	Ball(Ball&&) = delete;
	Ball(const Ball&) = delete;
	Ball& operator=(const Ball& rhs) = delete;
	Vector2E<int>& speed() { return m_speed; };
	Vector2E<float>& position() { return m_position; };
	int& radius() { return m_radius; };

	Ball& update();
	Ball& draw();
};
