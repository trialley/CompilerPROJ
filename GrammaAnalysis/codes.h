#pragma once
#include "include.h"
#define code_max_num 500


struct Code {
	int f = 0;//������
	int l = 0;//��β�
	int a = 0;//λ����
};
class Codes {
public:
	void gen(int f, int l, int a);
	void print(string tp);
public:
	Code code[code_max_num];
	int codenum = 0;//��������
};