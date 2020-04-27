/*����������*/
#ifndef ITERPRETR_H
#define ITERPRETR_H


#include "SyntaxAnal.h"
#include <iostream>

using namespace std;

class Iterpreter{

public:
	Iterpreter(const char* filename);
	void run();//ѭ��ִ��Ŀ���ļ���ָ��
	void runInst();//ִ��һ��ָ��

	void readMem();//�����̲���
	void writeMem(int addr);//���


	void printStack();//���ջ��ǰ״̬

	void readPl0(const char* filename);//��ȡĿ���ļ�

private:
	CODE inst;	//ָ��Ĵ���
	int ip;	//���������
	int sp;//ջ���Ĵ���
	int bp;//��ַ�Ĵ���

	int *stack;//����ջ
	vector<CODE> codeSeg;	//�����

	
	int lev;	//���浱ǰ���¼�ľ�̬����
	vector<int> SL;//��̬��
};

#endif