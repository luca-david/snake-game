#include <stdio.h>
#include <conio.h>
#include <ctype.h>
#include <stdlib.h>
#include <windows.h>	//this is for GUI apps
#include <time.h>


#define HEIGHT 20
#define WIDTH 20
#define DRAWING_HEIGHT 60
#define DRAWING_WIDTH 60

int tail_x[100], tail_y[100];	//array to store the coordinates of tail

int tail_len;		//variable to store the length of the tail

int gameover, key, score;		//score and signal flags

int x, y, fruit_x, fruit_y;		//coordinates of snake's head and fruit

int speed_fruit_x, speed_fruit_y;

int bad_fruit_x, bad_fruit_y;

int delay = 100;

int active_boost = 0;

int target_time = 0;

int first_input = 0;

int first_tap = 1;

int gamemode = 0;

int target_score = -1;

void setup()
{
	gameover = 0;	

	x = WIDTH / 2;
	y = HEIGHT / 2;

	//randomize coordinates of fruit
	fruit_x = rand() % WIDTH;
	fruit_y = rand() % HEIGHT;
	while (fruit_x == 0)			//we use while in case rand generates something divisible by width for fruit_x(same thing applies to fruit_y)
		fruit_x = rand() % WIDTH;
	while (fruit_y == 0)
		fruit_y = rand() % HEIGHT;

	//randomize coords of speed fruit
	speed_fruit_x = rand() % WIDTH;
	speed_fruit_y = rand() % HEIGHT;
	while (speed_fruit_x == 0)
	{
		speed_fruit_x = rand() % WIDTH;
	}
	while (speed_fruit_y == 0)
	{
		speed_fruit_y = rand() % HEIGHT;
	}
	
	//randomize coords of bad fruit
	bad_fruit_x = rand() % WIDTH;
	bad_fruit_y = rand() % HEIGHT;
	while (bad_fruit_x == 0)
	{
		bad_fruit_x = rand() % WIDTH;
	}
	while (bad_fruit_y == 0)
	{
		bad_fruit_y = rand() % HEIGHT;
	}

	score = 0;
}

void draw()
{
	system("cls");		//clears windows console 

	//create the top wall
	for (int i = 0; i < WIDTH + 2; i++)
	{
		printf_s("-");
	}

	for (int i = 0; i < HEIGHT; i++)			
	{
		for (int j = 0; j < WIDTH + 1; j++)
		{
			if (j == 0 || j == WIDTH)		//create left and right wall
			{
				printf_s("#");
			}

			else if (i == y && j == x)	//head of snake
			{
				printf_s("O");
			}

			else if (i == fruit_y && j == fruit_x)		//fruit placement
			{
				printf_s("*");
			}

			else if (i == speed_fruit_y && j == speed_fruit_x)
			{
				printf_s("+");
			}

			else if (i == bad_fruit_y && j == bad_fruit_x)
			{
				printf_s("-");
			}

			else {					//body of snake(or tail wtv)
				int prTail = 0;
				for (int k = 0; k < tail_len; k++)
				{
					if (tail_x[k] == j && tail_y[k] == i)
					{
						printf_s("o");
						prTail = 1;
					}
				}
				if (prTail == 0)
				{
					printf_s(" ");
				}
			}
		}
		printf_s("\n");
	}

	//create bottom walls
	for (int i = 0; i < WIDTH; i++)
	{
		printf_s("-");
	}
	printf_s("\n");

	//print score and controls
	printf_s("Score: %d\n", score);
	if (time(NULL) < first_input + 5)
	{
		printf_s("WASD to move\n");
		printf_s("Press ctrl+c to exit the game(or just x)\n");
	}
	/*printf_s("WASD to move\n");
	printf_s("Press ctrl+c to exit the game(or just x)\n");*/

	////Print a line from the top
	//for (int i = HEIGHT; i < DRAWING_HEIGHT; i++)
	//{
	//	for (int j = WIDTH; j < DRAWING_WIDTH; j++)
	//	{
	//		printf_s("!");
	//	}
	//}
}

void input()
{
	if (_kbhit())
	{
		switch (tolower(_getch()))
		{
		case 'a':
			if (key != 2)
				key = 1;
			break;
		case 'd':
			if (key != 1)
				key = 2;
			break;
		case 'w':
			if (key != 4)
				key = 3;
			break;
		case 's':
			if (key != 3)
				key = 4;
			break;
		case 'x':
			gameover = 1;
			break;
		}
		//daca am apasat pt prima oara ia timpul actual
		if (first_tap)
		{
			first_input = time(NULL);
			first_tap = 0;
		}
	}
}

