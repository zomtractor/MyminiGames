#include <conio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <graphics.h>
#include <stdio.h>
#include "function.h"
Block block[28], one;
int map[WIDE][LEN];
int score = 0;
IMAGE img[12];
void initGame()
{
	memset(map, 0, sizeof(map));
	char name[30];
	for (int i = 0; i <12; i++) {
		sprintf(name, "res/image/%.2d.jpg", i);
		loadimage(&img[i], name);
	}
	FILE *fp = fopen("block.ini", "r");
	for (int i = 0; i < 28; i++) {
		fscanf(fp, "%d", &block[i].ID);
		for (int j = 0; j < 5; j++) {
			for (int k = 0; k < 6; k++) {
				fscanf(fp, "%d", &block[i].mp[j][k]);
			}
		}
	}
	fclose(fp);
}
void summonBlock()
{
	srand((unsigned)time(NULL));
	one = block[rand() % 28];
	one.place.y = 0, one.place.x = 2;
	if (map[1][LEN >> 1] != 0) {
		gameover();
	}
}
void gameover()
{
	char mess[50];
	sprintf(mess, "game over!\nyour score: %d", score);
	if (MessageBox(NULL, mess, "S**t!", MB_OKCANCEL)==IDOK) {
		initGame();
		score = 0;
		summonBlock();
	}
	else exit(0);
}
void move(char key,bool canmove)
{
	if (!canmove) {
		return;
	}
	for (int j = 0; j < 5; j++) {
		for (int i = 0; i < 5; i++) {
			if (selfmap == 1) outtermap = 0;
		}
	}
	int c1, c2, c3;
	switch (key)
	{
	case 's':one.place.y++; break;
	case 'a':one.place.x--; break;
	case 'd':one.place.x++; break;
	}
	for (int j = 0; j < 5; j++) {
		for (int i = 0; i < 5; i++) {
			if (selfmap == 1) outtermap = one.ID%10;
		}
	}
}
inline void getKey(char key)
{
	bool canMove = true;
	switch (key) {
	case 's':case 'S':case 80:
		for (int j = 0; j < 5; j++) {
			for (int i = 0; i < 6; i++) {
				if (selfmap == 2 || selfmap == 7 || selfmap == 8)
					if (outtermap || dy >= WIDE) {
						canMove = false;
						boom();
						summonBlock();
					}
			}
		}
		move('s',canMove);
		break;
	case 'a':case 'A':case 75:
		for (int j = 0; j < 5; j++) {
			for (int i = 0; i < 6; i++) {
				if (selfmap == 4 || selfmap == 7)
					if (outtermap || dx < 0) canMove = false;
			}
		}
		move('a',canMove);
		break;
	case 'd':case 'D':case 77:
		for (int j = 0; j < 5; j++) {
			for (int i = 0; i < 6; i++) {
				if (selfmap == 6 || selfmap == 8)
					if (outtermap || dx >= LEN) canMove = false;
			}
		}
		move('d',canMove);
		break;
	case 'w':case 'W':case 72:
		change();
		break;
	default:break;
	}
}
void change()
{
	Block temp = one;
	bool canchange = true;
	for (int j = 0; j < 5; j++) {
		for (int i = 0; i < 6; i++) {
			if (selfmap == 1) outtermap = 0;
		}
	}
	one.ID = (one.ID + 10) % 40;
	for (int i = 0; i < 28; i++) {
		if (one.ID == block[i].ID) {
			one = block[i];
			one.place = temp.place;
			break;
		}
	}
	for (int j = 0; j < 5; j++) {
		for (int i = 0; i < 6; i++) {
			if (selfmap == 1)
				if (outtermap != 0) canchange = false;
		}
	}
	if(!canchange) one=temp;
	for (int j = 0; j < 5; j++) {
		for (int i = 0; i < 6; i++) {
			if (selfmap == 1) outtermap = one.ID % 10;
		}
	}

}
void boom()
{
	bool boob; 
	for (int j = WIDE - 1; j > 0; j--) {
		boob = true;
		for (int i = 0; i < LEN; i++) {
			if (map[j][i] == 0) {
				boob = false;
				break;
			}
			
		}
		if (boob) {
			memset(map[j], 0, sizeof(map[j]));
			score += 10;
			for (int k = j; k > 1; k--) {
				memcpy(map[k], map[k - 1], sizeof(map[1]));
			}
			j++;
		}
		
	}

}
void drawMap()
{
	for (int j = 0; j < WIDE; j++) {
		for (int i = 0; i < LEN; i++) {
			putimage(i * BLOCKSIZE, j * BLOCKSIZE, &img[map[j][i]]);
		}
	}
}
int main()
{
	clock_t pre, cur;
	char key;
	initgraph(LEN * BLOCKSIZE, WIDE * BLOCKSIZE);
	initGame();
	BeginBatchDraw();
	summonBlock();
	while (true) {
		pre = GetCurrentTime();
		while (true) {
			if (_kbhit()) {
				key = _getch();
				if (key < 0) key = _getch();
				getKey(key);
			}
			cur = GetCurrentTime();
			if (cur - pre >= speed) {
				//printf("(%d,%d)\n", one.place.y, one.place.x);
				getKey('s');
				drawMap();
				FlushBatchDraw();
				break;
			}
			drawMap();
			FlushBatchDraw();
		}
	}
	getchar();
	return 0;
}