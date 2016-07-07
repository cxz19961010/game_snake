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
	wborder(main_win,'|','|','-','-','+','+','+','+');
	info_win=newwin(height+2,10,0,width+3);
	wborder(info_win,'|','|','-','-','+','+','+','+');
	wrefresh(main_win);
	wrefresh(info_win);
	attroff(COLOR_PAIR(COLOR_BOX));
}

/*int print_snake(){
	init_pair(COLOR_SNAKE1,COLOR_RED,COLOR_RED);
	init_pair(COLOR_SNAKE2,COLOR_BLUE);
	
*/
	 	

int main(){
	init();
	getch();	
	endwin();

}
