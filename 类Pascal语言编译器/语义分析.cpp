#include "��ͷ.h"

idtab identifiers[MAX];               //���ű�
intertable quaternaries[MAX];         //��Ԫʽ��

extern int N;
extern int count;

int isin_identifiers(string name, int type){          //�ж�����Ϊname����Ϊtype�ķ����ڲ��ڷ��ű��ڣ������򷵻ظ÷����ڱ��ڵ�λ��i+1�����򷵻�0
	int i;
	for(i=0;i<MAX;i++){
		if(!identifiers[i].used){
			break;
		}
		if((!identifiers[i].name.compare(name))&&(identifiers[i].type==type)){
			return i+1;
		}
	}
	return 0;
}

void enter_identifiers(string name, int type){          //�ڷ��ű�β�˼����µķ���
	int i;
	for(i=0;i<MAX;i++){
		if(!identifiers[i].used){
			break;
		}
	}
	identifiers[i].name=name;
	identifiers[i].used=1;
	identifiers[i].type=type;
	identifiers[i].paranum=0;
}

void enter_paranum(int i, int paranum){          //�����i�����ŵĲ�������
	identifiers[i].paranum=paranum;
}

int compare_paramum(int i, int n){          //����i�����ŵĲ���������n�Ƚϣ��������򷵻�1�����򷵻�0
	return n==identifiers[i].paranum?1:0;
}

void enter_quaternaries(int nxq, string op, string arg1, string arg2, string result){          //��д��nxq����Ԫʽ
	quaternaries[nxq].nxq=nxq;
	quaternaries[nxq].op=op;
	quaternaries[nxq].arg1=arg1;
	quaternaries[nxq].arg2=arg2;
	quaternaries[nxq].result=result;
}

void print_quaternaries(){          //��ӡ��Ԫʽ��
	int i=0;
	printf("%3d  ( %s,\t%s,\t%s,\t%s )\n", quaternaries[i].nxq,quaternaries[i].op.c_str(),quaternaries[i].arg1.c_str(),quaternaries[i].arg2.c_str(),quaternaries[i].result.c_str());
	for(i=1;i<N;i++){
		printf("%3d  ( %s,\t%s,\t%s,\t%s )\n", quaternaries[i].nxq,quaternaries[i].op.c_str(),quaternaries[i].arg1.c_str(),quaternaries[i].arg2.c_str(),quaternaries[i].result.c_str());	
	}
}