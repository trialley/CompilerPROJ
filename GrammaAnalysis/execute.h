#pragma once
#include "gra.h"
#define max_stack 500

class Execute {
public:
	void execute();
	void print();
public:
	Code ir;//ָ��Ĵ���
	int ip = 0;//�����ַ�Ĵ���
	int sp = 0;//ջ���Ĵ���
	int bp = 0;//����ַ�Ĵ���
	int sl = 1, dl = 2, ra = 0;
	//  ��̬��  ��̬��  ���ص�ַ
	//
	//bp+2-->��̬��     dl
	//bp+1-->��̬��     sl
	//bp---->���ص�ַ   ra
	int stack[max_stack] = { 0 };
	int sp_stack[max_stack] = { 0 };//ָʾ��ǰջ�����г����
	int sp_top = 0;//sp_stack��ջ��
	int sp_bp = 0;//
};