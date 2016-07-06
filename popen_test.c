#include<stdio.h>
#include<unistd.h>


int main(){
	FILE* fp1=popen("echo $LINES","r");
	FILE* fp2=popen("echo $COLUMNS","r");
	printf("%p",fp1);
	printf("%p",fp2);
	sleep(5);
	char i[10]={"99"};
	char j[10]={"44"};;
	int ii=11,jj=11;
	fread(i,sizeof(char),10,fp1);
	fread(j,sizeof(char),10,fp2);
	int k;
	for (k=0;k<10;k++){
		printf("%d\n",(int)i[k]);
				}
	sscanf(i,"%d",&ii);
	sscanf(j,"%d",&jj);
	
	printf("%d\n%d\n",ii,jj);
	pclose(fp1)
		;
	pclose(fp2);
}
