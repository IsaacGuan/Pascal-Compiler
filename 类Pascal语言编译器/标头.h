#include <stdio.h>
#include <iostream>
using std:: string;

#define MAX 10000

#define ERROR   -1
#define ��      0

#define id       1      //�û���ʶ��
#define integer  2      //����
#define decimal  3      //С��
#define plus     4      //�Ӻ�
#define minus    5      //����
#define mutiply  6      //�˺�
#define divide   7      //����
#define assign   8      //��ֵ
#define gthan    9      //����
#define lthan    10     //С��
#define egthan   11     //���ڵ���
#define elthan   12     //С�ڵ���
#define unequal  13     //������
#define equal    14     //����
#define and      15     //��
#define or       16     //��
#define LP       17     //������
#define RP       18     //������
#define comma    29     //����
#define semi     20     //�ֺ�

#define keybegin     21    //������begin
#define keyas        22    //������as
#define keysub       23    //������sub
#define keyend       24    //������end
#define keyif        25    //������if
#define keythen      26    //������then
#define keyelse      27    //������else
#define keycall      28    //������call
#define keywhile     29    //������while
#define keydo        30    //������do
#define keyinteger   31    //������interger
#define keyfloat     32    //������float
#define keyinput     33    //������input
#define keyoutput    34    //������output
#define keyvar       35    //������var

#define processname  100
#define variablename 101

typedef struct{          //���ű�ṹ��
	string name;         //��������
	int used;            //�жϵ�ǰ�ṹ������û�����ݣ���Ϊ1����Ϊ0
	int type;            //�������ͣ�������ֵ��processname��������variablename������
	int paranum;         //������������ֵΪ0
}idtab;

typedef struct{          //��Ԫʽ��ṹ��
	int nxq;             //��ǰ��Ԫʽ���
	string op;           //�����
	string arg1;         //�������1
	string arg2;         //�������2
	string result;       //���
}intertable;