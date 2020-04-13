#pragma once
//SYM�����ÿ�����ʵ����Ϊ�ڲ�����ı�ʾ��ʽ��
//ID������û�������ı�ʶ����ֵ������ʶ���ַ����Ļ��ڱ�ʾ��
//NUM������û����������
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <vector> 
#include <string> 
#include <fstream>  
#include <iomanip>
using namespace std;

#define NUMBER 29//���ֵ�����
#define IDENT  30//��ʶ��������

#define idnum 50//�ʷ���������ı�ʶ����Ĵ�С
#define numnum 50//�ʷ�������������ֱ�Ĵ�С
#define word_max_num 1000
//�ʷ�������¼��
struct Lexword {
	string name = "";//����
	int sym = 0;//����
	int row = 0;//��������
};



class LexicalAnalysis {
public:
	LexicalAnalysis();
	void setFileName (string);
	bool IsLetter(char letter);
	bool IsDigit(char digit);
	int Retract(char*te);
	int IsSoj(char soj);
	int IsSoj(char *soj);
	int InsertId(char*te);
	char tran(char te);
	int InsertNum(char*te);
	int getsym();
	//void deal();
	bool lexanalysis();
public:
	string filename;
	ifstream sourfile;//���ļ���
	ofstream destfile;//д�ļ���

	char idTB[idnum][10] = { "" };//��ʶ����
	int id = 0;//��ʶ����ָ��
	char numTB[numnum][10] = { "" };//���ֱ�
	int num = 0;//���ֱ�ָ��





	char ch;//��ǰ������ַ�
	char strToken[15] = "";//��ǰ������ַ���
	int p = 0;//��ǰ������ַ�����ָ��
	int code, value;//������Ϣ
	char other[3] = "--";//�������

};










