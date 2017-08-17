#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <ncursesw/ncurses.h>

#define MaxLenStr 4096
#define MaxChildProc 1024
#define xx 20
#define yy 20

int print_array(int *arr, int x, int y, int max_x, int max_y, chtype *ch){
	if ( max_y<y || ((int) (max_x/2))<x ){
		return -1;
	}
	int dx=(int) ( ((int) (max_x/2))-x )/2;
	int dy=(int) (max_y-y)/2;
	for (int i = 0; i < y; ++i)
	{
		move(dy+i,2*dx);
		for (int j = 0; j < x; ++j)
		{
			addch(ch[arr[i*x+j]]);
			addch(ch[arr[i*x+j]]);			
		}
	}
	curs_set(0);
	refresh();
}


int main(int argc, char **argv){
	int max_x, max_y;
	initscr();                   // Переход в curses-режим
	getmaxyx(stdscr, max_y, max_x);
	if (!has_colors())
	{
	  endwin();
	  printf("Цвета не поддерживаются");
	  exit(1);
	}
	start_color();
	// 1 цвет в палитре - красные символы на чёрном фоне
	init_pair(1, COLOR_WHITE, COLOR_GREEN);
	// 2 цвет в палитре - зелёные символы на желтом фоне
	init_pair(2, COLOR_WHITE, COLOR_RED);
	// 2 цвет в палитре - зелёные символы на желтом фоне
	init_pair(3, COLOR_WHITE, COLOR_BLUE);
	init_pair(4, COLOR_WHITE, COLOR_BLACK);
	
	chtype ch[3];
	chtype ch1;
	ch[0] = ' ' | COLOR_PAIR(1);
	ch[1] = ' ' | COLOR_PAIR(2);
	ch[2] = ' ' | COLOR_PAIR(3);
	ch1 = ' ' | COLOR_PAIR(4);
	srand( (unsigned) (time(NULL)/2) );
	int arr[xx*yy];
	int err;
	for (int k = 0; k < 5; ++k)
	{
		for (int i = 0; i < yy; ++i)
		{
			for (int j = 0; j < xx; ++j)
			{
				arr[i*xx+j]=rand()%3;
			}
		}
		err = print_array( &arr, xx, yy, max_x, max_y, &ch);
		sleep(2);
		move((int) (max_y/2),(int) (max_x/2));
		addch(ch1);
		addch(ch1);
		refresh();
		sleep(3);	
	}
	
    endwin();                    // Выход из curses-режима. Обязательная команда.
    printf("%d\n", err);
	return(0);
}
