#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

typedef struct Desk
{
	int Width;
	int Height;
	int** Array;
	int CountBomb;
} Desk;

Desk DeskGeneration(int Width, int Height, double Percent)
{
	Desk desk;
	desk.CountBomb = 0;
	if (Width <= 0 || Height <= 0 || Percent <= 0.001 || Percent >= 1)
	{
		return desk;
	}
	desk.Width = Width;
	desk.Height = Height;
	desk.Array = (int**)malloc(Height * sizeof(int*));
	for (int i = 0; i < Height; i++)
	{
		desk.Array[i] = (int*)malloc(Width * sizeof(int));
		for (int j = 0; j < Width; j++)
		{
			desk.Array[i][j] = rand()%3 == 0 ? 1 : 0;// ((rand() % 1001) * Percent); - отредактировать
			if (desk.Array[i][j] == 1)
			{
				desk.CountBomb++;
			}
		}
	}
	return desk;
}

int isDesk(Desk desk, int x, int y)
{
	return !(x < 0 || y < 0 || x >= desk.Width || y >= desk.Height);
}

int BombCount(Desk desk, int x, int y)
{
	int count = 0;
	if (isDesk(desk, x, y - 1) && desk.Array[y - 1][x] % 10 == 1)
	{
		count++;
	}
	if (isDesk(desk, x, y + 1) && desk.Array[y + 1][x] % 10 == 1)
	{
		count++;
	}
	if (isDesk(desk, x - 1, y - 1) && desk.Array[y - 1][x - 1] % 10 == 1)
	{
		count++;
	}
	if (isDesk(desk, x - 1, y) && desk.Array[y][x - 1] % 10 == 1)
	{
		count++;
	}
	if (isDesk(desk, x - 1, y + 1) && desk.Array[y + 1][x - 1] % 10 == 1)
	{
		count++;
	}
	if (isDesk(desk, x + 1, y - 1) && desk.Array[y - 1][x + 1] % 10 == 1)
	{
		count++;
	}
	if (isDesk(desk, x + 1, y) && desk.Array[y][x + 1] % 10 == 1)
	{
		count++;
	}
	if (isDesk(desk, x + 1, y + 1) && desk.Array[y + 1][x + 1] % 10 == 1)
	{
		count++;
	}
	return count;
}



void DeskOpen(Desk desk, int x, int y)
{
	desk.Array[y][x] += 10;
	if (isDesk(desk, x, y - 1) && desk.Array[y - 1][x] == 0)
	{
		DeskOpen(desk, x, y - 1);
	}
	if (isDesk(desk, x, y + 1) && desk.Array[y + 1][x] == 0)
	{
		DeskOpen(desk, x, y + 1);
	}
	if (isDesk(desk, x - 1, y) && desk.Array[y][x - 1] == 0)
	{
		DeskOpen(desk, x - 1, y);
	}
	if (isDesk(desk, x + 1, y) && desk.Array[y][x + 1] == 0)
	{
		DeskOpen(desk, x + 1, y);
	}
}

int DeskClick(Desk desk, int x, int y)
{
	if (isDesk(desk, x, y))
	{
		if (desk.Array[y][x] % 10 == 1)
		{
			return 1;
		}
		DeskOpen(desk, x, y);
	}
	return 0;
}

void DeskDraw(Desk desk)
{
	for (int i = 0; i < desk.Height; i++)
	{
		for (int j = 0; j < desk.Width; j++)
		{
			if (desk.Array[i][j] >= 10 /*|| 1 == 1*/)
			{
				if (desk.Array[i][j] % 10 == 0)
				{
					printf("%d", BombCount(desk, j, i));
				}
				else
				{
					printf("@");
				}
				
			}
			else
			{
				printf("*");
			}
		}
		printf("\n");
	}
	printf("\n");
}


void ClearStdin() {
	int trash;
	do {
		trash = getchar();
	} while (trash != '\n' && trash != EOF);
}
int Scan(char* text) {
	int a;
	while (1) {
		printf(text);
		int err = scanf("%i", &a);
		ClearStdin();
		if (err == 1) { break; }
		printf("Incorrect data entry\n");
	}
	return a;
}

int main()
{
	srand(time(NULL));
	int x, y, r = 0;
	printf("=====Minesweeper=====\n\n");
	x = Scan("Read Width: ");
	y = Scan("Read Height: ");
	Desk desk = DeskGeneration(x, y, 0.9);
	if (desk.CountBomb > 0)
	{
		while (r == 0)
		{
			x = Scan("Read X: ");
			y = Scan("Read Y: ");
			r = DeskClick(desk, x, y);
			DeskDraw(desk);
		}
	}
	return 0;
}