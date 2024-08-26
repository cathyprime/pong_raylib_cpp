#include <raylib.h>
#include <utility>

#include "paddle.hh"

Paddle::Paddle(int speed, const Vector2E<int>& keys, const Vector2E<float>& size, const Vector2E<float>& position)
	: m_keys(std::move(keys))
	, speed(speed)
	, m_size(std::move(size))
	, position(std::move(position))
{}

Paddle& Paddle::draw()
{
	DrawRectangleV(position, m_size, WHITE);
	return *this;
}

Paddle& Paddle::update()
{
	if (IsKeyDown(m_keys.x))
		position.y -= speed;

	if (IsKeyDown(m_keys.y))
		position.y += speed;

	if (position.y <= 0)
		position.y = 0;

	if (position.y + m_size.y >= GetScreenHeight())
		position.y = GetScreenHeight() - m_size.y;

	return *this;
}

Paddle::operator Rectangle()
{
	return Rectangle{position.x, position.y, m_size.x, m_size.y};
}
