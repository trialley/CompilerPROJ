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

/*�ؼ���*/
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

/*�����*/
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

/*�ֽ��*/
#define $SEMICOLON 23  //;
#define $LPAIR 24	   //(
#define $RPAIR 25	   //)
#define $LBRACE 26	   //{
#define $RBRACE 27	   //}
#define $COMMA 28	   //,

/*��ʶ��������*/
#define $IDENT 32
#define $NUMBER 33

#define INVALID -1

#define RESERVE_LEN 13

extern std::vector<std::string> rsv_;  //�ؼ���

enum SymbolKind {  //��������
	CONST,
	VAR,
	PROD
};

enum FunctionCode {	 //Ŀ��ָ��
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

struct tuple3 {				//��ʾ���ŵ���Ԫ��
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