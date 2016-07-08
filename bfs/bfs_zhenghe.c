//这是实现无方向广度优先搜索的程序
#include<stdio.h>
#include<malloc.h>
#define DEBUG
#ifdef DEBUG
#define PRINT(i) printf("%d\n",i)
#endif

int read_map(void *Self,char *p);
struct map{
	
	struct{
		int x,y;//记录地图的长宽
		char **map;
	};
	int (*read_map)(void *Self,char *p);
}map={0,0,NULL,read_map};


struct queue_mem{
	int position[2];//分别对应x，y的值
	struct queue_mem * pre;
	struct queue_mem * que_pre;//指向队列前一个,注意入队在队尾进行，出队在队首进行
	int distance;
}*in,*out,*close;//记录最近的路线的指针

char** map_cp;

struct queue_mem* bfs(int width,int height, char **map,int a_x,int a_y,int b_x,int b_y){
//map 是指向地图的指针，width和height为地图宽高，a_x a_y b_x b_y 是要计算距离的两个点。
//返回值为指向最近的路线最末端节点的指针
//map_cp中'0'为可通过，'1'为边界或不可通过，
	
	if(NULL==(out=(struct queue_mem*)malloc(sizeof(struct queue_mem))))
		return NULL;
	if(NULL==(close=(struct queue_mem*)malloc(sizeof(struct queue_mem))))
		return NULL;

	close->distance=height*width+1;
	close->pre=NULL;

	in=out;
	in->position[0]=a_x;
	in->position[1]=a_y;
	in->que_pre=NULL;
	in->pre=NULL;
	in->distance=0;
	
	if(NULL==(map_cp=(char**)malloc(sizeof(char*)*(height+2))))
		return NULL;
	int i,j;


	for(i=0;i<height+2;i++)
	{
		if(NULL==(	map_cp[i]=(char*)malloc(sizeof(char)*(width+2))))
			return NULL;
	}

	PRINT(10);
	for(i=0;(i==0||i==height+1);i=i+height+1)		
	{
		for(j=0;j<width+2;j++)
		{
			map_cp[i][j]='1';
			map_cp[j][i]='1';
		}
		
	}//绘制出边框
	for(i=0;i<height;i++)
		for(j=0;j<width;j++)
		{
			if(map[i][j]=='0')
				map_cp[i+1][j+1]='0';
			else
				map_cp[i+1][j+1]='1';
		}
	//绘制出地图
	
	PRINT(11);
	long jj=0;
	while(out!=NULL)
	{
		int i=bfs_search(map_cp,out,b_x,b_y);
		out=out->que_pre;
		jj++;
	}
	printf("%ld\n",jj);
	return close;
	
}
	
int bfs_search(char**map_cp,struct queue_mem *p,int b_x,int b_y){//p指该
	int i,j;
	struct queue_mem *next;
/*	for(i=-1;i<2;i++){
		for(j=-1;j<<2;j++){
			if((map_cp[p->position[1]+1+i][p->position[0]+1+j]=='0')||(p->position[1]+1+i==b_y&&p->position[0]+1+j==b_x)){
*/
	int a[4][2]={{-1,0},{1,0},{0,-1},{0,1}};
	map_cp[p->position[1]+1][p->position[0]+1]='1';
	for(i=0;i<4;i++){

		if((map_cp[p->position[1]+1+a[i][1]][p->position[0]+1+a[i][0]]=='0')||(p->position[1]+1+a[i][1]==b_y&&p->position[0]+1+a[i][0]==b_x)){
	
			if(NULL==(next=(struct queue_mem*)malloc(sizeof(struct queue_mem))))
					return -1;
			

			next->position[1]=p->position[1]+a[i][1];
			next->position[0]=p->position[0]+a[i][0];
			next->pre=p;
	//		next=in->que_pre;
			next->que_pre=NULL;
			next->distance=p->distance+1;
			if(next->position[1]==b_y&&next->position[0]==b_x)
			{
				if(next->distance<close->distance){
					close=next;
		
		map_cp[p->position[1]+1+a[i][1]][p->position[0]+1+a[i][0]]='1';
					continue;
				}
			}
			in->que_pre=next;
			in=next;
			
			
			
			}
		map_cp[p->position[1]+1+a[i][1]][p->position[0]+1+a[i][0]]='1';
		}
	return 2;

}
int read_map(void *Self ,char * p){//map的第一行为地图的长宽，第二行开始为地图
	struct map* self=(struct map*)Self;
	FILE* fp=NULL;
	if(NULL==(fp=fopen(p,"r")))return 1;
	char line[100];
	fgets(line,100,fp);
	sscanf(line,"%d%d",&self->x,&self->y);
	if(NULL==(self->map=(char**)malloc(sizeof(char*)*(self->y))))
		return 2;
	int i;
	for(i=0;i<self->y;i++){
		if(NULL==((self->map[i])=	(char*)malloc(sizeof(char)*(self->x+3))))
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
		printf("%s",map.map[i]);
	
		
	}
	int a_x,a_y,b_x,b_y;
	sscanf(argv[2],"%d",&a_x);
	sscanf(argv[3],"%d",&a_y);
	sscanf(argv[4],"%d",&b_x);
	sscanf(argv[5],"%d",&b_y);

	struct queue_mem * node=bfs(map.x,map.y,map.map,a_x,a_y,b_x,b_y);
	if (node==NULL)
	{	printf("error");

		return 0;
	}
	while(node!=NULL){
		map.map[node->position[1]][node->position[0]]='3';
		node=node->pre;
	}
	for(i=0;i<map.y;i++){


		printf("%s",map.map[i]);
	}
	PRINT(close->distance);
	return 0;
}
		

