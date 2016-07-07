#include<stdio.h>
#include<ncurses.h>

#define COLOR_SNAKE1 1
#define COLOR_SNAKE2 2
#define COLOR_BOX 3
struct map{
	
	struct{
		int x,y;//记录地图的长宽
		char **map;
	};
	int (*read_map)(void *Self,char *p);
}map={10,5,NULL,read_map};

WINDOW *main_win,*info_win;//main为游戏主界面，info为得分界面
int init(){
	int height=map.y;
	int	width=map.x;
	initscr();
	start_color();//初始化字体颜色

	refresh();
	init_pair(COLOR_BOX,COLOR_WHITE,COLOR_BLACK);
	attron(COLOR_PAIR(COLOR_BOX));
	raw();
	noecho();//取消回显
	keypad(stdscr,TRUE);//支持特殊键位

	main_win=newwin(height+2,width+2,0,0);
	info_win=newwin(height+2,10,0,width+3);

	wattrset(main_win,COLOR_PAIR(COLOR_BOX));
	wattrset(info_win,COLOR_PAIR(COLOR_BOX));
	wborder(main_win,'|','|','-','-','+','+','+','+');
	wborder(info_win,'|','|','-','-','+','+','+','+');
	wattroff(main_win,COLOR_PAIR(COLOR_BOX));
	wattroff(info_win,COLOR_PAIR(COLOR_BOX));//绘制白色边线


	refresh();
	wrefresh(main_win);
	wrefresh(info_win);
	wattroff(main_win,COLOR_PAIR(COLOR_BOX));
	wattroff(info_win,COLOR_PAIR(COLOR_BOX));
}

int print_snake(){
	wclear(main_win);
	wclear(info_win);//清空屏幕

	init_pair(COLOR_BOX,COLOR_WHITE,COLOR_BLACK);
	init_pair(COLOR_SNAKE1,COLOR_RED,COLOR_RED);
	init_pair(COLOR_SNAKE2,COLOR_BLUE);

	wattrset(main_win,COLOR_PAIR(COLOR_BOX));
	wattrset(info_win,COLOR_PAIR(COLOR_BOX));
	wborder(main_win,'|','|','-','-','+','+','+','+');
	wborder(info_win,'|','|','-','-','+','+','+','+');
//绘制白色边线

	wattrset(main_win,COLOR_PAIR(COLOR_SNAKE1)|A_REVERSE);
	struct snake_node *node=player1.head;
	while(node!=NULL)
	{
		mvwaddch(main_win,node->position[1],node->position[0],' ');
		node=node->next;
	}//绘制player1的图像

	wattrset(main_win,COLOR_PAIR(COLOR_SNAKE2)|A_REVERSE);
	node=player2.head;
	while(node!=NULL)
	{
		mvwaddch(main_win,node->position[1],node->position[0],' ');
		node=node->next;
	}//绘制player2的图像

	refresh();
	wrefresh(main_win);
	wrefresh(info_win);//输出两部分图像
}
	


int main(){
	init();
	getch();	
	endwin();

}
