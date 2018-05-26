#include "stdafx.h"

#include "KMain.h"
#include "resource.h"
#include <stdio.h>
#include<time.h>
#include<stdlib.h>
#include <Windows.h>

#define RECT_WIDTH 100
#define RECT_HEIGHT 100
#define MAX 3
#define DIR_LEFT 0
#define DIR_RIGHT 1
#define DIR_UP 2
#define DIR_DOWN 3

int mx;
int my;
HWND mHwnd;
HINSTANCE mInst;

int puzzle[MAX][MAX] = { 0 };

HBITMAP hBitmap;
RECT crt;

#pragma warning(disable:4996)
void initArray();
int changeElement(int sx, int sy, int dx, int dy);
void shakeArray();
void startNewGame();

void startNewGame()
{
	initArray();
	shakeArray();
}

void initArray()
{
	int a = 1;
	for (int y = 0; y < MAX; y++)
	{
		for (int x = 0; x < MAX; x++)
		{
			puzzle[y][x] = a++;
		}
	}
}

int changeElement(int sx, int sy, int dx, int dy)
{
	if (0 > sx || 0 > sy || 0 > dx || 0 > dy)
	{
		return -1;
	}
	
	if (MAX <= sx || MAX <= sy || MAX <= dx || MAX <= dy)
	{
		return -1;
	}

	if (9 == puzzle[dy][dx])
	{
		int t = puzzle[sy][sx];
		puzzle[sy][sx] = puzzle[dy][dx];
		puzzle[dy][dx] = t;
	}

	return 0;
}

void shakeArray()
{
	int x = MAX-1;
	int y = MAX-1;
	for (int i = 0; i < 10000; i++)
	{
		int ran = rand() % 4;

		switch (ran)
		{
		case DIR_LEFT:
			if (0 == changeElement(x, y, x - 1, y))
			{
				x--;
			}
			break;
		case DIR_RIGHT:
			if (0 == changeElement(x, y, x + 1, y))
			{
				x++;
			}
			break;
		case DIR_UP:
			if (0 == changeElement(x, y, x, y - 1))
			{
				y--;
			}
			break;
		case DIR_DOWN:
			if (0 == changeElement(x, y, x, y + 1))
			{
				y++;
			}
			break;
		}
	}
}

void OnCreate(HWND hWnd, HINSTANCE hInst)
{
	mHwnd = hWnd;
	mInst = hInst;
	srand(time(NULL));

	hBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP2));
	GetClientRect(hWnd, &crt);

	startNewGame();
}

int gameOver()
{
	int a = 1;
	for (int y = 0; y < MAX; y++)
	{
		for (int x = 0; x < MAX; x++)
		{
			if (a != puzzle[y][x])
			{
				return -1;
			}
			a++;
		}
	}
	return 0;
}



void OnDraw(HDC hdc)
{
	HDC hMemDC;
	HBITMAP OldBitmap;

	hMemDC = CreateCompatibleDC(hdc);
	OldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);

	for (int y = 0; y < MAX; y++)
	{
		for (int x = 0; x < MAX; x++)
		{
			int sx = (puzzle[y][x] - 1) % MAX, sy = (puzzle[y][x] - 1) / MAX;
			if (9 != puzzle[y][x])
			{
				BitBlt(hdc, x*RECT_WIDTH, y*RECT_HEIGHT, RECT_WIDTH, RECT_HEIGHT, hMemDC, RECT_WIDTH*sx,RECT_HEIGHT * sy, SRCCOPY);
			}
			
			//Rectangle(hdc, RECT_WIDTH * x, RECT_HEIGHT * y, RECT_WIDTH * x + RECT_WIDTH, RECT_HEIGHT * y + RECT_HEIGHT);
		}
	}

	//for (int y = 0; y < MAX; y++)
	//{
	//	for (int x = 0; x < MAX; x++)
	//	{
	//		if (9 != puzzle[y][x]) 
	//		{
	//			char msg[20];
	//			wsprintf(msg, "%d", puzzle[y][x]);
	//			TextOutA(hdc, 100 * x + 50, 100 * y + 50, msg, strlen(msg));
	//		}
	//		
	//	}
	//}

	SelectObject(hMemDC, OldBitmap); 
	DeleteDC(hMemDC); // hMemDC 제거

}

void OnLButtonDown(HWND hWnd, LPARAM lParam)
{
	// 완성되었는지?
	if (0 == gameOver())
	{
		MessageBoxA(hWnd, "GameOver", "GameOver", MB_OK);
		shakeArray();
	}

	int lx = LOWORD(lParam);
	int ly = HIWORD(lParam);

	if (lx > MAX*RECT_WIDTH || ly > MAX*RECT_HEIGHT)
	{
		return;
	}

	int x = lx / RECT_WIDTH;
	int y = ly / RECT_WIDTH;

	if (0 == changeElement(x, y, x - 1, y))
	{

	}
	if (0 == changeElement(x, y, x + 1, y))
	{

	}
	if (0 == changeElement(x, y, x, y - 1))
	{

	}
	if (0 == changeElement(x, y, x, y + 1))
	{

	}

	
	InvalidateRect(hWnd, NULL, true);
}

void OnKeyDown(HWND hWnd,WPARAM wParam)
{
	switch (wParam)
	{
	case VK_LEFT:
		break;
	case VK_RIGHT:
		break;
	case VK_DOWN:
		break;
	case VK_UP:
		shakeArray();
		break;
	}
	InvalidateRect(hWnd, NULL, false);
}