#include <iostream>
#include <windows.h>
#include <conio.h>
using namespace std;

HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
const int WIDTH = 40; // ������ ���������
enum Color { DARKGREEN = 2, YELLOW = 14, RED = 12, BLUE = 9, WHITE = 15, DARKYELLOW = 6, DARKRED = 4 };
int coins = 0; // ������� ��������� �����
int health = 100; // ���������� ����� �������� �������� �����
int steps = 0;
COORD infobox;
COORD position; // ���������� ������ ���������


// function definition (����������� �������)
// ��������� �������� ������
void SetConsoleFont()
{
	// ��������� ������ �������
	CONSOLE_FONT_INFOEX font; // https://docs.microsoft.com/en-us/windows/console/console-font-infoex
	font.cbSize = sizeof(font);
	font.dwFontSize.Y = 52;
	font.FontFamily = FF_DONTCARE;
	font.FontWeight = FW_NORMAL;
	wcscpy_s(font.FaceName, 9, L"Consolas");
	SetCurrentConsoleFontEx(h, 0, &font);
}

// ��� ���� ����������������� �������, ������� ���������� ���������� ����� � ����������� ����� �������
void ShowCoins()
{
	infobox.X = WIDTH + 1;
	infobox.Y = 1;
	SetConsoleCursorPosition(h, infobox);
	SetConsoleTextAttribute(h, Color::DARKYELLOW);
	cout << "COINS: ";
	SetConsoleTextAttribute(h, Color::YELLOW);
	cout << coins << "\n"; // 0
}
void ShowHealth()
{
	infobox.Y = 2;
	SetConsoleCursorPosition(h, infobox);
	SetConsoleTextAttribute(h, Color::DARKRED);
	cout << "HEALTH: ";
	SetConsoleTextAttribute(h, Color::RED);
	cout << health << " \n";
}
void ShowSteps()
{
	infobox.Y = 3;
	SetConsoleCursorPosition(h, infobox);
	SetConsoleTextAttribute(h, Color::DARKRED);
	cout << "STEPS: ";
	SetConsoleTextAttribute(h, Color::RED);
	cout << steps << "\n";
}
void ShowHero()
{
	position.X = 0;
	position.Y = 2;
	SetConsoleCursorPosition(h, position);
	SetConsoleTextAttribute(h, Color::BLUE);
	cout << (char)2;
}

