#include "标头.h"

extern int getToken();
extern void advance();
extern void program();

FILE * fin;          //文件指针，操作源文件
int l;               //记录当前所在行
int r;               //记录当前所在列
int right;           //记录分析过程中有无出错，初值为1，一旦出错置0

void main(){
	l=1;
	r=0;
	right=1;
	char scanin[256];
	string buffer;
	char temp;
	printf("请输入源程序文件名（包括路径）：");
	scanf_s("%s", scanin,255);
	if(fopen_s(&fin, scanin, "r")){
		printf("打开源程序文件出错!\n");
		system("pause");
		return;
	}
	while((temp=fgetc(fin))!=EOF){
		buffer+=temp;
	}
	fseek(fin,0,0);
	printf("\n源程序文件内容：\n%s\n",buffer.c_str());
	advance();
	program();
	if(right==1){
		printf("\n分析成功！\n");
	}
	else{
		printf("\n分析失败！\n");
	}
	fclose(fin);
	printf("\n");
	system("pause");
}