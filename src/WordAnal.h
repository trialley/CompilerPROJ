/*词法分析器程序*/

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

	tuple3 GETSYM();	//读取源文件并返回单词符号

	int row = 0;  //在文件的第几行

private:
	FILE *fp;	//源文件的指针

	char buffer[BUFFERLEN];	//读入缓冲区
	int index_pointer; //搜索指示器
	char ch;  //读入字符

};