int main()
{
	// enumeration (������������ - ��� ����� ����������� ������������� ��������)
	// MazeObject - ���������������� (�����������������) ��� ������
	enum MazeObject { HALL, WALL, COIN, ENEMY, BORDER };
	enum KeyCode { ENTER = 13, ESCAPE = 27, SPACE = 32, LEFT = 75, RIGHT = 77, UP = 72, DOWN = 80 };

	SetConsoleFont(); // function call (����� �������)

	system("title Maze");
	MoveWindow(GetConsoleWindow(), 20, 60, 1400, 600, true);
	// 20 - ������ ����� �� ����� ������� �������� ����� �� ����� ������� ���� ������� (� ��������!)
	// 60 - ������ ������ �� ������� ������� �������� ����� �� ������� ������� ���� �������
	// 1850 - ������ ���� ������� � ��������
	// 900 - ������ ���� �������
	// true - ������������ ���� ����� �����������

	srand(time(0));


	const int HEIGHT = 10; // ������ ���������

	int maze[HEIGHT][WIDTH] = {}; // maze - �������� ��-���������

	// �������� ���������� �������
	for (int y = 0; y < HEIGHT; y++) // ������� �����
	{
		for (int x = 0; x < WIDTH; x++) // ������� ��������
		{
			maze[y][x] = rand() % 4; // 4 ���� �������� � ����

			if (maze[y][x] == MazeObject::ENEMY) // ���� � ��������� ���������� ����
			{
				int probability = rand() % 15; // 0...14, ���� ������ 0 - ���� ���������, ��������� ������ ���� ����� ����� ������
				if (probability != 0) // ������� �����
				{
					maze[y][x] = MazeObject::HALL; // �� ����� ����� ������ �������
				}
			}

			if (maze[y][x] == MazeObject::WALL) // ���� � ��������� ��������������� �����
			{
				int probability = rand() % 2; // 0...1, ���� ������ 0 - ����� ���������, ��������� ������ �������� ����
				if (probability != 0) // ������� �����
				{
					maze[y][x] = MazeObject::HALL; // �� ����� ����� ������ �������
				}
			}

			if (x == 0 || y == 0 || x == WIDTH - 1 || y == HEIGHT - 1) maze[y][x] = MazeObject::BORDER; // ����� �����

			if (x == 0 && y == 2 || x == 1 && y == 2 || x == 2 && y == 2) maze[y][x] = MazeObject::HALL; // ����

			if (x == WIDTH - 1 && y == HEIGHT - 3 ||
				x == WIDTH - 2 && y == HEIGHT - 3 ||
				x == WIDTH - 3 && y == HEIGHT - 3) maze[y][x] = MazeObject::HALL; // �����

		}
	}

	// ����� ���������
	for (int y = 0; y < HEIGHT; y++) // ������� �����
	{
		for (int x = 0; x < WIDTH; x++) // ������� ��������
		{
			switch (maze[y][x])
			{
			case MazeObject::HALL: // hall - �������
				cout << " ";
				break;

			case MazeObject::WALL: // wall - �����
				SetConsoleTextAttribute(h, Color::DARKGREEN);
				cout << (char)178;
				break;

			case MazeObject::BORDER: // border - �����
				SetConsoleTextAttribute(h, Color::WHITE);
				cout << (char)178;
				break;

			case MazeObject::COIN: // coin - �������
				SetConsoleTextAttribute(h, Color::YELLOW);
				cout << ".";
				break;

			case MazeObject::ENEMY: // enemy - ����
				SetConsoleTextAttribute(h, Color::RED);
				cout << (char)1;
				break;
			}
		}
		cout << "\n";
	}

	/////////////////////////////////////////////////////////////////////

	// ���������� �������� ����� (��)
	ShowHero();

	/////////////////////////////////////////////////////////////////////
	// ���������� �� ���� �����������

	ShowCoins(); // function call
	ShowHealth();
	ShowSteps();

	while (true)
	{
		if (_kbhit()) // ���� ���� ������� �� ������� �������������
		{
			int code = _getch(); // get character, ��������� ���� ������� �������
			if (code == 224) { // ���� ��� �������
				code = _getch(); // �������� ���������� ��� �������
			}

			// �������� ����������� � ������ �������
			SetConsoleCursorPosition(h, position);
			cout << " ";

			if (code == KeyCode::ENTER)
			{
				cout << "ENTER!\n";
				exit(0);
			}
			else if (code == KeyCode::ESCAPE)
			{
				cout << "ESCAPE!\n";
				exit(0);
			}
			else if (code == KeyCode::SPACE)
			{
				cout << "SPACE!\n";
				exit(0);
			}
			else if (code == KeyCode::LEFT
				&& maze[position.Y][position.X - 1] != MazeObject::WALL
				&& maze[position.Y][position.X - 1] != MazeObject::BORDER)
			{
				position.X--;
				steps++;
				ShowSteps();
			}
			else if (code == KeyCode::RIGHT // ���� �� �������� ����� �������
				&& maze[position.Y][position.X + 1] != MazeObject::WALL
				&& maze[position.Y][position.X + 1] != MazeObject::BORDER)
				// � ��� ���� � ��������� �� ��� �� ������ (��� ��) �
				// ������� (�� ���� ������) ������ �� 1 ������� �� ��
			{
				position.X++;
				steps++;
				ShowSteps();
			}
			else if (code == KeyCode::UP
				&& maze[position.Y - 1][position.X] != MazeObject::WALL
				&& maze[position.Y - 1][position.X] != MazeObject::BORDER)
			{
				position.Y--;
				steps++;
				ShowSteps();
			}
			else if (code == KeyCode::DOWN
				&& maze[position.Y + 1][position.X] != MazeObject::WALL
				&& maze[position.Y + 1][position.X] != MazeObject::BORDER)
			{
				position.Y++;
				steps++;
				ShowSteps();
			}

			// ����� �� � ����� �������
			SetConsoleCursorPosition(h, position);
			SetConsoleTextAttribute(h, Color::BLUE);
			cout << (char)2;

			////////////////////////////////////////////////////////////////
			// ����������� � ���������� �������
			if (position.Y == HEIGHT - 3 &&
				position.X == WIDTH - 1)
			{
				MessageBoxA(0, "you find exit!", "WIN!", 0);
				system("cls");
				exit(0);
				// main(); // ��� ������� �������, �� �� ������ ��������� �������
			}

			// ����������� � ���������
			// ���� � ��������� �� ������� �� (��� ���) ��������� �������
			if (maze[position.Y][position.X] == MazeObject::COIN)
			{
				coins++; // �� ���� ������� ������� ������
				ShowCoins();
				maze[position.Y][position.X] = MazeObject::HALL; // ������� ������� �� ���������
			}

			// ����������� � �������
			// ���� � ��������� �� ������� �� (��� ���) ��������� ����
			if (maze[position.Y][position.X] == MazeObject::ENEMY)
			{
				health -= 20;
				ShowHealth();
				maze[position.Y][position.X] = MazeObject::HALL; // ������� ����� �� ���������

				if (health <= 0)
				{
					int answer = MessageBoxA(0, "health is over!\n\nwant to play again?", "FAIL!", MB_YESNO);
					system("cls");
					if (answer == IDYES)
					{
						main();
					}
					else
					{
						exit(0);
					}
				}
			}
		}
		else // ������� �� ����, ������� ������
		{
			Sleep(15);
			///////////////////////////////////////////////////////////////
			// �������� ������
			COORD enemy_positions[100]; // ������ ������� ������ ���������� ������
			int enemy_count = 0; // �������� ���������� ������ �� ������ ������ ����� ���� ��

			// ������� ���� ����� ������� ���� � ���������
			for (int y = 0; y < HEIGHT; y++) // ������� �����
			{
				for (int x = 0; x < WIDTH; x++) // ������� ��������
				{
					// ���� ��������� ������ - ��� ����
					if (maze[y][x] == MazeObject::ENEMY)
					{
						enemy_positions[enemy_count].X = x;
						enemy_positions[enemy_count].Y = y;
						enemy_count++;
					}
				}
			}

			// ������� ���� ������
			for (int i = 0; i < enemy_count; i++)
			{
				int r = rand() % 100; // 0-left, 1-right, 2-up, 3-down
				if (r == LEFT &&
					maze[enemy_positions[i].Y][enemy_positions[i].X - 1] != MazeObject::WALL &&
					maze[enemy_positions[i].Y][enemy_positions[i].X - 1] != MazeObject::BORDER &&
					maze[enemy_positions[i].Y][enemy_positions[i].X - 1] != MazeObject::ENEMY)
				{
					// �������� ����� � ������ �������
					COORD temp = enemy_positions[i];
					SetConsoleCursorPosition(h, temp);
					cout << " ";
					maze[enemy_positions[i].Y][enemy_positions[i].X] = MazeObject::HALL;

					// ���������� ����� � ����� �������
					temp.X = enemy_positions[i].X - 1;
					temp.Y = enemy_positions[i].Y;
					SetConsoleCursorPosition(h, temp);
					SetConsoleTextAttribute(h, Color::RED);
					cout << (char)1;
					maze[enemy_positions[i].Y][enemy_positions[i].X - 1] = MazeObject::ENEMY;
				}
				else if (r == RIGHT &&
					maze[enemy_positions[i].Y][enemy_positions[i].X + 1] != MazeObject::WALL &&
					maze[enemy_positions[i].Y][enemy_positions[i].X + 1] != MazeObject::BORDER &&
					maze[enemy_positions[i].Y][enemy_positions[i].X + 1] != MazeObject::ENEMY)
				{
					// �������� ����� � ������ �������
					COORD temp = enemy_positions[i];
					SetConsoleCursorPosition(h, temp);
					cout << " ";
					maze[enemy_positions[i].Y][enemy_positions[i].X] = MazeObject::HALL;

					// ���������� ����� � ����� �������
					temp.X = enemy_positions[i].X + 1;
					temp.Y = enemy_positions[i].Y;
					SetConsoleCursorPosition(h, temp);
					SetConsoleTextAttribute(h, Color::RED);
					cout << (char)1;
					maze[enemy_positions[i].Y][enemy_positions[i].X + 1] = MazeObject::ENEMY;
				}

				else if (r == UP &&
					maze[enemy_positions[i].Y - 1][enemy_positions[i].X] != MazeObject::WALL &&
					maze[enemy_positions[i].Y - 1][enemy_positions[i].X] != MazeObject::BORDER &&
					maze[enemy_positions[i].Y - 1][enemy_positions[i].X] != MazeObject::ENEMY)
				{
					// �������� ����� � ������ �������
					COORD temp = enemy_positions[i];
					SetConsoleCursorPosition(h, temp);
					cout << " ";
					maze[enemy_positions[i].Y][enemy_positions[i].X] = MazeObject::HALL;

					// ���������� ����� � ����� �������
					temp.X = enemy_positions[i].X;
					temp.Y = enemy_positions[i].Y - 1;
					SetConsoleCursorPosition(h, temp);
					SetConsoleTextAttribute(h, Color::RED);
					cout << (char)1;
					maze[enemy_positions[i].Y - 1][enemy_positions[i].X] = MazeObject::ENEMY;
				}
				else if (r == DOWN &&
					maze[enemy_positions[i].Y + 1][enemy_positions[i].X] != MazeObject::WALL &&
					maze[enemy_positions[i].Y + 1][enemy_positions[i].X] != MazeObject::BORDER &&
					maze[enemy_positions[i].Y + 1][enemy_positions[i].X] != MazeObject::ENEMY)
				{
					// �������� ����� � ������ �������
					COORD temp = enemy_positions[i];
					SetConsoleCursorPosition(h, temp);
					cout << " ";
					maze[enemy_positions[i].Y][enemy_positions[i].X] = MazeObject::HALL;

					// ���������� ����� � ����� �������
					temp.X = enemy_positions[i].X;
					temp.Y = enemy_positions[i].Y + 1;
					SetConsoleCursorPosition(h, temp);
					SetConsoleTextAttribute(h, Color::RED);
					cout << (char)1;
					maze[enemy_positions[i].Y + 1][enemy_positions[i].X] = MazeObject::ENEMY;
				}
			}
		}
	}
}