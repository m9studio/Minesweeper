#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

typedef struct Panel Panel;
typedef struct Panel
{
	Panel* Left;
	Panel* Right;
	Panel* Top;
	Panel* Bottom;
	Panel* LeftTop;
	Panel* RightTop;
	Panel* LeftBottom;
	Panel* RightBottom;
	int Bomb;
	int Check;
} Panel;
typedef struct Desk
{
	int Width;
	int Height;
	Panel** Array;
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
	desk.Array = (Panel**)malloc(Height * sizeof(Panel*));
	for (int i = 0; i < Height; i++)
	{
		desk.Array[i] = (Panel*)malloc(Width * sizeof(Panel));
		for (int j = 0; j < Width; j++)
		{
			desk.Array[i][j] = (Panel){ NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0 ,0 };
			desk.Array[i][j].Bomb = rand()%3 == 0 ? 1 : 0;// ((rand() % 1001) * Percent); - отредактировать
			if (desk.Array[i][j].Bomb == 1)
			{
				desk.CountBomb++;
			}
			if (i > 0)
			{
				desk.Array[i][j].Top = &desk.Array[i - 1][j];
				desk.Array[i - 1][j].Bottom = &desk.Array[i][j];
				if (j + 1 < Width)
				{
					desk.Array[i][j].RightTop = &desk.Array[i - 1][j + 1];
					desk.Array[i - 1][j + 1].LeftBottom = &desk.Array[i][j];
				}

			}
			if (j > 0)
			{
				desk.Array[i][j].Left = &desk.Array[i][j - 1];
				desk.Array[i][j - 1].Right = &desk.Array[i][j];
				if (i > 0)
				{
					desk.Array[i][j].LeftTop = &desk.Array[i - 1][j - 1];
					desk.Array[i - 1][j - 1].RightBottom = &desk.Array[i][j];
				}
			}
		}
	}
	return desk;
}
int BombCount(Panel panel)
{
	int count = 0;
	if (panel.Top != NULL && panel.Top->Bomb == 1)
	{
		count++;
	}
	if (panel.Bottom != NULL && panel.Bottom->Bomb == 1)
	{
		count++;
	}
	if (panel.Left != NULL && panel.Left->Bomb == 1)
	{
		count++;
	}
	if (panel.Right != NULL && panel.Right->Bomb == 1)
	{
		count++;
	}
	if (panel.LeftTop != NULL && panel.LeftTop->Bomb == 1)
	{
		count++;
	}
	if (panel.RightTop != NULL && panel.RightTop->Bomb == 1)
	{
		count++;
	}
	if (panel.LeftBottom != NULL && panel.LeftBottom->Bomb == 1)
	{
		count++;
	}
	if (panel.RightBottom != NULL && panel.RightBottom->Bomb == 1)
	{
		count++;
	}
	return count;
}



void DeskOpen(Desk desk, int x, int y)
{
	desk.Array[y][x].Check = 1;
	if (desk.Array[y][x].Top != NULL && desk.Array[y][x].Top->Bomb == 0 && desk.Array[y][x].Top->Check == 0)
	{
		DeskOpen(desk, x, y - 1);
	}
	if (desk.Array[y][x].Bottom != NULL && desk.Array[y][x].Bottom->Bomb == 0 && desk.Array[y][x].Bottom->Check == 0)
	{
		DeskOpen(desk, x, y + 1);
	}
	if (desk.Array[y][x].Left != NULL && desk.Array[y][x].Left->Bomb == 0 && desk.Array[y][x].Left->Check == 0)
	{
		DeskOpen(desk, x - 1, y);
	}
	if (desk.Array[y][x].Right != NULL && desk.Array[y][x].Right->Bomb == 0 && desk.Array[y][x].Right->Check == 0)
	{
		DeskOpen(desk, x + 1, y);
	}
}

int DeskClick(Desk desk, int x, int y)
{
	if (x < 0 || y < 0 || x >= desk.Width || y >= desk.Height)
	{
		if (desk.Array[y][x].Bomb == 1)
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
			if (desk.Array[i][j].Check == 1 /*|| 1 == 1*/)
			{
				if (desk.Array[i][j].Bomb == 0)
				{
					printf("%d", BombCount(desk.Array[i][j]));
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