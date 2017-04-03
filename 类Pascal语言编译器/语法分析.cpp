#include "��ͷ.h"

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

int lookahead;          //�洢��ǰ��������
int N=0;                //��Ԫʽ���
int temp=0;             //��ʱ�������
int count;              //��������

/*
�ķ����£�
01. <program> -> <process> [ <process> ]
02. <process> -> sub id <parameterlist> <statement> end sub
03. <parameterlist> -> ( id as <datatype> [ , id as <datatype> ] )
                    -> ��
04. <statement> -> <branch>
                -> <assignment>
			    -> <loop>
			    -> <input>
			    -> <output>
			    -> <callfunction>
			    -> <variabledefinition>
			    -> <compound>
			    -> ��
05. <compound> -> begin <statement> [ , <statement> ] end
06. <variabledefinition> -> var id [ , id ] as <datatype> ;
07. <datatype> -> integer
               -> float
08. <input> -> input id [ , id ] ;
09. <output> -> output <expression> [ , <expression> ] ;
10. <assignment> -> id = <expression> ;
11. <callfunction> -> call id ( <passparameter> ) ;
12. <passparameter> -> id [ , id ]
                    -> ��
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

char *errormsg[]={                                                 //������Ϣ
	"����β�����ֶ��൥�ʣ�",                                      //��:0
	"ȱ�ٱ�ʶ�������ʶ��ƴд����",                              //id:1
	"ȱ��������������ƴд����",                                  //integer:2
	"ȱ��С������С��ƴд����",                                  //decimal:3
	"ȱ��+��-����+��-ƴд����",                                  //plus:4
	"ȱ��+��-����+��-ƴд����",                                  //minus:5
	"ȱ��*��/����*��/ƴд����",                                  //multiply:6
	"ȱ��*��/����*��/ƴд����",                                  //divide:7
	"ȱ��=����=ƴд����",                                        //assign:8
	"ȱ��>��<��>=��<=��==��<>����>��<��>=��<=��==��<>ƴд����",  //gthan:9
	"ȱ��>��<��>=��<=��==��<>����>��<��>=��<=��==��<>ƴд����",  //lthan:10
	"ȱ��>��<��>=��<=��==��<>����>��<��>=��<=��==��<>ƴд����",  //egthan:11
	"ȱ��>��<��>=��<=��==��<>����>��<��>=��<=��==��<>ƴд����",  //elthan:12
	"ȱ��>��<��>=��<=��==��<>����>��<��>=��<=��==��<>ƴд����",  //unequal:13
	"ȱ��>��<��>=��<=��==��<>����>��<��>=��<=��==��<>ƴд����",  //equal:14
	"ȱ��and��or����and��orƴд����",                            //and:15
	"ȱ��and��or����and��orƴд����",                            //or:16
	"ȱ��(����(ƴд����",                                        //LP:17
	"ȱ��)����)ƴд����",                                        //RP:18
	"ȱ�٣���ƴд����",                                        //comma:19
	"ȱ�٣���ƴд����",                                        //semi:20
	"ȱ��begin����beginƴд����",                                //keybegin:21
	"ȱ��as����asƴд����",                                      //keyas:22
	"ȱ��sub����subƴд����",                                    //keysub:23
	"ȱ��end����endƴд����",                                    //keyend:24
	"ȱ��if����ifƴд����",                                      //keyif:25
	"ȱ��then����thenƴд����",                                  //keythen:26
	"ȱ��else����elseƴд����",                                  //keyelse:27
	"ȱ��call����callƴд����",                                  //keycall:28
	"ȱ��while����whileƴд����",                                //keywhile:29
	"ȱ��do����doƴд����",                                      //keydo:30
	"ȱ��integer��float����integer��floatƴд����",              //keyinteger:31
	"ȱ��integer��float����integer��floatƴд����",              //keyfloat:32
	"ȱ��input����inputƴд����",                                //keyinput:33
	"ȱ��output����outputƴд����",                              //keyoutput:34
	"ȱ��var����varƴд����"                                     //keyvar:35
};

void program();               //����
void process();               //����
void parameterlist();         //������
void statement();             //���
void datatype();              //��������
void branch();                //��֧���
void assignment();            //��ֵ���
void loop();                  //ѭ�����
void input();                 //�������
void output();                //������
void callfunction();          //�����������
void variabledefinition();    //�����������
void compound();              //�������
string expression();          //���ʽ
void passparameter();         //���ݲ���
string booleanexpression();   //�������ʽ
string term();                //��
string factor();              //����
string relationexpression();  //��ϵ���ʽ
int relation();               //��ϵ

void advance(){          //����һ�����ʵĵ������ʹ���lookahead
	lookahead=getToken();
}

void error(int n){          //��ӡȱ��n�൥�ʵĴ�����Ϣ
	printf("\n�﷨��������\n����λ�ã���%d�У���%d�����ʡ�\n",l,r);
	printf("����ԭ��%s\n����ʧ�ܣ�\n",errormsg[n]);
	fclose(fin);
	printf("\n");
	system("pause");
	exit(0);
}

void match(int t){          //�жϵ�ǰ���������Ƿ���t��ȣ��������򱨴�
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
	match(��);
	if(right==1){
		printf("\n��ӡ��Ԫʽ���£�\n");
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
		printf("\n��%d�У���%d�����ʣ�%s�Ѷ��塣�����ظ����壡",l,r,token.c_str());
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
                    -> ��
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
			printf("\n��%d�У���%d�����ʣ�%s�Ѷ��塣�����ظ����壡",l,r,token.c_str());
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
				printf("\n��%d�У���%d�����ʣ�%s�Ѷ��塣�����ظ����壡",l,r,token.c_str());
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
			    -> ��
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
		printf("\n��%d�У���%d�����ʣ�%s�Ѷ��塣�����ظ����壡",l,r,token.c_str());
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
			printf("\n��%d�У���%d�����ʣ�%s�Ѷ��塣�����ظ����壡",l,r,token.c_str());
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
		printf("\n��%d�У���%d�����ʣ�%sδ���壡",l,r,token.c_str());
	}
	advance();
	while(lookahead==comma){
		advance();
		match(id);
		if(!isin_identifiers(token, variablename)){
			right=0;
			printf("\n��%d�У���%d�����ʣ�%sδ���壡",l,r,token.c_str());
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
		printf("\n��%d�У���%d�����ʣ�%sδ���壡",l,r,token.c_str());
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
		printf("\n��%d�У���%d�����ʣ�%sδ���壡",l,r,token.c_str());
	}
	advance();
	match(LP);
	advance();
	count=0;
	passparameter();
	if(!compare_paramum(isin_identifiers(recentname,processname)-1,count)){
		right=0;
		printf("\n��%d�У���%d�����ʣ�����%sʵ�����βε���Ŀ��һ�£�",l,r,recentname.c_str());
	}
	match(RP);
	advance();
	match(semi);
	advance();
}

/*
12. <passparameter> -> id [ , id ]
                    -> ��
*/
void passparameter(){
	if(lookahead==id){
		if(!isin_identifiers(token, variablename)){
			right=0;
			printf("\n��%d�У���%d�����ʣ�%sδ���壡",l,r,token.c_str());
		}
		count++;
		advance();
		while(lookahead==comma){
			advance();
			match(id);
			if(!isin_identifiers(token, variablename)){
				right=0;
				printf("\n��%d�У���%d�����ʣ�%sδ���壡",l,r,token.c_str());
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
			printf("\n��%d�У���%d�����ʣ�%sδ���壡",l,r,token.c_str());
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