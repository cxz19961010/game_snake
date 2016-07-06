#include<stdio.h>
#include<ncurses.h>


WINDOW *main_win,*info_win;//main为游戏主界面，info为得分界面
int init(int width,int height){
	initscr();
	refresh();
	raw();
	noecho();//取消回显
	keypad(stdscr,TRUE);//支持特殊键位
	main_win=newwin(height+2,width+2,0,0);
	wborder(main_win,'|','|','-','-','+','+','+','+');
	info_win=newwin(height+2,10,0,width+3);
	wborder(info_win,'|','|','-','-','+','+','+','+');
	wrefresh(main_win);
	wrefresh(info_win);
}

int print_snake()
int main(){
	init(10,5);
	getch();	
	endwin();

}
