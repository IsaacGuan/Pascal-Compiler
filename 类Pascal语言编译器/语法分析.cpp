#include "标头.h"

extern int getToken();
extern int isin_identifiers(string s, int stype);
extern void enter_identifiers(string name, int type);
extern void enter_paranum(int i, int paranum);
extern int compare_paramum(int i, int n);
extern void enter_quaternaries(int nxq, string op, string arg1, string arg2, string result);
extern void print_quaternaries();

extern FILE * fin;
extern string token;
extern int l;
extern int r;
extern int right;
extern idtab identifiers[MAX];

int lookahead;          //存储当前单词类型
int N=0;                //四元式序号
int temp=0;             //临时变量序号
int count;              //参数个数

/*
文法如下：
01. <program> -> <process> [ <process> ]
02. <process> -> sub id <parameterlist> <statement> end sub
03. <parameterlist> -> ( id as <datatype> [ , id as <datatype> ] )
                    -> ε
04. <statement> -> <branch>
                -> <assignment>
			    -> <loop>
			    -> <input>
			    -> <output>
			    -> <callfunction>
			    -> <variabledefinition>
			    -> <compound>
			    -> ε
05. <compound> -> begin <statement> [ , <statement> ] end
06. <variabledefinition> -> var id [ , id ] as <datatype> ;
07. <datatype> -> integer
               -> float
08. <input> -> input id [ , id ] ;
09. <output> -> output <expression> [ , <expression> ] ;
10. <assignment> -> id = <expression> ;
11. <callfunction> -> call id ( <passparameter> ) ;
12. <passparameter> -> id [ , id ]
                    -> ε
13. <branch> -> if <booleanexpression> then <statement>
             -> if <booleanexpression> then <statement> else <statement>
14. <loop> -> while <booleanexpression> do <statement>
15. <expression> -> <term> [ + | - <term> ]
16. <term> -> <factor> [ * | / <factor> ]
17. <factor> -> id | con | deci | ( <expression> )
18. <booleanexpression> -> <relationexpression> [ and | or <booleanexpression> ]
19. <relationexpression> -> <expression> <relation> <expression>
20. <relation> -> < | <= | > | >= | == | <>
*/

char *errormsg[]={                                                 //报错信息
	"程序尾部出现多余单词！",                                      //＃:0
	"缺少标识符，或标识符拼写错误！",                              //id:1
	"缺少整数，或整数拼写错误！",                                  //integer:2
	"缺少小数，或小数拼写错误！",                                  //decimal:3
	"缺少+或-，或+或-拼写错误！",                                  //plus:4
	"缺少+或-，或+或-拼写错误！",                                  //minus:5
	"缺少*或/，或*或/拼写错误！",                                  //multiply:6
	"缺少*或/，或*或/拼写错误！",                                  //divide:7
	"缺少=，或=拼写错误！",                                        //assign:8
	"缺少>或<或>=或<=或==或<>，或>或<或>=或<=或==或<>拼写错误！",  //gthan:9
	"缺少>或<或>=或<=或==或<>，或>或<或>=或<=或==或<>拼写错误！",  //lthan:10
	"缺少>或<或>=或<=或==或<>，或>或<或>=或<=或==或<>拼写错误！",  //egthan:11
	"缺少>或<或>=或<=或==或<>，或>或<或>=或<=或==或<>拼写错误！",  //elthan:12
	"缺少>或<或>=或<=或==或<>，或>或<或>=或<=或==或<>拼写错误！",  //unequal:13
	"缺少>或<或>=或<=或==或<>，或>或<或>=或<=或==或<>拼写错误！",  //equal:14
	"缺少and或or，或and或or拼写错误！",                            //and:15
	"缺少and或or，或and或or拼写错误！",                            //or:16
	"缺少(，或(拼写错误！",                                        //LP:17
	"缺少)，或)拼写错误！",                                        //RP:18
	"缺少，或，拼写错误！",                                        //comma:19
	"缺少；或；拼写错误！",                                        //semi:20
	"缺少begin，或begin拼写错误！",                                //keybegin:21
	"缺少as，或as拼写错误！",                                      //keyas:22
	"缺少sub，或sub拼写错误！",                                    //keysub:23
	"缺少end，或end拼写错误！",                                    //keyend:24
	"缺少if，或if拼写错误！",                                      //keyif:25
	"缺少then，或then拼写错误！",                                  //keythen:26
	"缺少else，或else拼写错误！",                                  //keyelse:27
	"缺少call，或call拼写错误！",                                  //keycall:28
	"缺少while，或while拼写错误！",                                //keywhile:29
	"缺少do，或do拼写错误！",                                      //keydo:30
	"缺少integer或float，或integer或float拼写错误！",              //keyinteger:31
	"缺少integer或float，或integer或float拼写错误！",              //keyfloat:32
	"缺少input，或input拼写错误！",                                //keyinput:33
	"缺少output，或output拼写错误！",                              //keyoutput:34
	"缺少var，或var拼写错误！"                                     //keyvar:35
};

