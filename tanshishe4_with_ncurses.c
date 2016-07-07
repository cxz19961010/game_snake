#include<stdio.h>
#include<stdlib.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<fcntl.h>
#include<united.h>
#include<termios.h>
#include<time.h>
#include<malloc.h>

#define ECHOFLAGS (ECHO|ECHONL|ICANON)

#define COLOR_SNAKE1 1
#define COLOR_SNAKE2 2
#define COLOR_BOX 3




int readkey();


#define FOODS_NU_MAX 3
int game_begin();//������
int game_end();//�������
int game_createfood();
int game_pause();//���ڼ��������ٶȣ������Ѷ�
int game_judge();//�����ж���Ϸ״�������С�ʤ����ʧ��
struct {
	int level;
	int score;
	int high_score;
	long pause_time;//���ڼ�¼��Ϸÿ��ѭ��ʱ����
	int foods_nu;//ʳ������
	int foods_co[FOODS_NU_MAX][2];//ʳ������;
	int (*begin)();
	int (*end)();
	int (*createfood)();
	int (*pause)();
	int (*judge)();
}game_info={0,0,0,800000,1,0,0,0,0,0,0,game_begin,game_end,game_createfood,game_pause,game_judge};



int read_map(void*Self,char*p);
struct map{
	struct{
		int x,y;//��¼��ͼ�ĳ���
		char **map;
	};
	int (*read_map)(void *Self,char *p);
}map={10,5,NULL,read_map};


WINDOW *main_win,*info_win;//mainΪ��Ϸ�����棬infoΪ�÷ֽ���
int board_init();//��ʼ�����̣�ͨ���ļ���ȡ������Ϣ
int board_output();//�������̻��沢���
struct{
	int (*init)();
	int (*output)();
}board_info{board_init,board_output};


#define SNAKE_BEGINNING_LENGTH 10
int snake_init(void * Self);
int snake_ifeat(void * Self);
int snake_ifdie(void * Self);
int snake_move(void * Self);
int snake_getdir(void *Self);
int computer_search_food(int head[2],int food_nu,int *p_food_co);//ͨ����������㷨������·��������Ϊ��ͷλ�úͼ�¼ʳ��λ�õ�ָ��,����ֵΪǰ���ķ���(wasd)
struct snake_node{
	int position[2];//�ڵ������λ��
	struct snake_node *next;}//�ڵ����һ���ڵ��ָ��
struct snake{

	int head_begin[2];//����ָʾ��ͷ�ĳ�ʼλ��
	struct snake_node * self;//����ָ���Լ�;
	int length;//������
	struct snake_node * head;//ָ�������ͷָ��
	int (*ifeat)(void *self);
	int (*ifdie)(void *self);
	int (*move)(void *self);
	int (*getdir)(void *self);
	
	int (*search_food)(int head[2],int food_nu,int *p_food_co);

}player1={15,5,&player1,SNAKE_BEGINNING_LENGTH,NULL,snake_ifeat,snake_ifdie,snake_move,NULL,'d','d'},
computer{25,5,&computer,SNAKE_BEGINNING_LENGTH,NULL,snake_ifeat,snake_ifdie,snake_move,NULL,'d','d'};



int snake_init(void * Self){//��ʼ������,�ɹ�����0��ʧ�ܷ�����Ӧ������
//return 1 mallocʧ��
	struct snake * self=(struct snake*)Self;
	if(NULL==(self->head=(struct snake_node*)malloc(sizeof(struct snake_node))))
		return 1;
		self->head->position[0]=self->head_begin[0];
		self->head->position[1]=self->head_begin[1];
		self->head->next=NULL;
	struct snake_node *node, * pre_node=self->node;
	for(i=0;i<CSLENGTH-1;i++)
		{
		if(NULL==(node=(struct snake_node)malloc(sizeof(struct snake_node))))
			return 1;
		pre_nodenod->next=node;
		node->position[0]=(self->position[0])-1-i;
		node->position[1]=(self->position[1]);
		node->next=NULL;
		}

}

int snake_ifeat(void * Self)//return 0 �Ե�ʳ��
{
	struct snake * self=(struct snake*)Self;
	int i;
	for(i=0;i<game_info.foods_nu;i++)
	{

		if((self->head->position[0]==game_info.foods_co[i][0])&&
				(self->head->position[1]==game_info.foods_co[i][1]))
				return 1;
	}
	return 0;

}

int snake_ifdie(void *Self)
{
	struct snake * self=(struct snake*)Self;


}

