
#include <iostream>
#include <conio.h>
#include<windows.h>
#include <chrono>
#include <thread>

using namespace std;

//Framerate Settings
const double targetFrameRate = 15.0;
const double frameDelay = 1000.0 / targetFrameRate;

//Game "window" size
const int width = 20;
const int height = 20;

//Game entities and score int
int x, y, fruitX, fruitY, score;
int tailX[100], tailY[100];
int nTail;
bool gameOver;
//Directional control
enum eDirection { STOP = 0, LEFT, RIGHT, UP, DOWN };
eDirection dir;

void Logic();

void Setup() {
	gameOver = false;
	dir = STOP;
	x = width / 2;
	y = height / 2;
	fruitX = rand() % width;
	fruitY = rand() % height;
	score = 0;
}
void Draw() {

	system("cls");

	for (int i = 0; i < width + 1; i++) {
		cout << "#";
	}
	cout << endl;

	//RENDER
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			if (j == 0) {
				cout << "#";
			}

			else {
				if (i == y && j == x) {
					cout << "0";
				}
				else if (i == fruitY && j == fruitX) {
					cout << "@";
				}
				else {
					bool print = false;
					for (int k = 0; k < nTail; k++) {
						if (tailX[k] == j && tailY[k] == i) {
							cout << "o";
							print = true;
						}

					}
					if (!print) {
						cout << " ";
					}
				}
			}

			if (j == width - 1) {
				cout << "#";
			}
		}
		cout << endl;
	}

	for (int i = 0; i < width + 1; i++) {
		cout << "#";
	}
	cout << endl;
	cout << "Score:" << score << endl;
	cout << endl << "Use 'w' 'a' 's' and 'd' to move" << endl;
}
void Input() {
	if (_kbhit()) {
		switch (_getch())
		{
		case 'a':
			dir = LEFT;
			break;

		case 'd':
			dir = RIGHT;
			break;

		case 'w':
			dir = UP;
			break;

		case 's':
			dir = DOWN;
			break;

		case 'x':
			gameOver = true;
			break;

		}
		Logic();
	}
}
void Logic() {
	//Tail
	int prevX = tailX[0];
	int prevY = tailY[0];
	int prev2X, prev2Y;
	tailX[0] = x;
	tailY[0] = y;

	for (int i = 1; i < nTail; i++) {
		 prev2X = tailX[i];
		 prev2Y = tailY[i];
		 tailX[i] = prevX;
		 tailY[i] = prevY;
		 prevX = prev2X;
		 prevY = prev2Y;
	}

	//Movement
	switch (dir) {

		case LEFT:
			x--;
			break;

		case RIGHT:
			x++;
			break;

		case UP:
			y--;
			break;

		case DOWN:
			y++;
			break;

		default:
			break;
	}

	//Enforces game border
	if (x >= width-1) {x = 1;}
	else if (x <= 0) {x = width - 2;}
	if (y >= height) {y = 0;}
	else if (y < 0) {y = height - 1;}

	//Tail Collision
	for (int i = 0; i < nTail; i++) {
		if (tailX[i] == x && tailY[i] == y) {
			gameOver = true;
		}
	}

	//Fruit consuming logic
	if (x == fruitX && y == fruitY) {
		score += 10;
		fruitX = rand() % width;
		fruitY = rand() % height;

		nTail++;
	}
}

int main()
{
	double previousFrameTime = 0.0;
    Setup();

	while (!gameOver) {
		double currentTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		double frameTime = currentTime - previousFrameTime;

		if (frameTime >= frameDelay) {

				Draw();
				Input();
				//Logic();
			
			previousFrameTime = currentTime;
		}
		else {
			// Sleep to control the frame rate
			std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(frameDelay - frameTime)));
		}
	}


}