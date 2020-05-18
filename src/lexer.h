/*�ʷ�����������*/

#pragma once

#include "defines.h"

class lexer {
public:
	lexer(const std::string&);

	void initMap();	 //��ʼ��ÿ�����Ŷ�Ӧ���ַ������ƣ����ڴ�ӡ����

	void openSrc(const std::string&);  //��Դ�ļ�
	bool readLine();				   //��ȡһ��

	void GetChar();	 //���һ������
	void GetBC();	 //�����ո�
	void Retract();

	bool IsLetter(const char c);
	bool IsDigit(const char c);

	int Reserve(const char* strToken);

	symEntry getSym();	//��ȡԴ�ļ������ص��ʷ���

	int row = 0;  //���ļ��ĵڼ���

private:
	FILE* fp;  //Դ�ļ���ָ��

	char buffer[BUFFERLEN];	 //���뻺����
	int index_pointer;		 //����ָʾ��
	char ch;				 //�����ַ�
};
