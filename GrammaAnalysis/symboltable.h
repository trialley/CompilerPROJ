#pragma once
#include "include.h"
#define table_max_num 50
//���ű���ÿһ����¼��Ӧ�Ľṹ
struct Grasym {
	string name = "";//����
	string kind = "";//���ͣ�const var procedur
					 //int kind;
	int value = 0;//��ֵ��constʹ��
	int level = 0;//�����㣬var procedureʹ��
	int addr = 0;;//��ַ��var procedureʹ��
	int top = 0;//ÿһ���̵�����
	//int size;//��Ҫ����Ŀռ䣬procedureʹ��
};

class SymbolTable {
public:
	int search(string name, int level, int mode);
	void enter(string name, string type, int value, int level, int addr);
	void print(string tp);
public:
	Grasym table[table_max_num];
	int tablenum = 0;//��Ƿ��ű��з�������
	//int tablep = 0;//table��ָ��

};