void program();               //程序
void process();               //过程
void parameterlist();         //参数表
void statement();             //语句
void datatype();              //数据类型
void branch();                //分支语句
void assignment();            //赋值语句
void loop();                  //循环语句
void input();                 //输入语句
void output();                //输出语句
void callfunction();          //函数调用语句
void variabledefinition();    //变量定义语句
void compound();              //复合语句
string expression();          //表达式
void passparameter();         //传递参数
string booleanexpression();   //布尔表达式
string term();                //项
string factor();              //因子
string relationexpression();  //关系表达式
int relation();               //关系

void advance(){          //将下一个单词的单词类型存入lookahead
	lookahead=getToken();
}

void error(int n){          //打印缺少n类单词的错误信息
	printf("\n语法分析出错！\n错误位置：第%d行，第%d个单词。\n",l,r);
	printf("错误原因：%s\n分析失败！\n",errormsg[n]);
	fclose(fin);
	printf("\n");
	system("pause");
	exit(0);
}

void match(int t){          //判断当前单词类型是否与t相等，若不等则报错
	if(lookahead!=t) {
		error(t);
		right=0;
	}
}

/*
01. <program> -> <process> [ <process> ]
*/
void program(){
	process();
	while(lookahead==keysub){
		process();
	}
	match(＃);
	if(right==1){
		printf("\n打印四元式如下：\n");
		print_quaternaries();
	}
}

/*
02. <process> -> sub id <parameterlist> <statement> end sub
*/
void process(){
	match(keysub);
	advance();
	match(id);
	string recentname=token;
	if(!isin_identifiers(token, processname)){
		enter_identifiers(token, processname);
	}
	else{
		right=0;
		printf("\n第%d行，第%d个单词：%s已定义。不能重复定义！",l,r,token.c_str());
	}
	advance();
	count=0;
	parameterlist();
	enter_paranum(isin_identifiers(recentname,processname)-1,count);
	statement();
	match(keyend);
	advance();
	match(keysub);
	advance();
}

/*
03. <parameterlist> -> ( id as <datatype> [ , id as <datatype> ] )
                    -> ε
*/
void parameterlist(){
	if(lookahead==LP){
		advance();
		match(id);
		if(!isin_identifiers(token, variablename)){
			enter_identifiers(token, variablename);
		}
		else{
			right=0;
			printf("\n第%d行，第%d个单词：%s已定义。不能重复定义！",l,r,token.c_str());
		}
		count++;
		advance();
		match(keyas);
		advance();
		datatype();
		while(lookahead==comma){
			advance();
			match(id);
			if(!isin_identifiers(token, variablename)){
				enter_identifiers(token, variablename);
			}
			else{
				right=0;
				printf("\n第%d行，第%d个单词：%s已定义。不能重复定义！",l,r,token.c_str());
			}
			count++;
			advance();
			match(keyas);
			advance();
			datatype();
		}
		match(RP);
		advance();
	}
}

/*
04. <statement> -> <branch>
                -> <assignment>
			    -> <loop>
			    -> <input>
			    -> <output>
			    -> <callfunction>
			    -> <variabledefinition>
			    -> <compound>
			    -> ε
*/
void statement(){
	if(lookahead==keyif){
		branch();
	}
	else if(lookahead==id){
		assignment();
	}
	else if(lookahead==keywhile){
		loop();
	}
	else if(lookahead==keyinput){
		input();
	}
	else if(lookahead==keyoutput){
		output();
	}
	else if(lookahead==keycall){
		callfunction();
	}
	else if(lookahead==keyvar){
		variabledefinition();
	}
	else if(lookahead==keybegin){
		compound();
	}
}

/*
05. <compound> -> begin <statement> [ , <statement> ] end
*/
void compound(){
	match(keybegin);
	advance();
	statement();
	while(lookahead==comma){
		advance();
		statement();
	}
	match(keyend);
	advance();
}

/*
06. <variabledefinition> -> var id [ , id ] as <datatype> ;
*/
void variabledefinition(){
	match(keyvar);
	advance();
	match(id);
	if(!isin_identifiers(token, variablename)){
		enter_identifiers(token, variablename);
	}
	else{
		right=0;
		printf("\n第%d行，第%d个单词：%s已定义。不能重复定义！",l,r,token.c_str());
	}
	advance();
	while(lookahead==comma){
		advance();
		match(id);
		if(!isin_identifiers(token, variablename)){
			enter_identifiers(token, variablename);
		}
		else{
			right=0;
			printf("\n第%d行，第%d个单词：%s已定义。不能重复定义！",l,r,token.c_str());
		}
		advance();
	}
	match(keyas);
	advance();
	datatype();
	match(semi);
	advance();
}

