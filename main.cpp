#include <conio.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "snake.h"
void initGame()
{
    char name[30];
    for (int i = 0; i < 9; i++) {
        sprintf(name, "res/image/%.2d.jpg", i);
        loadimage(&img[i], name, 16, 16);
    }
    loadimage(&img[9], "res/image/mine.jpg", 16, 16);
    loadimage(&img[10], "res/image/on.jpg", 16, 16);
    loadimage(&img[11], "res/image/flag.jpg", 16, 16);
    for (int j = 0; j < WIDE; j++) {
        for (int i = 0; i < LEN; i++) {
            if (j == 0 || i == 0 || j == WIDE - 1 || i == LEN - 1) map[j][i] = 10;
            else map[j][i] = 0;
        }
    }
    snake.body.push({ LEN / 2,WIDE / 2 });
    snake.body.push({ LEN / 2 - 1,WIDE / 2 });
    snake.body.push({ LEN / 2 - 2,WIDE / 2 });
    snake.size = 3;
    snake.dir = 'd';
    summonFood();
    drawMap();
    getch();
}
void drawMap()
{
    for (int j = 0; j < WIDE; j++) {
        for (int i = 0; i < LEN; i++) {
            putimage(i * BLOCKSIZE, j * BLOCKSIZE, &img[map[j][i]]);
        }
    }
}
void summonFood()
{
    int x, y;
    srand(time(NULL));
    while (true) {
        x = rand() % LEN, y = rand() % WIDE;
        if (map[y][x] == 0) {
            map[y][x] = 11;
            break;
        }
    }
}
void move() 
{
    map[HY][HX] = rand() % 7 + 1;
    switch (snake.dir) {
    case 'd':
        snake.body.push({ HX + 1,HY }); break;
    case 'a':
        snake.body.push({ HX - 1 , HY }); break;
    case 'w':
        snake.body.push({ HX,HY - 1 }); break;
    case 's':
        snake.body.push({ HX,HY + 1 }); break;
    }
    if (map[HY][HX] == 0) {
        map[HY][HX] = 9;
        map[TY][TX] = 0;
        snake.body.pop();
    }
    else if (map[HY][HX] == 11) {
        map[HY][HX] = 9;
        snake.size++;
        summonFood();
        score += 10;
    }
    else {
        char message[50];
        sprintf(message, "game over!\nyour score: %d", score);
        if (MessageBox(NULL, message, "s**t!", MB_OKCANCEL) == IDOK) {
            while (!snake.body.empty()) snake.body.pop();
            initGame();
            score = 0;
            
        }
        else exit(0);
    }
}
int main()
{
    clock_t pre, cur;
    char key;
    srand(time(NULL));
    initgraph(LEN * BLOCKSIZE, WIDE * BLOCKSIZE);
    initGame();

    BeginBatchDraw();
    while (true) {
        pre = GetCurrentTime();
        while (true) {
            cur = GetCurrentTime();
            if (_kbhit()) {
                key = getch();
                if (key < 0) key = getch();
                switch (key) {
                case 'w':case 'W':case 72:
                    if (snake.dir != 's') snake.dir = 'w'; break;
                case 's':case 'S':case 80:
                    if (snake.dir != 'w') snake.dir = 's'; break;
                case 'a':case 'A':case 75:
                    if (snake.dir != 'd') snake.dir = 'a'; break;
                case 'd':case 'D':case 77:
                    if (snake.dir != 'a') snake.dir = 'd'; break;
                case 32:
                    getch(); break;
                default:break;
                }
            }
            if (cur - pre >= SPEED) {
                move();
                drawMap();
                FlushBatchDraw();
                break;
            }
        }
    }
    EndBatchDraw();
    closegraph();
    getchar();
    return 0;
}