//this "game" has been made for learning Visual C++ environment

#include <iostream>
#include <conio.h>
#include <windows.h>
#include <vector>
#include <time.h>

using std::cout;

enum LOCATION //location in game (main menu, game, ...)
{
	MAIN,
	LEVELCHOOSE,
	HELP,
	GAME,
	WIN
};

enum OBJECT //object ID on map (walls, apples, ...)
{
	EMPTY,
	WALL,
	APPLE,
	PORTAL,
	HEAD,
	TAIL,
	TAILONPORTAL,
	HEADONPORTAL
};

char objectToCharacter(char object)
{
	switch (object)
	{
	case EMPTY:
		return ' ';
	case WALL:
		return 'X';
	case APPLE:
		return 'O';
	case PORTAL:
		return '@';
	case HEAD:
		return '0';
	case TAIL:
		return 'S';
	case TAILONPORTAL:
		return '$';
	case HEADONPORTAL:
		return '0';
	default:
		return '?';
	}
}

//two dimensional vector
template<typename T> 
struct vector2
{
	T x;
	T y;

	vector2<T> operator + (vector2<T> value)
	{
		return { x + value.x, y + value.y };
	}
};

bool mapFilled(char**& map, vector2<int>size)
{
	for (int x = 0; x < size.x; x++)
		for (int y = 0; y < size.y; y++)
			if (map[x][y] == EMPTY)
				return false;
	return true;
}

