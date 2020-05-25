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

#define MAX_ID_LEN 10
#define BUFFERLEN 1024

/*关键字*/
#define _BEGIN 1
#define _CALL 2
#define _CONST 3
#define _PROCEDURE 4
#define _IF 5
#define _THEN 6
#define _DO 7
#define _END 8
#define _WHILE 9
#define _ODD 10
#define _READ 11
#define _WRITE 12
#define _VAR 13

/*运算符*/
#define _BIGGER 14	   //>
#define _BIGGER_EQ 15  //<=
#define _LOWER 16	   //<
#define _LOWER_EQ 17   //>=
#define _ASSIGN 18	   //:=
#define _EQ 19		   //=
#define _PLUS 20	   //+
#define _POWER 21	   //10//**
#define _STAR 22	   //*
#define _MINUS 29	   //-
#define _DIV 30		   ///
#define _UEQ 31		   //#

/*分界符*/
#define _SEMICOLON 23  //;
#define _LPAIR 24	   //(
#define _RPAIR 25	   //)
#define _LBRACE 26	   //{
#define _RBRACE 27	   //}
#define _COMMA 28	   //,

/*标识符和数字*/
#define _IDENT 32
#define _NUMBER 33

#define INVALID -1

#define RESERVE_LEN 13

extern std::vector<std::string> KeyWords;  //关键字

enum SymbolType {  //符号类型
	CONST,
	VAR,
	PROD  //function
};

enum InsType {	//目标指令
	LIT,		//
	LOD,
	STO,
	CAL,
	INT,
	JMP,
	JPC,
	OPR
};

extern std::map<SymbolType, std::string> SymToString;
extern std::map<InsType, std::string> TisToString;

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

struct symEntry {	   //表示符号的三元组
	int sym;		   //词的词性
	std::string name;  //名字
	int row;		   //所在行
};

struct SYMBOL {
	SYMBOL() {
	}

	SYMBOL(const std::string& name, SymbolType kind, int val, int lev, int addr) {
		this->name = name;
		this->kind = kind;
		this->val = val;
		this->lev = lev;
		this->addr = addr;
	}

	std::string name;  //名字
	SymbolType kind;   //类型
	int val = -1;	   //值
	int lev;		   //层次
	int addr = -1;	   //地址
};

struct CODE {
	CODE(){};
	CODE(InsType fun, int lev, int offset) {
		this->fun = fun;
		this->lev = lev;
		this->offset = offset;
	}
	InsType fun;  //操作码
	int lev;	  //层次差
	int offset;	  //偏移量，也可做操作码
};

// TODO:  在此处引用程序需要的其他头文件
#ifdef _WIN32
#define filenamecut(x) (strrchr(x, '\\') ? strrchr(x, '\\') + 1 : x)
#elif __linux
#define filenamecut(x) (strrchr(x, '/') ? strrchr(x, '/') + 1 : x)
#else
#PushError "No suitable filename cutter for your opearting system.\n"
#endif
#define LOG \
	std::cout << __FILE__ << ":" << __LINE__ << ":1 " << __func__ << "() "
