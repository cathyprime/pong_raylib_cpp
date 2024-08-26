#include <raylib.h>

#include "ball.hh"
#include "vector2.hh"

Ball::Ball(int* player1_score, int* player2_score, int value, const Vector2E<float>& position,
		   const Vector2E<int>& speed)
	: p1_score(std::move(player1_score))
	, p2_score(std::move(player2_score))
	, m_radius(value)
	, m_start_pos(position)
	, m_position(std::move(position))
	, m_speed(std::move(speed)){};

Ball& Ball::update()
{
	m_position.x += m_speed.x;
	m_position.y += m_speed.y;

	if (m_position.y + m_radius >= GetScreenHeight() || m_position.y - m_radius <= 0) m_speed.y *= -1;
	if (m_position.x + m_radius >= GetScreenWidth()) {
		m_position = m_start_pos;
		m_speed = {-7, 7};
		(*p1_score)++;
	}
	if (m_position.x - m_radius <= 0) {
		m_position = m_start_pos;
		m_speed = {7, 7};
		(*p2_score)++;
	}

	return *this;
}

Ball& Ball::draw()
{
	DrawCircleV(m_position, m_radius, WHITE);
	return *this;
}