/*
07. <datatype> -> integer
               -> float
*/
void datatype(){
	if(lookahead==keyinteger){
		advance();
	}
	else{
		match(keyfloat);
		advance();
	}
}

/*
08. <input> -> input id [ , id ] ;
*/
void input(){
	match(keyinput);
	advance();
	match(id);
	if(!isin_identifiers(token, variablename)){
		right=0;
		printf("\n第%d行，第%d个单词：%s未定义！",l,r,token.c_str());
	}
	advance();
	while(lookahead==comma){
		advance();
		match(id);
		if(!isin_identifiers(token, variablename)){
			right=0;
			printf("\n第%d行，第%d个单词：%s未定义！",l,r,token.c_str());
		}
		advance();
	}
	match(semi);
	advance();
}

/*
09. <output> -> output <expression> [ , <expression> ] ;
*/
void output(){
	match(keyoutput);
	advance();
	expression();
	while(lookahead==comma){
		advance();
		expression();
	}
	match(semi);
	advance();
}

/*
10. <assignment> -> id = <expression> ;
*/
void assignment(){
	string v1,v2;
	match(id);
	v1=token;
	if(!isin_identifiers(token, variablename)){
		right=0;
		printf("\n第%d行，第%d个单词：%s未定义！",l,r,token.c_str());
	}
	advance();
	match(assign);
	advance();
	v2=expression();
	match(semi);
	enter_quaternaries(N++,"=",v2,"",v1);
	//printf("%3d  ( =,\t%s,\t,\t%s )\n",N++,v2.c_str(),v1.c_str());
	advance();
}

/*
11. <callfunction> -> call id ( <passparameter> ) ;
*/
void callfunction(){
	match(keycall);
	advance();
	match(id);
	string recentname=token;
	int recentline=l;
	int recentrow=r;
	if(!isin_identifiers(token, processname)){
		right=0;
		printf("\n第%d行，第%d个单词：%s未定义！",l,r,token.c_str());
	}
	advance();
	match(LP);
	advance();
	count=0;
	passparameter();
	if(!compare_paramum(isin_identifiers(recentname,processname)-1,count)){
		right=0;
		printf("\n第%d行，第%d个单词：函数%s实参与形参的数目不一致！",l,r,recentname.c_str());
	}
	match(RP);
	advance();
	match(semi);
	advance();
}

/*
12. <passparameter> -> id [ , id ]
                    -> ε
*/
void passparameter(){
	if(lookahead==id){
		if(!isin_identifiers(token, variablename)){
			right=0;
			printf("\n第%d行，第%d个单词：%s未定义！",l,r,token.c_str());
		}
		count++;
		advance();
		while(lookahead==comma){
			advance();
			match(id);
			if(!isin_identifiers(token, variablename)){
				right=0;
				printf("\n第%d行，第%d个单词：%s未定义！",l,r,token.c_str());
			}
			count++;
			advance();
		}
	}
}

/*
13. <branch> -> if <booleanexpression> then <statement>
             -> if <booleanexpression> then <statement> else <statement>
*/
void branch(){
	string v,r;
	match(keyif);
	advance();
	v=booleanexpression();
	match(keythen);
	char tch[256];
	sprintf(tch,"%d",N+2);
	r=tch;
	enter_quaternaries(N++,"jnz",v,"",r);
	int currentnxq1=N;
	enter_quaternaries(N++,"j","","","");	
	//printf("%3d  ( jnz,\t%s,\t,\t%d )\n",N++,v.c_str(),N+1);
	//printf("%3d  ( j,\t,\t,\telse )\n",N++);
	advance();
	statement();
	int currentnxq2=N;
	enter_quaternaries(N++,"j","","","");
	sprintf(tch,"%d",N);
	r=tch;
	enter_quaternaries(currentnxq1,"j","","",r);
	if(lookahead==keyelse){
		advance();
		statement();
	}
	sprintf(tch,"%d",N);
	r=tch;
	enter_quaternaries(currentnxq2,"j","","",r);
}

