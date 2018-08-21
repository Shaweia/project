#define _CRT_SECURE_NO_WARNINGS 


#include"game.h"
#include<stdio.h>
#include<stdlib.h>
#include<time.h>



void menu()
{
	printf("******************************\n");
	printf("*****  1.paly    2.exit   ****\n");
	printf("******************************\n");
}



void game()
{
	int count = 0;
	int ret = 0;
	int x = 0;
	int y = 0;
	char mine[ROWS][COLS] ;
	char show[ROWS][COLS] ;
	init_board(mine,ROWS,COLS,'0'); 
	init_board(show,ROWS,COLS,'*');
	lay_mine(mine,ROWS,COLS,M);
	dis_play(show,ROWS,COLS);
	//dis_play(mine,ROWS,COLS);

	while(1)
	{
		printf("请输入坐标：");
	    while(1)
	    {
           scanf("%d %d",&x,&y); 
	       if(((x>10)||(x<1))||((y>10)||(y<1)))
	       {
		      printf("输入坐标有误，请重新输入！\n");
	       }
	       else
	       {
		      count++;
	          break;
	       }
       }

	   one_input(mine,x,y,count);              //第一次踩到雷不炸
	   //dis_play(mine,ROWS,COLS);
	   ret = mine_count(mine,show,x,y);
	   if(ret =='b')
	   {
		  break;
	   }
	   dis_play(show,ROWS,COLS);
	   ret = check_win(show,ROWS,COLS);
	   if(ret == 'n')
	   {
		   break;
	   }
	
	}


	if(ret == 'b')
	{
		printf("很遗憾，你被炸死了！\n");
	}
    if(ret == 'n')
	{
		printf("恭喜你，扫雷成功！\n");
	}


}

int main()
{
	int input = 0;
	srand((unsigned)time(NULL));
	while(1)
	{
	menu();
	printf("请选择：");
	scanf("%d",&input);
	switch(input)
	{
	case 1:
		game();
		break;
	case 2:
		return 0;
		break;
	default:
		printf("选择错误，请重新选择：");
		break;
	}
	
	}

	system("pause");
	return 0;
}