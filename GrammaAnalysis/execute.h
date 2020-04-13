#pragma once
#include "gra.h"
#define max_stack 500

class Execute {
public:
	void execute();
	void print();
public:
	Code ir;//指令寄存器
	int ip = 0;//程序地址寄存器
	int sp = 0;//栈顶寄存器
	int bp = 0;//基地址寄存器
	int sl = 1, dl = 2, ra = 0;
	//  静态链  动态链  返回地址
	//
	//bp+2-->动态链     dl
	//bp+1-->静态链     sl
	//bp---->返回地址   ra
	int stack[max_stack] = { 0 };
	int sp_stack[max_stack] = { 0 };//指示当前栈中运行程序的
	int sp_top = 0;//sp_stack的栈顶
	int sp_bp = 0;//
};