/*
14. <loop> -> while <booleanexpression> do <statement>
*/
void loop(){
	string v,r;
	match(keywhile);
	advance();
	int currentnxq1=N;
	v=booleanexpression();
	match(keydo);
	char tch[256];
	sprintf(tch,"%d",N+2);
	r=tch;
	enter_quaternaries(N++,"jnz",v,"",r);
	int currentnxq2=N;
	enter_quaternaries(N++,"j","","","");	
	//printf("%3d  ( jnz,\t%s,\t,\t%d )\n",N++,v.c_str(),N+1);
	//printf("%3d  ( j,\t,\t,\tendwhile )\n",N++);
	advance();
	statement();
	sprintf(tch,"%d",currentnxq1);
	r=tch;
	enter_quaternaries(N++,"j","","",r);
	sprintf(tch,"%d",N);
	r=tch;
	enter_quaternaries(currentnxq2,"j","","",r);
}

/*
15. <expression> -> <term> [ + | - <term> ]
*/
string expression(){
	string v1,v2,r;
	int op;
	v1=term();
	while(lookahead==plus||lookahead==minus){
		op=lookahead;
		advance();
		v2=term();
		char tch[256];
		sprintf(tch,"t%d",temp);
		r=tch;
		enter_quaternaries(N++,op==plus?"+":"-",v1,v2,r);
		//printf("%3d  ( %c,\t%s,\t%s,\tt%d )\n",N++,op==plus?'+':'-',v1.c_str(),v2.c_str(),temp);		
		sprintf(tch,"t%d",temp++);
		v1=tch;
	}
	return v1;
}

/*
16. <term> -> <factor> [ * | / <factor> ]
*/
string term(){
	string v1,v2,r;
	int op;
	v1=factor();
	while(lookahead==mutiply||lookahead==divide){
		op=lookahead;
		advance();
		v2=factor();
		char tch[256];
		sprintf(tch,"t%d",temp);
		r=tch;
		enter_quaternaries(N++,op==mutiply?"*":"/",v1,v2,r);
		//printf("%3d  ( %c,\t%s,\t%s,\tt%d )\n",N++,op==mutiply?'*':'/',v1.c_str(),v2.c_str(),temp);
		sprintf(tch,"t%d",temp++);
		v1=tch;
	}
	return v1;
}

/*
17. <factor> -> id | con | deci | ( <expression> )
*/
string factor(){
	string v;
	if(lookahead==id){
		if(!isin_identifiers(token, variablename)){
			right=0;
			printf("\n第%d行，第%d个单词：%s未定义！",l,r,token.c_str());
		}
		v=token;
		advance();
	}
	else if(lookahead==integer){
		v=token;
		advance();
	}
	else if(lookahead==decimal){
		v=token;
		advance();
	}
	else{
		match(LP);
		advance();
		v=expression();
		match(RP);
		advance();
	}
	return v;
}

/*
18. <booleanexpression> -> <relationexpression> [ and | or <booleanexpression> ]
*/
string booleanexpression(){
	string v1,v2,r;
	int op;
	v1=relationexpression();
	while(lookahead==and||lookahead==or){
		op=lookahead;
		advance();
		v2=booleanexpression();
		char tch[256];
		sprintf(tch,"t%d",temp);
		r=tch;
		enter_quaternaries(N++,op==and?"and":"or",v1,v2,r);
		//printf("%3d  ( %s,\t%s,\t%s,\tt%d )\n",N++,op==and?"and":"or",v1.c_str(),v2.c_str(),temp);
		sprintf(tch,"t%d",temp++);
		v1=tch;
	}
	return v1;
}

/*
19. <relationexpression> -> <expression> <relation> <expression>
*/
string relationexpression(){
	string v1,v2,r;
	int op;
	v1=expression();
	op=relation();
	v2=expression();
	char tch[256];
	sprintf(tch,"t%d",temp);
	r=tch;
	enter_quaternaries(N++,op==lthan?"<":op==elthan?"<=":op==gthan?">":op==egthan?">=":op==equal?"==":"<>",v1,v2,r);
	//printf("%3d  ( %s,\t%s,\t%s,\tt%d )\n",N++,op==lthan?"<":op==elthan?"<=":op==gthan?">":op==egthan?">=":op==equal?"==":"<>",v1.c_str(),v2.c_str(),temp);
	sprintf(tch,"t%d",temp++);
	v1=tch;
	return v1;
}

/*
20. <relation> -> < | <= | > | >= | == | <>
*/
int relation(){
	int op;
	if(lookahead==lthan){
		op=lookahead;
		advance();
	}
	else if(lookahead==elthan){
		op=lookahead;
		advance();
	}
	else if(lookahead==gthan){
		op=lookahead;
		advance();
	}
	else if(lookahead==egthan){
		op=lookahead;
		advance();
	}
	else if(lookahead==equal){
		op=lookahead;
		advance();
	}
	else{
		match(unequal);
		op=lookahead;
		advance();
	}
	return op;
}