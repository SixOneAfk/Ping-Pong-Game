#include <iostream>
#include <raylib.h>

using namespace std;



const int screenWidth = 1200;
const int screenHeight = 800;

int playerScore = 0;
int cpuScore = 0;

class Ball {
public:
	float x, y;
	int speedX, speedY;
	int radius;
	//Drawing Ball shape
	void Draw() {
		DrawCircle(x, y, radius, RED);
	}
	//Updating ball movements
    void Update(int screenWidth, int screenHeight) {
		x += speedX;
		y += speedY;

		if (x + radius >= screenWidth)
		{
			playerScore++;
			ResetBall();
		}
		if (x - radius <= 0)
		{
			cpuScore++;
			ResetBall();
		}
	    //Checking collision with top or bottom walls
		if (y + radius >= screenHeight || y - radius <= 0)
		{
			speedY *= -1;
		}
	}
	//Setting ball back in the center with random ititial direction after scoring
	void ResetBall() {
		x = GetScreenWidth() / 2;
		y = GetScreenHeight() / 2;

		int speedChoices[2] = { -1, 1 };
		speedX *= speedChoices[GetRandomValue(0, 1)];
		speedY *= speedChoices[GetRandomValue(0, 1)];
	}
};

class Paddle {
//This function makes sure the paddles do not move out from the game window
protected :
	void LimitMovement() {
		if (y <= 0)
		{
			y = 0;
		}

		if (y + height >= screenHeight)
		{
			y = screenHeight - height;
		}
	}
public :
	float x, y;
	float width, height;
	int speed;
	Color color;

	void Draw() {
		DrawRectangle(x, y, width, height, color);
	}
//Function to move paddles up and down in window range
	void Update() {
		if (IsKeyDown(KEY_UP)) 
		{
			y -= speed;
		}
		if (IsKeyDown(KEY_DOWN))
		{
			y += speed;
		}

		LimitMovement();
	}
};
// Child class of Paddle with specific function to follow Y coordinate of the ball
class CpuPaddle: public Paddle {
public:
	void Update(int ballY) 
	{
		if (y + height / 2 > ballY) {
			y = y - speed;
		}
		if (y + height / 2 <= ballY) {
			y = y + speed;
		}

		LimitMovement();
	}


};

//Creating game objects
Ball ball;
Paddle player;
CpuPaddle cpu;

int main() {
//Initializing the characteristics of the object's
	ball.radius = 20;
	ball.x = screenWidth / 2;
	ball.y = screenHeight / 2;
	ball.speedX = 7;
	ball.speedY = 7;

	player.color = GREEN;
	player.width = 25;
	player.height = 120;
	player.x = 10;
	player.y = screenHeight / 2 - player.height / 2;
	player.speed = 6;

	cpu.color = WHITE;
	cpu.width = 25;
	cpu.height = 120;
	cpu.x = screenWidth - cpu.width - 10;
	cpu.y = screenHeight / 2 - cpu.height / 2;
	cpu.speed = 6;

//Creating a game window
	InitWindow(screenWidth, screenHeight, "Raylib Ping Pong");
	SetTargetFPS(60);
//Game loop
	while (!WindowShouldClose()) {

		BeginDrawing();

		//Updates
		ball.Update(GetScreenWidth(), GetScreenHeight());
		player.Update();
		cpu.Update(ball.y);

		//Check collisions
		if (CheckCollisionCircleRec(Vector2{ ball.x, ball.y }, ball.radius, Rectangle{ player.x, player.y, player.width, player.height }))
		{
			ball.speedX *= -1;
		}

		if (CheckCollisionCircleRec(Vector2{ ball.x, ball.y }, ball.radius, Rectangle{ cpu.x, cpu.y, cpu.width, cpu.height }))
		{
			ball.speedX *= -1;
		}




		//Drawing
		ClearBackground(BLACK);
		DrawLine(screenWidth / 2, 0, screenWidth / 2, screenHeight, WHITE);
		ball.Draw();
		player.Draw();
		cpu.Draw();

		DrawText(TextFormat("%i", playerScore), screenWidth/4 - 20, 20, 80, WHITE);
		DrawText(TextFormat("%i", cpuScore), 3 * screenWidth / 4 - 20, 20, 80, WHITE);


		EndDrawing();
	}

	CloseWindow();




	return 0;
}


