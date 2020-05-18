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

/*�ؼ���*/
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

/*�����*/
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

/*�ֽ��*/
#define _SEMICOLON 23  //;
#define _LPAIR 24	   //(
#define _RPAIR 25	   //)
#define _LBRACE 26	   //{
#define _RBRACE 27	   //}
#define _COMMA 28	   //,

/*��ʶ��������*/
#define _IDENT 32
#define _NUMBER 33

#define INVALID -1

#define RESERVE_LEN 13

extern std::vector<std::string> rsv_;  //�ؼ���

enum SymbolKind {  //��������
	CONST,
	VAR,
	PROD  //function
};

enum FunctionCode {	 //Ŀ��ָ��
	LIT,			 //
	LOD,
	STO,
	CAL,
	INT,
	JMP,
	JPC,
	OPR
};

extern std::map<SymbolKind, const char*> symMap;
extern std::map<FunctionCode, const char*> opMap;

enum OPR {	//OPRָ����a���ȡֵ
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

struct symEntry {			//��ʾ���ŵ���Ԫ��
	int sym;				//�ʵĴ���
	char name[MAX_ID_LEN];	//����
	int row;				//������
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

	char name[MAX_ID_LEN];	//����
	SymbolKind kind;		//����
	int val = -1;			//ֵ
	int lev;				//���
	int addr = -1;			//��ַ
};

struct CODE {
	CODE(){};
	CODE(FunctionCode fun, int lev, int offset) {
		this->fun = fun;
		this->lev = lev;
		this->offset = offset;
	}
	FunctionCode fun;  //������
	int lev;		   //��β�
	int offset;		   //ƫ������Ҳ����������
};

// TODO:  �ڴ˴����ó�����Ҫ������ͷ�ļ�
#ifdef _WIN32
#define filenamecut(x) (strrchr(x, '\\') ? strrchr(x, '\\') + 1 : x)
#elif __linux
#define filenamecut(x) (strrchr(x, '/') ? strrchr(x, '/') + 1 : x)
#else
#error "No suitable filename cutter for your opearting system.\n"
#endif
#define LOG \
	std::cout << __FILE__ << ":" << __LINE__ << ":1 " << __func__ << "() "
