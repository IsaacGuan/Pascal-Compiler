#include "标头.h"

extern FILE * fin;
extern int l;
extern int r;

string token;          //记录当前单词内容

//关键字表
const string keywordslist[15]={"begin","as","sub","end","if","then","else","call","while","do","integer","float","input","output","var"};
const int keywords[15]={keybegin,keyas,keysub,keyend,keyif,keythen,keyelse,keycall,keywhile,keydo,keyinteger,keyfloat,keyinput,keyoutput,keyvar};
//bool型变量and和or
const string booland="and";
const string boolor="or";

int getToken(){          //词法分析函数
	token="";
	int state=0;
	int tokeType=＃;          //词法分析返回的单词类型
	char ch=getc(fin);
	if(ch!=EOF){
		/*
		while(ch==' '||ch=='\n'||ch=='\t'){
			if(ch=='\n'){
				r=0;
				l=l+1;
			}
			ch=getc(fin);
		}
		*/
		while(ch=='/'||ch==' '||ch=='\n'||ch=='\t'){          //跳过注释、空格、回车、制表符
			if(ch=='/'){
				ch=getc(fin);
				if(ch=='/'){
					ch=getc(fin);
					while(ch!='\n'){
						ch=getc(fin);
					}
				}
				else if(ch=='*'){
					ch=getc(fin);
					while(ch!='/'){
						while(ch!='*'){
							if(ch=='\n'){
								r=0;
								l=l+1;
							}
							if(ch==EOF){
								return ＃;
							}
							ch=getc(fin);
						}
						ch=getc(fin);
					}
					ch=getc(fin);
				}
				else{
					fseek(fin,-1,1);
					ch='/';
					break;
				}
			}
			while(ch==' '||ch=='\n'||ch=='\t'){
				if(ch=='\n'){
					r=0;
					l=l+1;
				}
				ch=getc(fin);
			}
		}
		if(ch==EOF){
			return ＃;          //若遇到文件结尾，则返回界符＃
		}
		while(tokeType==＃){          //根据状态转换图的词法分析过程
			switch(state){
			case 0:
				switch(ch){
				case '$':case 'a':case 'b':case 'c':case 'd':case 'e':case 'f':case 'g':case 'h':case 'i':case 'j':case 'k':case 'l':
				case 'm':case 'n':case 'o':case 'p':case 'q':case 'r':case 's':case 't':case 'u':case 'v':case 'w':case 'x':case 'y':
				case 'z':case 'A':case 'B':case 'C':case 'D':case 'E':case 'F':case 'G':case 'H':case 'I':case 'J':case 'K':case 'L':
				case 'M':case 'N':case 'O':case 'P':case 'Q':case 'R':case 'S':case 'T':case 'U':case 'V':case 'W':case 'X':case 'Y':
				case 'Z':
					token+=ch;
					r=r+1;
					ch=getc(fin);
					state=1;
					break;
				case '0':case '1':case '2':case '3':case '4':case '5':case '6':case '7':case '8':case '9':
					token+=ch;
					r=r+1;
					ch=getc(fin);
					state=2;
					break;
				case '+':
					token+=ch;
					r=r+1;
					tokeType=plus;
					break;
				case '-':
					token+=ch;
					r=r+1;
					tokeType=minus;
					break;
				case '*':
					token+=ch;
					r=r+1;
					tokeType=mutiply;
					break;
				case '/':
					token+=ch;
					r=r+1;
					tokeType=divide;
					break;
				case '=':
					token+=ch;
					r=r+1;
					ch=getc(fin);
					state=3;
					break;
				case '<':
					token+=ch;
					r=r+1;
					ch=getc(fin);
					state=4;
					break;
				case '>':
					token+=ch;
					r=r+1;
					ch=getc(fin);
					state=5;
					break;
				case '(':
					token+=ch;
					r=r+1;
					tokeType=LP;
					break;
				case ')':
					token+=ch;
					r=r+1;
					tokeType=RP;
					break;
				case ',':
					token+=ch;
					r=r+1;
					tokeType=comma;
					break;
				case ';':
					token+=ch;
					r=r+1;
					tokeType=semi;
					break;
				default:
					token+=ch;
					r=r+1;
					tokeType=ERROR;
					break;
				}
				break;
			case 1:
				switch(ch){
				case '$':case 'a':case 'b':case 'c':case 'd':case 'e':case 'f':case 'g':case 'h':case 'i':case 'j':case 'k':case 'l':
				case 'm':case 'n':case 'o':case 'p':case 'q':case 'r':case 's':case 't':case 'u':case 'v':case 'w':case 'x':case 'y':
				case 'z':case 'A':case 'B':case 'C':case 'D':case 'E':case 'F':case 'G':case 'H':case 'I':case 'J':case 'K':case 'L':
				case 'M':case 'N':case 'O':case 'P':case 'Q':case 'R':case 'S':case 'T':case 'U':case 'V':case 'W':case 'X':case 'Y':
				case 'Z':case '0':case '1':case '2':case '3':case '4':case '5':case '6':case '7':case '8':case '9':
					token+=ch;
					ch=getc(fin);
					state=1;
					break;
				default:
					tokeType=id;
					for(int i=0;i<15;i++){
						if(!token.compare(keywordslist[i])){
							tokeType=keywords[i];
						}
					}
					if(!token.compare(booland)){
						tokeType=and;
					}
					if(!token.compare(boolor)){
						tokeType=or;
					}
					if(ch!=EOF){
						fseek(fin,-1,1);
					}
					break;
				}
				break;
			case 2:
				switch(ch){
				case '0':case '1':case '2':case '3':case '4':case '5':case '6':case '7':case '8':case '9':
					token+=ch;
					ch=getc(fin);
					state=2;
					break;
				case '.':
					token+=ch;
					ch=getc(fin);
					state=6;
					break;
				default:
					tokeType=integer;
					if(ch!=EOF){
						fseek(fin,-1,1);
					}
					break;
				}
				break;
			case 3:
				switch(ch){
				case '=':
					token+=ch;
					tokeType=equal;
					break;
				default:
					tokeType=assign;
					if(ch!=EOF){
						fseek(fin,-1,1);
					}
					break;
				}
				break;
			case 4:
				switch(ch){
				case '=':
					token+=ch;
					tokeType=elthan;
					break;
				case '>':
					token+=ch;
					tokeType=unequal;
					break;
				default:
					tokeType=lthan;
					if(ch!=EOF){
						fseek(fin,-1,1);
					}
					break;
				}
				break;
			case 5:
				switch(ch){
				case '=':
					token+=ch;
					tokeType=egthan;
					break;
				default:
					tokeType=gthan;
					if(ch!=EOF){
						fseek(fin,-1,1);
					}
					break;
				}
				break;
			case 6:
				switch(ch){
				case '0':case '1':case '2':case '3':case '4':case '5':case '6':case '7':case '8':case '9':
					token+=ch;
					ch=getc(fin);
					state=6;
					break;
				default:
					tokeType=decimal;
					if(ch!=EOF){
						fseek(fin,-1,1);
					}
					break;
				}
				break;
            }
		}
	}
	return tokeType;  
}