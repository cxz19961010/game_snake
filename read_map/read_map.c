#include<stdio.h>
#include<malloc.h>
//#include<ncurses.h>
//#include<stdlib.h>

int read_map(void *Self,char *p);
struct map{
	
	struct{
		int x,y;//记录地图的长宽
		char **map;
	};
	int (*read_map)(void *Self,char *p);
}map={0,0,NULL,read_map};
int read_map(void *Self ,char * p){//map的第一行为地图的长宽，第二行开始为地图
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
	

int main(int argc,char *argv[]){
	printf("ok\n");
	printf("%s\n",argv[1]);
	if((map.read_map)((void*)&map,argv[1])){
		printf("error");
		return 0;
		
	}
	int i;
	for(i=0;i<map.y;i++){
		printf("%s\n",map.map[i]);
	}
	return 0;
}
		
		
