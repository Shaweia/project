#define _CRT_SECURE_NO_WARNINGS 
#include"game.h"
#include<stdio.h>
#include<stdlib.h>


void init_board(char board[ROWS][COLS],int rows,int cols,char ch)    //填充棋盘
{
	int i = 0;
	int j = 0;
	for(i = 0;i < rows;i++)
	{
		for(j = 0;j < cols;j++)    
		{
			board[i][j] = ch;
		}
	}
}

void lay_mine(char board[ROWS][COLS],int rows,int cols,int m)   //埋雷
{
	int i = 0;
	while(i<m)
	{
		while(1)
		{
		int x = (rand()%(ROWS-2))+1;
	    int y = (rand()%(COLS-2))+1;
		if(board[x][y] != '1')
		{
			board[x][y] = '1';
			break;
		}
		}
		i++;
	}
}


void dis_play(char board[ROWS][COLS],int rows,int clos)    //打印棋盘
{
	int i = 0;
	int j = 0;
	printf("   ");
	for(i = 1;i<rows-1;i++)
	{
		printf("%3d",i);
	}
	printf("\n");
	for(i = 0;i<rows-1;i++)
	{
		printf("---");
	}
	printf("\n");
	for(i = 1;i < rows-1;i++)
	{
		printf("%2d|",i);
		for(j = 1;j < clos-1;j++)
		{
			printf("%3c",board[i][j]);
		}
		printf("\n");
	}
}


void one_input(char board1[ROWS][COLS],int x,int y,int count)   //第一次输入坐标时不炸
{
	int x1 = 0;
	int y1 = 0;
	if((count == 1)&&(board1[x][y] == '1'))                         
	{
		while(1)
		{
		 x1 = (rand()%(ROWS-2))+1;
	     y1 = (rand()%(COLS-2))+1;
	     if(board1[x1][y1] == '0')        //把雷移走
		 {
			 int tmp;
			 tmp = board1[x][y];
			 board1[x][y] = board1[x1][y1];
			 board1[x1][y1] = tmp;
			 break;
		 }

		}
	}
}


int mine_count(char board1[ROWS][COLS],char board2[ROWS][COLS],int x,int y)     //周围雷的个数
{
	if(board1[x][y] == '1')                 //踩到雷，返回‘b’
	{
		board2[x][y] = '1';
		return 'b';
	}
	if(board1[x][y] == '0')
	{
					
	    board2[x][y] = (board1[x-1][y-1] + board1[x-1][y] + board1[x-1][y+1] + 
			            board1[x][y-1]   + board1[x][y+1] + 
					    board1[x+1][y-1] + board1[x+1][y] + board1[x+1][y+1]) - (7*48) ;
	}
	return 1;
}


int check_win(char board1[ROWS][COLS],int rows,int cols)                  //判断没有雷的地方是否扫完
{
	int i = 0;
	int j = 0;
	int m = 0;
	for(i = 1;i <rows-1; i++)
	{
		for(j = 1;j <cols-1;j++)
		{
			if(board1[i][j] != '*')
			{
				m++;
			}
		}
	}
	if(m == (100-M))
	{
		return 'n';
	}

	return 1;
}