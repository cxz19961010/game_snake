#include<stdio.h>
#include<linux/input.h>
#include<fcntl.h>
#include<termios.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/types.h>

#define ECHOFLAGS (ECHO|ECHONL|ICANON) 

//���溯��������ʼ��������0�����ɹ�
int readkey_init(){//��Ϊ��������һ������Ϊ�ǻ��ԣ��ڶ�������Ϊ������
			//tcgetattr()���ڸ����ն˵���Ӧ������
			//fcntl()���ڸı��ļ�����
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
	//���漸�������˷ǻ���
	
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
	}//�����������÷�����
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
	//���漸�������˻���
	
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
	}//����������������
	return 0;

}

int readkey(){//����-1��ʾ���ַ���ȡ�����򷵻��ַ�ascii��
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
