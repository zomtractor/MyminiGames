#pragma once
#include <graphics.h>
#include <queue>
#define WIDE 30
#define LEN 40
#define BLOCKSIZE 16
#define SPEED 100

 
#define HX snake.body.back().x 
#define HY snake.body.back().y
#define TX snake.body.front().x
#define TY snake.body.front().y

IMAGE img[12];
int map[WIDE][LEN];
struct pos {
    int x, y;
    int nextx, nexty;
};
struct {
    std::queue<pos> body;
    int size;
    char dir;
} snake;
void summonFood();
void drawMap();
void move();
void initGame();
int score = 0;
