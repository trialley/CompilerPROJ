#pragma once
#include "include.h"
#define table_max_num 50
//符号表中每一条记录对应的结构
struct Grasym {
	string name = "";//名字
	string kind = "";//类型，const var procedur
					 //int kind;
	int value = 0;//数值，const使用
	int level = 0;//所处层，var procedure使用
	int addr = 0;;//地址，var procedure使用
	int top = 0;//每一过程的索引
	//int size;//需要分配的空间，procedure使用
};

class SymbolTable {
public:
	int search(string name, int level, int mode);
	void enter(string name, string type, int value, int level, int addr);
	void print(string tp);
public:
	Grasym table[table_max_num];
	int tablenum = 0;//标记符号表中符号数量
	//int tablep = 0;//table表指针

};
