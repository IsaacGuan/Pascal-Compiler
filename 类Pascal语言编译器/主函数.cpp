#include "��ͷ.h"

extern int getToken();
extern void advance();
extern void program();

FILE * fin;          //�ļ�ָ�룬����Դ�ļ�
int l;               //��¼��ǰ������
int r;               //��¼��ǰ������
int right;           //��¼�������������޳�����ֵΪ1��һ��������0

void main(){
	l=1;
	r=0;
	right=1;
	char scanin[256];
	string buffer;
	char temp;
	printf("������Դ�����ļ���������·������");
	scanf_s("%s", scanin,255);
	if(fopen_s(&fin, scanin, "r")){
		printf("��Դ�����ļ�����!\n");
		system("pause");
		return;
	}
	while((temp=fgetc(fin))!=EOF){
		buffer+=temp;
	}
	fseek(fin,0,0);
	printf("\nԴ�����ļ����ݣ�\n%s\n",buffer.c_str());
	advance();
	program();
	if(right==1){
		printf("\n�����ɹ���\n");
	}
	else{
		printf("\n����ʧ�ܣ�\n");
	}
	fclose(fin);
	printf("\n");
	system("pause");
}