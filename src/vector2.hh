#pragma once
#include <raylib.h>
#include <type_traits>

template<typename T>
	requires std::is_arithmetic_v<T>
class Vector2E
{
  public:
	T x, y;

	operator Vector2()
	{
		Vector2 vec;
		vec.x = x;
		vec.y = y;
		return vec;
	}
};
