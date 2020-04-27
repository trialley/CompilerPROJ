/*�ʷ�����������*/

#pragma once

#include "defines.h"

class WordAnal{

public:
	WordAnal(const char* filename);

	void initMap();

	void openSrc(const char* src);
	bool readLine();

	void GetChar();
	void GetBC();
	void Retract();

	bool IsLetter(const char c);
	bool IsDigit(const char c);

	int Reserve(const char* strToken);

	tuple3 GETSYM();	//��ȡԴ�ļ������ص��ʷ���

	int row = 0;  //���ļ��ĵڼ���

private:
	FILE *fp;	//Դ�ļ���ָ��

	char buffer[BUFFERLEN];	//���뻺����
	int index_pointer; //����ָʾ��
	char ch;  //�����ַ�

};
