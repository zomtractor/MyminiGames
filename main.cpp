#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <graphics.h>
#define ROW 10
#define COL 10
#define BLOCKSIZE 32
#define MINECRAFT 10
#define BLOCKS ROW*COL-MINECRAFT
int map[ROW][COL];
int cnt;
IMAGE img[12];
bool gameover = false;
void GameInit()
{
    cnt = 0;
    memset(map, 0, sizeof(map));//initialize map
    srand((unsigned)time(NULL));//
    for (int t1 = 0, t2 = 0, cnt = 0; cnt < MINECRAFT;) { //summon mines
        t1 = rand() % ROW, t2 = rand() % COL;
        if (map[t1][t2] != -1) {
            map[t1][t2] = -1;
            cnt++;
            for (int j = t1 - 1; j <= t1 + 1; j++) {
                for (int i = t2 - 1; i <= t2 + 1; i++) {
                    if (j >= 0 && j < ROW && i >= 0 && i < COL && map[j][i] != -1) map[j][i]++;
                }
            }
        }
    }
    char name[50];
    for (int i = 0; i < 9; i++) {
        sprintf(name, "res/image/%.2d.jpg", i);
        loadimage(&img[i], name);
    }
    loadimage(&img[9], "res/image/mine.jpg");
    loadimage(&img[10], "res/image/on.jpg");
    loadimage(&img[11], "res/image/flag.jpg");
    for (int j = 0; j < ROW; j++) {
        for (int i = 0; i < COL; i++) map[j][i] += 20;
    }
}
void show()
{
    for (int j = 0; j < ROW; j++) {
        for (int i = 0; i < COL; i++) printf("%2d", map[j][i]);
        printf("\n");
    }
    system("cls");
}
void AnswerDraw(bool lose = false)
{
    for (int j = 0; j < ROW; j++) {
        for (int i = 0; i < COL; i++) {
            if ((map[j][i] == -1) || (lose && (map[j][i] == -1 || map[j][i] == 19 || map[j][i] == 39)))
                putimage(i * BLOCKSIZE, j * BLOCKSIZE, &img[9]);
            else if (map[j][i] < 9)
                putimage(i * BLOCKSIZE, j * BLOCKSIZE, &img[map[j][i]]);
            else if (map[j][i] >= 19 && map[j][i] <= 28)
                putimage(i * BLOCKSIZE, j * BLOCKSIZE, &img[10]);
            else if (map[j][i] > 30)
                putimage(i * BLOCKSIZE, j * BLOCKSIZE, &img[11]);
        }
    }
}
void OpenBlock(int j, int i)
{
    if (map[j][i] < 9 || map[j][i]>30) return;
    map[j][i] -= 20;
    cnt++;
    if (cnt == BLOCKS) {
        if(MessageBox(NULL,"you win!","ohhhhhhhh",MB_OKCANCEL)==IDOK){
            GameInit();
            cnt = 0;
            return;
        }else exit(0);
    }
    
    if (map[j][i] == 0) {
        if (j - 1 >= 0) {
            if (i - 1 >= 0) OpenBlock(j - 1, i - 1);
            OpenBlock(j - 1, i);
            if (i + 1 < COL) OpenBlock(j - 1, i + 1);
        }
        if (i - 1 >= 0) OpenBlock(j, i - 1);
        if (i + 1 < COL) OpenBlock(j, i + 1);
        if (j + 1 < ROW) {
            if (i - 1 >= 0) OpenBlock(j + 1, i - 1);
            OpenBlock(j + 1, i);
            if (i + 1 < COL) OpenBlock(j + 1, i + 1);
        }
    }
    else if (map[j][i] == -1) {
        gameover = true;
    }
}
void MouseControl()
{
    int row, col;
    if (MouseHit())
    {
        MOUSEMSG msg = GetMouseMsg();
        row = msg.y / BLOCKSIZE;
        col = msg.x / BLOCKSIZE;
        switch (msg.uMsg) {
        case WM_LBUTTONDOWN:
            if (map[row][col] > 8) OpenBlock(row, col); break;
        case WM_RBUTTONDOWN:
            if (map[row][col] > 8 && map[row][col] <= 28) map[row][col] += 20;
            else if (map[row][col] > 28) map[row][col] -= 20;
            break;
        }
    }
}
int main()
{

    initgraph(ROW * BLOCKSIZE, COL * BLOCKSIZE);
aa:
    GameInit();
    BeginBatchDraw();
    while (!gameover) {
        MouseControl();
        AnswerDraw();
        //show();
        FlushBatchDraw();

    }
    if (gameover) {
        AnswerDraw(true);
        if (MessageBox(NULL, "try again!", "gameover", MB_OKCANCEL) == IDOK) {
            gameover = false;
            goto aa;
        }
        else exit(0);
    }
    EndBatchDraw();
    getchar();
}
//