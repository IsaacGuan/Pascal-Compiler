#include "标头.h"

idtab identifiers[MAX];               //符号表
intertable quaternaries[MAX];         //四元式表

extern int N;
extern int count;

int isin_identifiers(string name, int type){          //判断名称为name类型为type的符号在不在符号表内，若在则返回该符号在表内的位置i+1，否则返回0
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

void enter_identifiers(string name, int type){          //在符号表尾端加入新的符号
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

void enter_paranum(int i, int paranum){          //加入第i个符号的参数个数
	identifiers[i].paranum=paranum;
}

int compare_paramum(int i, int n){          //将第i个符号的参数个数和n比较，若等于则返回1，否则返回0
	return n==identifiers[i].paranum?1:0;
}

void enter_quaternaries(int nxq, string op, string arg1, string arg2, string result){          //填写第nxq个四元式
	quaternaries[nxq].nxq=nxq;
	quaternaries[nxq].op=op;
	quaternaries[nxq].arg1=arg1;
	quaternaries[nxq].arg2=arg2;
	quaternaries[nxq].result=result;
}

void print_quaternaries(){          //打印四元式表
	int i=0;
	printf("%3d  ( %s,\t%s,\t%s,\t%s )\n", quaternaries[i].nxq,quaternaries[i].op.c_str(),quaternaries[i].arg1.c_str(),quaternaries[i].arg2.c_str(),quaternaries[i].result.c_str());
	for(i=1;i<N;i++){
		printf("%3d  ( %s,\t%s,\t%s,\t%s )\n", quaternaries[i].nxq,quaternaries[i].op.c_str(),quaternaries[i].arg1.c_str(),quaternaries[i].arg2.c_str(),quaternaries[i].result.c_str());	
	}
}