int snake_move(void *Self){//����ֵΪ0˵���ɹ�,-1Ϊ�����ڴ�ʧ��
	struct snake * self=(struct snake*)Self;
	int dir=(*(self->getdir))(self);
	int ifeat=(*(self->ifeat))(self);
	int changex=0,changey=0;
	switch(dir){
		case 'w':
				changex=0,changey=-1;
				 break;
		case 's':

				changex=0,changey=1;
				 break;
		case 'a':
				changex=-1,changey=0;
				 break;
		
		case 'd':

				changex=1,changey=0;
				 break;
	}
	struct snake_node *head_new=NULL;
	if(NULL==(head_new=(struct snake_node*)malloc(sizeof(struct snake_node))))
		return -1;	
	head_new->next=self->head;
	head_new->position[0]=self->head->position[0]+changex;
	head_new->position[1]=self->head->position[1]+changey;
	self->head=head_new;	
	if(!ifeat){
		struct snake_node *node_pre,*node=self->head;
		while(node->next!=NULL)
		{
			node_pre=node;
			node=node->next;
			
		}
		free(node);
		node_pre->next=NULL;
	
		
	}
	return 0;
}




int broad_init(){
	int height=map.y;
	int	width=map.x;
	initscr();
	start_color();//��ʼ��������ɫ

	refresh();
	init_pair(COLOR_BOX,COLOR_WHITE,COLOR_BLACK);
	attron(COLOR_PAIR(COLOR_BOX));
	raw();
	noecho();//ȡ������
	keypad(stdscr,TRUE);//֧�������λ

	main_win=newwin(height+2,width+2,0,0);
	info_win=newwin(height+2,10,0,width+3);

	wattrset(main_win,COLOR_PAIR(COLOR_BOX));
	wattrset(info_win,COLOR_PAIR(COLOR_BOX));
	wborder(main_win,'|','|','-','-','+','+','+','+');
	wborder(info_win,'|','|','-','-','+','+','+','+');
	wattroff(main_win,COLOR_PAIR(COLOR_BOX));
	wattroff(info_win,COLOR_PAIR(COLOR_BOX));//���ư�ɫ����


	refresh();
	wrefresh(main_win);
	wrefresh(info_win);
	wattroff(main_win,COLOR_PAIR(COLOR_BOX));
	wattroff(info_win,COLOR_PAIR(COLOR_BOX));
}

int broad_output(){
	wclear(main_win);
	wclear(info_win);//�����Ļ

	init_pair(COLOR_BOX,COLOR_WHITE,COLOR_BLACK);
	init_pair(COLOR_SNAKE1,COLOR_RED,COLOR_RED);
	init_pair(COLOR_SNAKE2,COLOR_BLUE);

	wattrset(main_win,COLOR_PAIR(COLOR_BOX));
	wattrset(info_win,COLOR_PAIR(COLOR_BOX));
	wborder(main_win,'|','|','-','-','+','+','+','+');
	wborder(info_win,'|','|','-','-','+','+','+','+');
//���ư�ɫ����

	wattrset(main_win,COLOR_PAIR(COLOR_SNAKE1)|A_REVERSE);
	struct snake_node *node=player1.head;
	while(node!=NULL)
	{
		mvwaddch(main_win,node->position[1],node->position[0],' ');
		node=node->next;
	}//����player1��ͼ��

	wattrset(main_win,COLOR_PAIR(COLOR_SNAKE2)|A_REVERSE);
	node=player2.head;
	while(node!=NULL)
	{
		mvwaddch(main_win,node->position[1],node->position[0],' ');
		node=node->next;
	}//����player2��ͼ��

	refresh();
	wrefresh(main_win);
	wrefresh(info_win);//���������ͼ��
}


int read_map(void *Self ,char * p){//map�ĵ�һ��Ϊ��ͼ�ĳ����ڶ��п�ʼΪ��ͼ
	struct map* self=(struct map*)Self;
	FILE* fp=NULL;
	if(NULL==(fp=fopen(p,"r")))return 1;
	char line[40];
	fgets(line,40,fp);
	sscanf(line,"%d%d",&self->x,&self->y);
	if(NULL==(self->map=(char**)malloc(sizeof(char*)*(self->y))))
		return 2;
	int i;
	for(i=0;i<self->y;i++){
		if(NULL==((self->map[i])=	(char*)malloc(sizeof(char)*(self->x+1))))
			return 3;
		fgets(self->map[i],self->x+3,fp);
	}
	return 0;
}


