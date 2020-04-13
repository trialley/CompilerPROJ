#pragma once
//SYM：存放每个单词的类别，为内部编码的表示形式。
//ID：存放用户所定义的标识符的值，即标识符字符串的机内表示。
//NUM：存放用户定义的数。
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <vector> 
#include <string> 
#include <fstream>  
#include <iomanip>
using namespace std;

#define NUMBER 29//数字的种类
#define IDENT  30//标识符的种类

#define idnum 50//词法分析构造的标识符表的大小
#define numnum 50//词法分析构造的数字表的大小
#define word_max_num 1000
//词法分析记录项
struct Lexword {
	string name = "";//名称
	int sym = 0;//类型
	int row = 0;//所在行数
};



class LexicalAnalysis {
public:
	LexicalAnalysis();
	void setFileName (string);
	bool IsLetter(char letter);
	bool IsDigit(char digit);
	int Retract(char*te);
	int IsSoj(char soj);
	int IsSoj(char *soj);
	int InsertId(char*te);
	char tran(char te);
	int InsertNum(char*te);
	int getsym();
	//void deal();
	bool lexanalysis();
public:
	string filename;
	ifstream sourfile;//读文件用
	ofstream destfile;//写文件用

	char idTB[idnum][10] = { "" };//标识符表
	int id = 0;//标识符表指针
	char numTB[numnum][10] = { "" };//数字表
	int num = 0;//数字表指针





	char ch;//当前读入的字符
	char strToken[15] = "";//当前读入的字符串
	int p = 0;//当前读入的字符串的指针
	int code, value;//返回信息
	char other[3] = "--";//用于输出

};










