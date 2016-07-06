#include <stdio.h>  
#include <linux/input.h>  
#include <stdlib.h>  
#include <sys/types.h>  
#include <sys/stat.h>  
#include <fcntl.h>  
#include <unistd.h> 
#include <time.h>
#include "termios.h"
//#define DEV_PATH "/dev/input/event4"   //difference is possible  
#define ECHOFLAGS (ECHO|ECHOE|ECHOK|ECHONL|ICANON)



int level=1;
int score=0;
int highscore=0;
//身体部分
#define CSLENGTH 10//初始长度
#define QPLENGTH 35//棋盘长度
int length=CSLENGTH; 
int food[2];
int randd=0;//用于扰乱随机数
long times=800000;//用于控制延时时间
struct body{
	int position[2];
	struct body* next;
};


		struct body* bodys;


int readkey();//读取键位信息,返回按键值

int initbodys(){//初始化身体


	bodys->position[0]=15;

	bodys->position[1]=5;
	int i=0;
	struct body *node;
	struct body *preofnod=bodys;
	for(i=0;i<CSLENGTH-1;i++)
		{
		node=(struct body*)malloc(sizeof(struct body));	
		preofnod->next=node;
		node->position[0]=15-1-i;
		node->position[1]=5;

		preofnod=node;
		}
	preofnod->next=NULL;
			
}

