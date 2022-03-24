#pragma once
#include <string.h>
#define WIDE 20
#define LEN 10
#define BLOCKSIZE 32
#define speed 1000

#define dx one.place.x+i
#define dy one.place.y+j
#define selfmap one.mp[j][i]
#define outtermap map[dy][dx]
using namespace std;
typedef struct {
	int y, x;
}pos;
struct Block{
	pos place;
	int mp[5][6];
	int ID;
} ;
void initGame();
void move(char,bool);
void change();
void getKey(char);
void boom();
void summonBlock();
void gameover();

