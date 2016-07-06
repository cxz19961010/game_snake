#include<stdio.h>
#include<linux/input.h>
#include<fcntl.h>
#include<termios.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/types.h>

#define ECHOFLAGS (ECHO|ECHONL|ICANON) 

//下面函数用来初始化，返回0表明成功
int readkey_init(){//分为两步，第一步设置为非回显，第二步设置为非阻塞
			//tcgetattr()用于更改终端的相应的设置
			//fcntl()用于改变文件设置
	struct termios settings;
	if(0!=tcgetattr(STDIN_FILENO,&settings))
	{
		//printf("error:reading stdin_setting 1");
		exit(1);
	}
	settings.c_lflag&=~ECHOFLAGS;
	if(0!=tcsetattr(STDIN_FILENO,TCSAFLUSH,&settings))
	{
		//printf("error:setting stdin_setting 2");
		exit(2);
	}
	//上面几步设置了非回显
	
	int flag;
	if(-1==(flag=fcntl(STDIN_FILENO,F_GETFL)))
	{
		//printf("error:reading stdin_setting 1");
		exit(3);
	}
	flag|=O_NONBLOCK;
	if(fcntl(STDIN_FILENO,F_SETFL,flag)==-1)
	{
		//printf("error:setting stdin_setting 2");
		exit(4);
	}//以上用来设置非阻塞
	return 0;

}
int readkey_stop(){

	struct termios settings;
	if(0!=tcgetattr(STDIN_FILENO,&settings))
	{
		//printf("error:reading stdin_setting 1");
		exit(1);
	}
	settings.c_lflag|=ECHOFLAGS;
	if(0!=tcsetattr(STDIN_FILENO,TCSAFLUSH,&settings))
	{
		//printf("error:setting stdin_setting 2");
		exit(2);
	}
	//上面几步设置了回显
	
	int flag;
	if(-1==(flag=fcntl(STDIN_FILENO,F_GETFL)))
	{
		//printf("error:reading stdin_setting 1");
		exit(3);
	}
	flag&=~O_NONBLOCK;
	if(fcntl(STDIN_FILENO,F_SETFL,flag)==-1)
	{
		//printf("error:setting stdin_setting 2");
		exit(4);
	}//以上用来设置阻塞
	return 0;

}

int readkey(){//返回-1表示无字符读取，否则返回字符ascii码
	int ch;
	ch=getchar();
	return ch;
}


struct{
	int (*init)(void);
	int (*readkey)(void);
	int (*stop)(void);
}input_nonblock={readkey_init,readkey,readkey_stop};
	

	
	
int main()
{
	int se;
	if(0!=(se=(*input_nonblock.init)()))
	{
		printf("error:init	%d\n",se);
	}
	while(1){
		int ch=	(*input_nonblock.readkey)();
		if(ch=='-')
			break;
		if(ch!=-1)
			printf("character:%c\n",ch);
	}
	if(0!=(se=(*input_nonblock.stop)()))
	{
		printf("error:stop	%d\n",se);
	}
	return 0;
}