char prekey='d';//上一次的按键
char key='d';
int bodysgo(int deltail/*决定是否要删除蛇尾*/){//身体前进
	int ifnotopp=0;//用于判断两次案件是否是反方向;
	int changex=0,changey=0;
	
	switch (key){
		case 'w':ifnotopp+=1;
				 break;
		case 's':ifnotopp-=1;
				 break;
		case 'a':ifnotopp+=2;
				 break;
		
		case 'd':ifnotopp-=2;
				 break;
	}

	switch (prekey){
		case 'w':ifnotopp+=1;
				 break;
		case 's':ifnotopp-=1;
				 break;
		case 'a':ifnotopp+=2;
				 break;
		
		case 'd':ifnotopp-=2;
				 break;
	}
	if(!ifnotopp)
		key=prekey,prekey=key;
	else	
		prekey=key;//判断按键是否符合规范，不规范则舍弃(与上次方向相反）
	
	switch (key){
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
	
		struct body* node;	
		node=(struct body*)malloc(sizeof(struct body));	
		node->next=bodys;
		node->position[0]=bodys->position[0]+changex;
		node->position[1]=bodys->position[1]+changey;

		bodys=node;
		if(deltail==1){
			struct body* prenod;
			while(node->next!=NULL){
				prenod=node;node=node->next;
			}
			prenod->next=NULL;
			free(node);
		}//创建一个新的头并把尾删除,如果不需要删除则不删
	


}

int createfood(){//创造食物	
back:	srand((unsigned)time(NULL)+(randd++));
	food[0]=rand()%QPLENGTH;
	srand((unsigned)time(NULL)+(randd++));
	food[1]=rand()%QPLENGTH;
	struct body * node=bodys;
	while(node->next!=NULL){
		if(node->position[0]==food[0]&&node->position[1]==food[1])
			goto back;
		node=node->next;
	}
		if(node->position[0]==food[0]&&node->position[1]==food[1])
			goto back;
		node=node->next;
//判断食物是否出现在蛇身体上，是则重新进行	
	
	
}
int ifeat(){//判断是否吃食并处理//注意根据程序，在蛇身走动后要先进行判断是否死亡，再判断是否吃食，再产生下一步蛇身行走(在IFEAT中实现该步)
	if(bodys->position[0]==food[0]&&bodys->position[1]==food[1])
	{
		bodysgo(0);
		createfood();
		length++;
	}
	else
		bodysgo(1);

}
	
int ifdie(){//判断是否死亡并处理,是则返回1，否则返回0
	if(bodys->position[0]>=QPLENGTH||bodys->position[0]<0||bodys->position[1]>=QPLENGTH||bodys->position[1]<0)
		return 1;
	struct body* node=bodys->next;
	while(node->next!=NULL)
	{	if(bodys->position[0]==node->position[0]&&
		bodys->position[1]==node->position[1])
		return 1;
		node=node->next;
	}		
	
		if(bodys->position[0]==node->position[0]&&
			bodys->position[1]==node->position[1])
		return 1;

		return 0;
}





//棋盘部分
int qipan[QPLENGTH][QPLENGTH];


int renew(){  //更新棋盘
	int x=0,y=0;
	for(x=0;x<QPLENGTH;x++)
		for(y=0;y<QPLENGTH;y++)
			qipan[x][y]=0;
	struct body *node;

	qipan[food[0]][food[1]]=2;
	qipan[bodys->position[0]][bodys->position[1]]=3;//头部单独标示
	node=bodys->next;
	while(node->next!=NULL)
	{	
		qipan[node->position[0]][node->position[1]]=1;
		node=node->next;
	}

		qipan[node->position[0]][node->position[1]]=1;
	//标注蛇身位置；
	
	//标注食物位置

}

int output(){  //棋盘输出(可加分数）
	int x=0,y=0;
	for(x=0;x<QPLENGTH+2;x++)
		printf("%s","__");
	printf("\n");
	for(y=0;y<QPLENGTH;y++)
		{
			printf("%s","||");
			for(x=0;x<QPLENGTH;x++)
			{	if(qipan[x][y]==0)
					printf("%s","  ");
				else	if(qipan[x][y]==3)
					printf("%s","@ ");
				else	if(qipan[x][y]==1)
					printf("%s","o ");
				else	if(qipan[x][y]==2)
					printf("%s","$ ");
			}
			printf("%s","||");
			if(y==QPLENGTH/2-4)
				printf("                  SCORE :  %d",score);
			if(y==QPLENGTH/2-3)
				printf("             HIGH SCORE :  %d",highscore);
			if(y==QPLENGTH/2)
				printf("             Begin&Restrat[B]  Exit[X]" );
			printf("\n");
				
		}

	for(x=0;x<QPLENGTH+2;x++)
		printf("%s","==");
	printf("\n");
		


}

//等级部分

int scorepre=0;
int dealingoflevel(int ifdie){//处理和等级相关的事
	score=length-10;
	if(score>scorepre)
	times=times-times*50000/800000,scorepre=score;
	if(ifdie)
		if(score>highscore)
		highscore=score;
	
}


//操作部分
/*	int keys_fd;
	char ret[2];
	struct input_event t;
int  readkey(){//读取键位信息,返回按键值
	int keycode;
kk:	if(read(keys_fd,&t,sizeof(t))==sizeof(t))
	{	
		if(t.type=EV_KEY)
			if(t.value==0)
				keycode= t.code;
			
	}
	else
		return 0;
//	{	keycode=-1;
//		return 0;}//表示未读取到键位
	switch (keycode){
		case 17:key='w';
				break;
		case 32:key='d';
				break;
		case 30:key='a';
				break;
		case 31:key='s';
				break;
		default:key=prekey;
				goto kk;
	}
	
	
}*/

int readkeyinit(){
 struct termios new,old;
    int flag,i=0;;
    if(tcgetattr(STDIN_FILENO,&old)==-1)
        exit(1);
    new=old;
    new.c_lflag&=~ECHOFLAGS;
    if(tcsetattr(STDIN_FILENO,TCSAFLUSH,&new)==-1)
    {
        tcsetattr(STDIN_FILENO,TCSAFLUSH,&old);
        exit(1);
    }
    flag=fcntl(STDIN_FILENO,F_GETFL);
    flag|=O_NONBLOCK;
    if(fcntl(STDIN_FILENO,F_SETFL,flag)==-1)
        exit(1);}
int readkey(){
 	char j=0;
		j=getchar();
		char ch;while((ch = getchar()) != '\n' && ch != EOF);
        if(j=='w'||j=='a'||j=='s'||j=='d'||j=='b'||j=='x')
			key=j;
    
}

int bere(){//处理开局
	score=0;
	scorepre=0;
	length=10;
	times=800000;
	system("clear");
	food[0]=15;
	food[1]=5;
	renew;
	output();
	while(1){
	readkey();
	if(key=='x')
		return 1;
	else if(key=='b'){
	initbodys();
	createfood();
	return 0;
	}
	}
}
int end(){
	usleep(400000);
		system("clear");
		usleep(300000);
		output();
		usleep(300000);
		system("clear");
		usleep(300000);
		output();
		usleep(300000);
}
	
		
int main(){
	readkeyinit();
	bodys
		=(struct body*)malloc(sizeof(struct body));	
while(1){
	if(bere())
		return 0;
	key='d';
	while(1){
	usleep(times);
	//printf("\033[2J") ;//用于清屏
	system("clear");
	readkey();
	if(key=='b')
		break;
	if(key=='x')
		return 0;
		
	if(ifdie()==1){
		
		dealingoflevel(1);
		break;
	}
	ifeat();
	dealingoflevel(0);
	renew();
	output();}
	if(key!='b')
	end();
}
		
}
