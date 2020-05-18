#pragma once

#include <iostream>

#include "parser.h"

class vm {
public:
	vm(const std::string&);
	void run();		 //循环执行目标文件中指令
	void runInst();	 //执行一条指令

	void readMem();		 //读键盘操作
	void writeMem(int);	 //输出

	void printStack();	//输出栈当前状态

	void readPl0(const std::string&);  //读取目标文件

private:
	CODE inst;	//指令寄存器
	int ip;		//程序计数器
	int sp;		//栈顶寄存器
	int bp;		//基址寄存器

	int* stack;					//数据栈
	std::vector<CODE> codeSeg;	//代码段

	int lev;			  //保存当前活动记录的静态层数
	std::vector<int> SL;  //静态链
};