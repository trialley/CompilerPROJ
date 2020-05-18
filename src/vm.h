#pragma once

#include <iostream>

#include "parser.h"

class vm {
public:
	vm(const std::string&);
	void run();		 //ѭ��ִ��Ŀ���ļ���ָ��
	void runInst();	 //ִ��һ��ָ��

	void readMem();		 //�����̲���
	void writeMem(int);	 //���

	void printStack();	//���ջ��ǰ״̬

	void readPl0(const std::string&);  //��ȡĿ���ļ�

private:
	CODE inst;	//ָ��Ĵ���
	int ip;		//���������
	int sp;		//ջ���Ĵ���
	int bp;		//��ַ�Ĵ���

	int* stack;					//����ջ
	std::vector<CODE> codeSeg;	//�����

	int lev;			  //���浱ǰ���¼�ľ�̬����
	std::vector<int> SL;  //��̬��
};