void logic()
{
	for (int i = tail_len; i > 0; i--)	//start at the end of tail, and shift each segment to the position before it
	{
		tail_x[i] = tail_x[i - 1];		//last element becomes the one before it 
		tail_y[i] = tail_y[i - 1];
	}
	tail_x[0] = x;	//place new piece at the previous head position
	tail_y[0] = y;

	//change direction of movement of snake
	switch (key)
	{
	case 1:
		x--;
		break;
	case 2:
		x++;
		break;
	case 3:
		y--;	//pressing w decrease y because this is not an xy axis, this is a matrix, so we if we want to go up we should go towards 0
		break;
	case 4:
		y++;
		break;
	default:
		break;
	}
	
	//game over handling
	if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT)
	{
		gameover = 1;
	}

	//check for collision with tail
	for (int i = 0; i < tail_len; i++)
	{
		if (tail_x[i] == x && tail_y[i] == y)
		{
			gameover = 1;
		}
	}


	//check for collision with fruit
	if (x == fruit_x && y == fruit_y)
	{
		//if true respawn fruits
		fruit_x = rand() % WIDTH;
		fruit_y = rand() % HEIGHT;
		while (fruit_x == 0)
		{
			fruit_x = rand() % WIDTH;
		}
		while (fruit_y == 0)
		{
			fruit_y = rand() % HEIGHT;
		}

		//increase tail length
		tail_len++;

		//increase score
		score = score + 10;
	}

	//check for collison with speed fruit
	if (x == speed_fruit_x && y == speed_fruit_y)
	{
		//respawn speed fruit
		speed_fruit_x = rand() % WIDTH;
		speed_fruit_y = rand() % HEIGHT;
		while (speed_fruit_x == 0)
		{
			speed_fruit_x = rand() % WIDTH;
		}
		while (speed_fruit_y == 0)
		{
			speed_fruit_y = rand() % HEIGHT;
		}
		
		//activate speed boost
		active_boost = 1;
		//get actual time and add 3 to get the target time
		target_time = time(NULL) + 3;
	}
	
	if (active_boost)
	{
		//increase the speed of the game
		delay = 1;
		
		printf_s("Boost: %d\n", target_time - time(NULL));
		//check if the speed boost time is up
		if (time(NULL) > target_time)
		{
			delay = 100;
			active_boost = 0;
		}
	}

	//check for collision with bad fruit
	if (x == bad_fruit_x && y == bad_fruit_y)
	{	
		//respawn bad fruit
		bad_fruit_x = rand() % WIDTH;
		bad_fruit_y = rand() % HEIGHT;

		while (bad_fruit_x == 0)
		{
			bad_fruit_x = rand() % WIDTH;
		}
		while (bad_fruit_y == 0)
		{
			bad_fruit_y = rand() % HEIGHT;
		}

		if (tail_len > 0)	//if tail is bigger than 0
		{
			//remove one element from tail array
			for (int i = 0; i < tail_len - 1; i++)
			{
				tail_x[i] = tail_x[i + 1];
				tail_y[i] = tail_y[i + 1];
			}
			tail_len--;
			score = score - 10;
		}
		else
		{
			gameover = 1;
		}
	}

	if (gamemode == 2)
	{
		if (score == target_score)
		{
			gameover = 1;
		}
	}
	
}

void win_drawing()
{
	//draw the eyes
	for (int i = 0; i < DRAWING_WIDTH; i++)
	{
		if (i == 30 || i == DRAWING_WIDTH - 1)
		{
			printf_s("|");
		}
		else if (i == 34 || i == DRAWING_WIDTH - 4)
		{
			printf_s("|");
		}
		else
		{
			printf_s(" ");
		}
	}
	printf_s("\n");
	//draw the nose
	int count = 8;
	int temp_i = 6;
	int temp_j = 46;
	for (int i = 0; i < DRAWING_HEIGHT; i++)
	{
		for (int j = 0; j < DRAWING_WIDTH; j++)
		{
			if (i == temp_i && j == temp_j)
			{	
				if (count > 0)
				{	
					printf_s("/");
					temp_i = temp_i + 2;
					temp_j++;
					count--;
				}	
			}
			else if (i == temp_i + 2 && j == 46)
			{
				while (count < 9)
				{
					printf_s("-");
					count++;
				}
			}
			//draw the mouth
			else if (i == temp_i + 6 && j == 18 || i == temp_i + 6 && j == 59)
			{
				printf_s("-");
			}
			else if (i == temp_i + 8 && j == 19 || i == temp_i + 8 && j == 58)
			{
				printf_s("-");
			}
			else if (i == temp_i + 10 && j == 20 || i == temp_i + 10 && j == 57)
			{
				printf_s("-");
			}
			else if (i == temp_i + 12 && j == 21)
			{
				while (count < 45)
				{
					printf_s("-");
					count++;
				}
			}
			else
			{
				printf_s(" ");
			}
		}
	}
}

void main()
{
	//we need to initialize the variables
	setup();

	//take input from user, endless mode or score-based?
	printf_s("Type 1 for endless mode, or 2 for score-based mode: ");
	scanf_s("%d", &gamemode);
	if (gamemode == 2)
	{
		//score-based
		printf_s("You selected score-based!\n");
		printf_s("What is your target score?\t");
		printf_s("Be ambitious :)  ");
		scanf_s("%d", &target_score);
	}
	else if (gamemode == 1)
	{	
		Sleep(1000);
		printf_s("You selected endless mode!\n");
		Sleep(2000);
		printf_s("Have fun!");
		Sleep(2500);
	}


	while (!gameover)
	{
		//functions that will be repeatedly called after an interval
		draw();		
		input();
		logic();
		Sleep(delay);
	}

	if (gameover)
	{	
		if (score == target_score)
		{
			system("cls");
			//printf_s("You win!\n(^_^)\n");
			win_drawing();
			printf_s("You win!");
			Sleep(1000);
		}
		else
		{
			system("cls");
			printf_s("Game over!\n");
			printf_s("(-_-)\n");
			printf_s("Score: %d\n", score);
			Sleep(1000); 
		}
		
	}
}