#include <stdio.h>
#include <iostream>
using std:: string;

#define MAX 10000

#define ERROR   -1
#define ＃      0

#define id       1      //用户标识符
#define integer  2      //整数
#define decimal  3      //小数
#define plus     4      //加号
#define minus    5      //减号
#define mutiply  6      //乘号
#define divide   7      //除号
#define assign   8      //赋值
#define gthan    9      //大于
#define lthan    10     //小于
#define egthan   11     //大于等于
#define elthan   12     //小于等于
#define unequal  13     //不等于
#define equal    14     //等于
#define and      15     //与
#define or       16     //或
#define LP       17     //左括号
#define RP       18     //右括号
#define comma    29     //逗号
#define semi     20     //分号

#define keybegin     21    //保留字begin
#define keyas        22    //保留字as
#define keysub       23    //保留字sub
#define keyend       24    //保留字end
#define keyif        25    //保留字if
#define keythen      26    //保留字then
#define keyelse      27    //保留字else
#define keycall      28    //保留字call
#define keywhile     29    //保留字while
#define keydo        30    //保留字do
#define keyinteger   31    //保留字interger
#define keyfloat     32    //保留字float
#define keyinput     33    //保留字input
#define keyoutput    34    //保留字output
#define keyvar       35    //保留字var

#define processname  100
#define variablename 101

typedef struct{          //符号表结构体
	string name;         //符号名称
	int used;            //判断当前结构体内有没有内容，有为1，无为0
	int type;            //符号类型，有两种值：processname函数名，variablename变量名
	int paranum;         //参数个数，初值为0
}idtab;

typedef struct{          //四元式表结构体
	int nxq;             //当前四元式编号
	string op;           //运算符
	string arg1;         //运算对象1
	string arg2;         //运算对象2
	string result;       //结果
}intertable;