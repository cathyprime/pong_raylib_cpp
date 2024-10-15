#include <raylib.h>

#include "ball.hh"
#include "paddle.hh"
#include "vector2.hh"

void initializer(int *width, int *height, int *font_size, int *radius, Vector2E<int>* speed, Vector2E<int>* paddle_size)
{
	InitWindow(0, 0, "Pong");
	*width = GetScreenWidth();
	*height = GetScreenHeight();
	*font_size = *height / 54;
	*radius = *width / 54;

	*speed = {
	  *width / 274,
	  *width / 274
	};

	*paddle_size = {
	  *width / 77,
	  *height / 9
	};
	
	ToggleFullscreen();
	SetTargetFPS(60);
}

int main()
{
	int player1_score = 0;
	int player2_score = 0;
	
	int width, height, font_size, radius;
 
	Vector2E<int> speed, paddle_size;
		
    initializer(&width, &height, &font_size, &radius, &speed, &paddle_size);

	Ball ball{&player1_score, &player2_score, radius, {width / 2.f, height / 2.f}, speed};
	Paddle player1{height / 180, {KEY_W, KEY_S}, paddle_size, {10, static_cast<int>(height / 2.f - 60)}};
	Paddle player2{height / 180, {KEY_UP, KEY_DOWN}, paddle_size, {width - 35, static_cast<int>(height / 2.f - 60)}};
	
	while (!WindowShouldClose()) {
		BeginDrawing();
		{
			ClearBackground(BLACK);

			DrawLine(width / 2, 0, width / 2, height, WHITE);
			ball.update().draw();
			player1.update().draw();
			player2.update().draw();

			if (CheckCollisionCircleRec(ball.position(), ball.radius(), player1))
				ball.speed().x *= -1;

			if (CheckCollisionCircleRec(ball.position(), ball.radius(), player2))
				ball.speed().x *= -1;

			DrawText(TextFormat("%i", player1_score), width/4-20, font_size, 80, WHITE);
			DrawText(TextFormat("%i", player2_score), 3*width/4-20, font_size, 80, WHITE);
		}
		EndDrawing();
	}

	CloseWindow();
}
