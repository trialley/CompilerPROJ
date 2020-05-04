#pragma once

#include <cstdio>
#include <cstring>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "defines.h"

using namespace std;

#define MAX_ID_LEN 10
#define BUFFERLEN 1024

/*关键字*/
#define $BEGIN 1
#define $CALL 2
#define $CONST 3
#define $PROCEDURE 4
#define $IF 5
#define $THEN 6
#define $DO 7
#define $END 8
#define $WHILE 9
#define $ODD 10
#define $READ 11
#define $WRITE 12
#define $VAR 13

/*运算符*/
#define $BIGGER 14	   //>
#define $BIGGER_EQ 15  //<=
#define $LOWER 16	   //<
#define $LOWER_EQ 17   //>=
#define $ASSIGN 18	   //:=
#define $EQ 19		   //=
#define $PLUS 20	   //+
#define $POWER 21	   //10//**
#define $STAR 22	   //*
#define $MINUS 29	   //-
#define $DIV 30		   ///
#define $UEQ 31		   //#

/*分界符*/
#define $SEMICOLON 23  //;
#define $LPAIR 24	   //(
#define $RPAIR 25	   //)
#define $LBRACE 26	   //{
#define $RBRACE 27	   //}
#define $COMMA 28	   //,

/*标识符和数字*/
#define $IDENT 32
#define $NUMBER 33

#define INVALID -1

#define RESERVE_LEN 13

extern std::vector<std::string> rsv_;  //关键字

enum SymbolKind {  //符号类型
	CONST,
	VAR,
	PROD
};

enum FunctionCode {	 //目标指令
	LIT,
	LOD,
	STO,
	CAL,
	INT,
	JMP,
	JPC,
	OPR
};

extern map<SymbolKind, const char*> symMap;
extern map<FunctionCode, const char*> opMap;

enum OPR {	//OPR指令中a域的取值
	ADD = 1,
	SUB = 2,
	MUL = 3,
	DIV = 4,
	GT = 5,
	LT = 6,
	GE = 7,
	LE = 8,
	UE = 9,
	EQ = 10,
	WRITE = 11,
	READ = 12,
	MINUS = 13,
	ODD = 14
};

struct tuple3 {				//表示符号的三元组
	int sym;				//词的词性
	char name[MAX_ID_LEN];	//名字
	int row;				//所在行
};

struct SYMBOL {
	SYMBOL() {
	}

	SYMBOL(const char* name, SymbolKind kind, int val, int lev, int addr) {
		strcpy(this->name, name);
		this->kind = kind;
		this->val = val;
		this->lev = lev;
		this->addr = addr;
	}

	char name[MAX_ID_LEN];	//名字
	SymbolKind kind;		//类型
	int val = -1;			//值
	int lev;				//层次
	int addr = -1;			//地址
};

struct CODE {
	CODE(){};
	CODE(FunctionCode fun, int lev, int offset) {
		this->fun = fun;
		this->lev = lev;
		this->offset = offset;
	}
	FunctionCode fun;  //操作码
	int lev;		   //层次差
	int offset;		   //偏移量，也可做操作码
};

// TODO:  在此处引用程序需要的其他头文件
#ifdef _WIN32
#define filenamecut(x) (strrchr(x, '\\') ? strrchr(x, '\\') + 1 : x)
#elif __linux
#define filenamecut(x) (strrchr(x, '/') ? strrchr(x, '/') + 1 : x)
#else
#error "No suitable filename cutter for your opearting system.\n"
#endif
#define LOG \
	std::cout << __FILE__ << ":" << __LINE__ << ":1 " << __func__ << "() "