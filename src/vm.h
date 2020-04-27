/*����������*/
#ifndef ITERPRETR_H
#define ITERPRETR_H

#include <iostream>

#include "parser.h"

using namespace std;

class vm {
   public:
	vm(const char* filename);
	void run();		 //ѭ��ִ��Ŀ���ļ���ָ��
	void runInst();	 //ִ��һ��ָ��

	void readMem();			  //�����̲���
	void writeMem(int addr);  //���

	void printStack();	//���ջ��ǰ״̬

	void readPl0(const char* filename);	 //��ȡĿ���ļ�

   private:
	CODE inst;	//ָ��Ĵ���
	int ip;		//���������
	int sp;		//ջ���Ĵ���
	int bp;		//��ַ�Ĵ���

	int* stack;			   //����ջ
	vector<CODE> codeSeg;  //�����

	int lev;		 //���浱ǰ���¼�ľ�̬����
	vector<int> SL;	 //��̬��
};

#endif