#ifndef  __GAME_H__
#define  __GAME_H__

#define ROWS 12
#define COLS 12
#define M 98



void init_board(char board[ROWS][COLS],int rows,int cols,char ch);
void lay_mine(char board[ROWS][COLS],int rows,int cols,int m);
void dis_play(char board[ROWS][COLS],int rows,int clos);
void one_input(char board1[ROWS][COLS],int x,int y,int count);
int mine_count(char board1[ROWS][COLS],char board2[ROWS][COLS],int x,int y);
int check_win(char board[ROWS][COLS],int rows,int clos);



#endif//__GAME_H__