int main()
{
	srand(time(0));

	char location = MAIN;
	char choice;
	char level;

	vector2<int>mapSize;
	char** map = nullptr;

	std::vector<vector2<int>>snake;

	snake.push_back({ 1, 0 });
	snake.push_back({ 0, 0 });

	vector2<int>newApplePosition;
	vector2<int>portal[2];
	bool portalEscape;

	while (true)
	{
		cout << "========SNAKE========\n";

		switch (location)
		{
		case MAIN:
			cout << "=====MENU GLOWNE=====\n";
			cout << "1. Graj\n";
			cout << "2. Pomoc\n";
			cout << "3. Zakoncz\n";
			cout << "=====================";
			choice = _getch();
			system("cls");

			switch (choice)
			{
			case '1':
				location = LEVELCHOOSE;
				break;
			case '2':
				location = HELP;
				break;
			case '3':
				return 0;
			default:
				cout << "Podaj poprawna wartosc! (1-3)\n\n";
			}
			break;


		case LEVELCHOOSE:
			cout << "====WYBIERZ POZIOM====\n";
			cout << "1. Latwy\n";
			cout << "2. Sredni\n";
			cout << "3. Trudny\n";
			cout << "4. ???\n";
			cout << "5. Menu\n";
			cout << "======================";
			choice = _getch();
			system("cls");

			switch (choice)
			{
			case '1':
				location = GAME;
				level = 0;

				mapSize.x = 6;
				mapSize.y = 5;

				map = new char* [6];
				for (int i = 0; i < 6; i++)
				{
					map[i] = new char[5];

					for (int j = 0; j < 5; j++)
						map[i][j] = EMPTY;
				}

				map[0][0] = TAIL;
				map[1][0] = HEAD;
				
				while (true)
				{
					newApplePosition.x = rand() % mapSize.x;
					newApplePosition.y = rand() % mapSize.y;
					if (map[newApplePosition.x][newApplePosition.y] == EMPTY)
					{
						map[newApplePosition.x][newApplePosition.y] = APPLE;
						break;
					}
				}
				break;
			case '2':
				location = GAME;
				level = 1;

				mapSize.x = 8;
				mapSize.y = 6;

				map = new char* [8];
				for (int i = 0; i < 8; i++)
				{
					map[i] = new char[6];

					for (int j = 0; j < 6; j++)
						map[i][j] = EMPTY;
				}

				map[2][0] = WALL;
				map[2][1] = WALL;
				map[2][2] = WALL;
				map[2][3] = WALL;
				map[3][3] = WALL;
				map[4][3] = WALL;
				map[5][3] = WALL;
				map[5][2] = WALL;

				map[0][0] = TAIL;
				map[1][0] = HEAD;

				while (true)
				{
					newApplePosition.x = rand() % mapSize.x;
					newApplePosition.y = rand() % mapSize.y;
					if (map[newApplePosition.x][newApplePosition.y] == EMPTY)
					{
						map[newApplePosition.x][newApplePosition.y] = APPLE;
						break;
					}
				}
				break;
			case '3':
				location = GAME;
				level = 2;

				mapSize.x = 12;
				mapSize.y = 8;

				map = new char* [12];
				for (int i = 0; i < 12; i++)
				{
					map[i] = new char[8];

					for (int j = 0; j < 8; j++)
						map[i][j] = EMPTY;
				}

				map[4][0] = WALL;
				map[4][1] = WALL;
				map[4][2] = WALL;
				map[3][2] = WALL;
				map[2][2] = WALL;
				map[9][0] = WALL;
				map[9][1] = WALL;
				map[9][2] = WALL;
				map[9][3] = WALL;
				map[0][5] = WALL;
				map[1][5] = WALL;
				map[2][5] = WALL;
				map[3][5] = WALL;
				map[4][5] = WALL;
				map[5][5] = WALL;
				map[9][6] = WALL;
				map[9][7] = WALL;

				map[3][1] = PORTAL;
				portal[0].x = 3;
				portal[0].y = 1;
				map[11][7] = PORTAL;
				portal[1].x = 11;
				portal[1].y = 7;

				map[0][0] = TAIL;
				map[1][0] = HEAD;

				while (true)
				{
					newApplePosition.x = rand() % mapSize.x;
					newApplePosition.y = rand() % mapSize.y;
					if (map[newApplePosition.x][newApplePosition.y] == EMPTY)
					{
						map[newApplePosition.x][newApplePosition.y] = APPLE;
						break;
					}
				}
				break;
			case '4':
				location = GAME;
				level = 3;

				mapSize.x = 18;
				mapSize.y = 12;

				map = new char* [18];
				for (int i = 0; i < 18; i++)
				{
					map[i] = new char[12];

					for (int j = 0; j < 12; j++)
						map[i][j] = EMPTY;
				}

				map[4][2] = WALL;
				map[4][3] = WALL;
				map[4][4] = WALL;
				map[4][5] = WALL;
				map[5][4] = WALL;
				map[6][4] = WALL;

				map[6][5] = PORTAL;
				portal[0].x = 6;
				portal[0].y = 5;
				map[7][9] = PORTAL;
				portal[1].x = 7;
				portal[1].y = 9;

				map[0][0] = TAIL;
				map[1][0] = HEAD;

				while (true)
				{
					newApplePosition.x = rand() % mapSize.x;
					newApplePosition.y = rand() % mapSize.y;
					if (map[newApplePosition.x][newApplePosition.y] == EMPTY)
					{
						map[newApplePosition.x][newApplePosition.y] = APPLE;
						break;
					}
				}
				break;
			case '5':
				location = MAIN;
				break;
			default:
				cout << "Podaj poprawna wartosc! (1-5)\n\n";
			}
			break;


		case HELP:
			cout << "========POMOC========\n";
			cout << "Zbieraj jablka aby sie\n";
			cout << "powiekszyc. Wygrywasz\n";
			cout << "kiedy zapelnisz cala mape.\n\n";
			cout << "X - Sciana\n";
			cout << "0 - Glowa\n";
			cout << "S - Ogon\n";
			cout << "O - Jablko\n";
			cout << "@ - Portal\n\n";
			cout << "Sterowanie W A S D\n";
			cout << "lub strzalki\n\n";
			cout << "Kliknij dowolny klawisz\n";
			cout << "aby wrocic do menu.\n";
			cout << "======================";
			choice = _getch();
			system("cls");
			location = MAIN;
			break;


		case GAME:
			//drawing map
			for (int x = 0; x < mapSize.x + 2; x++)
				cout << 'X';
			cout << '\n';
			for (int y = 0; y < mapSize.y; y++)
			{
				cout << 'X';
				for (int x = 0; x < mapSize.x; x++)
					cout << objectToCharacter(map[x][y]);
				cout << "X\n";
			}
			for (int x = 0; x < mapSize.x + 2; x++)
				cout << 'X';

			choice = _getch();
			system("cls");

			switch (choice)
			{
			case 'w':
				if (snake[0].y > 0)
					switch (map[snake[0].x][snake[0].y - 1])
					{
					case EMPTY:
						if (map[snake[0].x][snake[0].y] == HEADONPORTAL)
							map[snake[0].x][snake[0].y] = TAILONPORTAL;
						else map[snake[0].x][snake[0].y] = TAIL;
						map[snake[0].x][snake[0].y - 1] = HEAD;
						if (map[snake.back().x][snake.back().y] == TAILONPORTAL)
						{
							map[portal[!portalEscape].x][portal[!portalEscape].y] = PORTAL;
							map[snake.back().x][snake.back().y] = PORTAL;
						}
						else map[snake.back().x][snake.back().y] = EMPTY;

						for (int i = snake.size() - 1; i > 0; i--)
							snake[i] = snake[i - 1];
						snake[0].y--;
						break;
					case TAIL:
						if (snake[0].x == snake.back().x && snake[0].y - 1 == snake.back().y)
						{
							if (map[snake[0].x][snake[0].y] == HEADONPORTAL)
								map[snake[0].x][snake[0].y] = TAILONPORTAL;
							else map[snake[0].x][snake[0].y] = TAIL;
							map[snake[0].x][snake[0].y - 1] = HEAD;

							for (int i = snake.size() - 1; i > 0; i--)
								snake[i] = snake[i - 1];
							snake[0].y--;
						}
						break;
					case APPLE:

						if (map[snake[0].x][snake[0].y] == HEADONPORTAL)
							map[snake[0].x][snake[0].y] = TAILONPORTAL;
						else map[snake[0].x][snake[0].y] = TAIL;
						map[snake[0].x][snake[0].y - 1] = HEAD;

						snake.push_back(snake.back());
						for (int i = snake.size() - 2; i > 0; i--)
							snake[i] = snake[i - 1];
						snake[0].y--;

						if (mapFilled(map, mapSize))
						{
							snake.assign({{ 1, 0 }, { 0, 0 }});
							location = WIN;
						}
						else while (true)
						{
							newApplePosition.x = rand() % mapSize.x;
							newApplePosition.y = rand() % mapSize.y;
							if (map[newApplePosition.x][newApplePosition.y] == EMPTY)
							{
								map[newApplePosition.x][newApplePosition.y] = APPLE;
								break;
							}
						}
						break;
					case PORTAL:
						if (snake[0].x == portal[0].x && snake[0].y - 1 == portal[0].y)
							portalEscape = 1;
						else portalEscape = 0;

						map[snake[0].x][snake[0].y - 1] = TAILONPORTAL;
						map[portal[portalEscape].x][portal[portalEscape].y] = HEADONPORTAL;
						map[snake[0].x][snake[0].y] = TAIL;
						map[snake.back().x][snake.back().y] = EMPTY;

						for (int i = snake.size() - 1; i > 0; i--)
							snake[i] = snake[i - 1];
						snake[0].x = portal[portalEscape].x;
						snake[0].y = portal[portalEscape].y;
						break;
					}
				break;
			case 's':
				if (snake[0].y < mapSize.y - 1)
					switch (map[snake[0].x][snake[0].y + 1])
					{
					case EMPTY:
						if (map[snake[0].x][snake[0].y] == HEADONPORTAL)
							map[snake[0].x][snake[0].y] = TAILONPORTAL;
						else map[snake[0].x][snake[0].y] = TAIL;
						map[snake[0].x][snake[0].y + 1] = HEAD;
						if (map[snake.back().x][snake.back().y] == TAILONPORTAL)
						{
							map[portal[!portalEscape].x][portal[!portalEscape].y] = PORTAL;
							map[snake.back().x][snake.back().y] = PORTAL;
						}
						else map[snake.back().x][snake.back().y] = EMPTY;

						for (int i = snake.size() - 1; i > 0; i--)
							snake[i] = snake[i - 1];
						snake[0].y++;
						break;
					case TAIL:
						if (snake[0].x == snake.back().x && snake[0].y + 1 == snake.back().y)
						{
							if (map[snake[0].x][snake[0].y] == HEADONPORTAL)
								map[snake[0].x][snake[0].y] = TAILONPORTAL;
							else map[snake[0].x][snake[0].y] = TAIL;
							map[snake[0].x][snake[0].y + 1] = HEAD;

							for (int i = snake.size() - 1; i > 0; i--)
								snake[i] = snake[i - 1];
							snake[0].y++;
						}
						break;
					case APPLE:
						if (map[snake[0].x][snake[0].y] == HEADONPORTAL)
							map[snake[0].x][snake[0].y] = TAILONPORTAL;
						else map[snake[0].x][snake[0].y] = TAIL;
						map[snake[0].x][snake[0].y + 1] = HEAD;

						snake.push_back(snake.back());
						for (int i = snake.size() - 2; i > 0; i--)
							snake[i] = snake[i - 1];
						snake[0].y++;

						if (mapFilled(map, mapSize))
						{
							snake.assign({ { 1, 0 }, { 0, 0 } });
							location = WIN;
						}
						else while (true)
						{
							newApplePosition.x = rand() % mapSize.x;
							newApplePosition.y = rand() % mapSize.y;
							if (map[newApplePosition.x][newApplePosition.y] == EMPTY)
							{
								map[newApplePosition.x][newApplePosition.y] = APPLE;
								break;
							}
						}
						break;
					case PORTAL:
						if (snake[0].x == portal[0].x && snake[0].y + 1 == portal[0].y)
							portalEscape = 1;
						else portalEscape = 0;

						map[snake[0].x][snake[0].y + 1] = TAILONPORTAL;
						map[portal[portalEscape].x][portal[portalEscape].y] = HEADONPORTAL;
						map[snake[0].x][snake[0].y] = TAIL;
						map[snake.back().x][snake.back().y] = EMPTY;

						for (int i = snake.size() - 1; i > 0; i--)
							snake[i] = snake[i - 1];
						snake[0].x = portal[portalEscape].x;
						snake[0].y = portal[portalEscape].y;
						break;
					}
				break;
			case 'a':
				if (snake[0].x > 0)
					switch (map[snake[0].x - 1][snake[0].y])
					{
					case EMPTY:
						if (map[snake[0].x][snake[0].y] == HEADONPORTAL)
							map[snake[0].x][snake[0].y] = TAILONPORTAL;
						else map[snake[0].x][snake[0].y] = TAIL;
						map[snake[0].x - 1][snake[0].y] = HEAD;
						if (map[snake.back().x][snake.back().y] == TAILONPORTAL)
						{
							map[portal[!portalEscape].x][portal[!portalEscape].y] = PORTAL;
							map[snake.back().x][snake.back().y] = PORTAL;
						}
						else map[snake.back().x][snake.back().y] = EMPTY;

						for (int i = snake.size() - 1; i > 0; i--)
							snake[i] = snake[i - 1];
						snake[0].x--;
						break;
					case TAIL:
						if (snake[0].x - 1 == snake.back().x && snake[0].y == snake.back().y)
						{
							if (map[snake[0].x][snake[0].y] == HEADONPORTAL)
								map[snake[0].x][snake[0].y] = TAILONPORTAL;
							else map[snake[0].x][snake[0].y] = TAIL;
							map[snake[0].x - 1][snake[0].y] = HEAD;

							for (int i = snake.size() - 1; i > 0; i--)
								snake[i] = snake[i - 1];
							snake[0].x--;
						}
						break;
					case APPLE:
						if (map[snake[0].x][snake[0].y] == HEADONPORTAL)
							map[snake[0].x][snake[0].y] = TAILONPORTAL;
						else map[snake[0].x][snake[0].y] = TAIL;
						map[snake[0].x - 1][snake[0].y] = HEAD;

						snake.push_back(snake.back());
						for (int i = snake.size() - 2; i > 0; i--)
							snake[i] = snake[i - 1];
						snake[0].x--;

						if (mapFilled(map, mapSize))
						{
							snake.assign({ { 1, 0 }, { 0, 0 } });
							location = WIN;
						}
						else while (true)
						{
							newApplePosition.x = rand() % mapSize.x;
							newApplePosition.y = rand() % mapSize.y;
							if (map[newApplePosition.x][newApplePosition.y] == EMPTY)
							{
								map[newApplePosition.x][newApplePosition.y] = APPLE;
								break;
							}
						}
						break;
					case PORTAL:
						if (snake[0].x - 1 == portal[0].x && snake[0].y == portal[0].y)
							portalEscape = 1;
						else portalEscape = 0;

						map[snake[0].x - 1][snake[0].y] = TAILONPORTAL;
						map[portal[portalEscape].x][portal[portalEscape].y] = HEADONPORTAL;
						map[snake[0].x][snake[0].y] = TAIL;
						map[snake.back().x][snake.back().y] = EMPTY;

						for (int i = snake.size() - 1; i > 0; i--)
							snake[i] = snake[i - 1];
						snake[0].x = portal[portalEscape].x;
						snake[0].y = portal[portalEscape].y;
						break;
					}
				break;
			case 'd':
				if (snake[0].x < mapSize.x - 1)
					switch (map[snake[0].x + 1][snake[0].y])
					{
					case EMPTY:
						if (map[snake[0].x][snake[0].y] == HEADONPORTAL)
							map[snake[0].x][snake[0].y] = TAILONPORTAL;
						else map[snake[0].x][snake[0].y] = TAIL;
						map[snake[0].x + 1][snake[0].y] = HEAD;
						if (map[snake.back().x][snake.back().y] == TAILONPORTAL)
						{
							map[portal[!portalEscape].x][portal[!portalEscape].y] = PORTAL;
							map[snake.back().x][snake.back().y] = PORTAL;
						}
						else map[snake.back().x][snake.back().y] = EMPTY;

						for (int i = snake.size() - 1; i > 0; i--)
							snake[i] = snake[i - 1];
						snake[0].x++;
						break;
					case TAIL:
						if (snake[0].x + 1 == snake.back().x && snake[0].y == snake.back().y)
						{
							if (map[snake[0].x][snake[0].y] == HEADONPORTAL)
								map[snake[0].x][snake[0].y] = TAILONPORTAL;
							else map[snake[0].x][snake[0].y] = TAIL;
							map[snake[0].x + 1][snake[0].y] = HEAD;

							for (int i = snake.size() - 1; i > 0; i--)
								snake[i] = snake[i - 1];
							snake[0].x++;
						}
						break;
					case APPLE:
						if (map[snake[0].x][snake[0].y] == HEADONPORTAL)
							map[snake[0].x][snake[0].y] = TAILONPORTAL;
						else map[snake[0].x][snake[0].y] = TAIL;
						map[snake[0].x + 1][snake[0].y] = HEAD;

						snake.push_back(snake.back());
						for (int i = snake.size() - 2; i > 0; i--)
							snake[i] = snake[i - 1];
						snake[0].x++;

						if (mapFilled(map, mapSize))
						{
							snake.assign({ { 1, 0 }, { 0, 0 } });
							location = WIN;
						}
						else while (true)
						{
							newApplePosition.x = rand() % mapSize.x;
							newApplePosition.y = rand() % mapSize.y;
							if (map[newApplePosition.x][newApplePosition.y] == EMPTY)
							{
								map[newApplePosition.x][newApplePosition.y] = APPLE;
								break;
							}
						}
						break;
					case PORTAL:
						if (snake[0].x + 1 == portal[0].x && snake[0].y == portal[0].y)
							portalEscape = 1;
						else portalEscape = 0;

						map[snake[0].x + 1][snake[0].y] = TAILONPORTAL;
						map[portal[portalEscape].x][portal[portalEscape].y] = HEADONPORTAL;
						map[snake[0].x][snake[0].y] = TAIL;
						map[snake.back().x][snake.back().y] = EMPTY;

						for (int i = snake.size() - 1; i > 0; i--)
							snake[i] = snake[i - 1];
						snake[0].x = portal[portalEscape].x;
						snake[0].y = portal[portalEscape].y;
						break;
					}
				break;
			}
			break;
			

			case WIN:
				cout << "Wygrales!!!\n";
				cout << "Kliknij dowolny klawisz\n";
				cout << "aby wrocic do menu";
				choice = _getch();
				system("cls");
				location = MAIN;
				break;
		}
	}

	return 0;
}