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
		printf("���������꣺");
	    while(1)
	    {
           scanf("%d %d",&x,&y); 
	       if(((x>10)||(x<1))||((y>10)||(y<1)))
	       {
		      printf("���������������������룡\n");
	       }
	       else
	       {
		      count++;
	          break;
	       }
       }

	   one_input(mine,x,y,count);              //��һ�βȵ��ײ�ը
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
		printf("���ź����㱻ը���ˣ�\n");
	}
    if(ret == 'n')
	{
		printf("��ϲ�㣬ɨ�׳ɹ���\n");
	}


}

int main()
{
	int input = 0;
	srand((unsigned)time(NULL));
	while(1)
	{
	menu();
	printf("��ѡ��");
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
		printf("ѡ�����������ѡ��");
		break;
	}
	
	}

	system("pause");
	return 0;
}