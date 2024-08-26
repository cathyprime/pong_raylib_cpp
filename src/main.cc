#include <raylib.h>

#include "ball.hh"
#include "paddle.hh"

const int width = 1920;
const int height = 1080;

int main()
{
	int player1_score = 0;
	int player2_score = 0;

	InitWindow(width, height, "Hello, World!");
	ToggleFullscreen();
	SetTargetFPS(60);

	Ball ball{&player1_score, &player2_score, 20, {width / 2.f, height / 2.f}, {7, 7}};
	Paddle player1{6, {KEY_W, KEY_S}, {25, 120}, {10, height / 2.f - 60}};
	Paddle player2{6, {KEY_UP, KEY_DOWN}, {25, 120}, {width - 35, height / 2.f - 60}};

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

			DrawText(TextFormat("%i", player1_score), width/4-20, 20, 80, WHITE);
			DrawText(TextFormat("%i", player2_score), 3*width/4-20, 20, 80, WHITE);
		}
		EndDrawing();
	}

	CloseWindow();
}
