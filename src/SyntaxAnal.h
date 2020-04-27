/*�﷨��������*/
#ifndef SYNTAXANAL_H
#define SYNTAXANAL_H

#include <string>

#include "defines.h"
#include "lexer.h"
class SyntaxAnal {
   public:
	SyntaxAnal(const std::string&);

	void BLOCK();		  //�﷨����
	void analyzeConst();  //����˵������
	void analyzeVar();	  //����˵������
	void analyzePro();	  //����˵������
	void analyzeSent();	  //���

	void analyzeCond();	 //�����������
	void analyzeExpr();	 //������ʽ

	void analyzeTerm();	 //������
	void analyzeElem();	 //��������

	bool insertSymbol(SymbolKind kind, const char* id);	 //����ű�������
	int searchSymbol(const char* id);					 //�ڷ��ű��в��ҷ��ţ�û�ҵ�����-1

	void GEN(FunctionCode fun, int lev, int offset);  //�����м����

	void printTable();
	void printCode();

	void generateFile(const char* filename);  //����Ŀ���ļ�

	int error(int e, int eline);  //������

	vector<CODE> codeTable;	 //�����

   private:
	lexer* wa;	//�ʷ�������
	tuple3 t;	//�ʷ�������ȡ���ĵ��ʷ���

	vector<SYMBOL> symbolTable;	 //���ű�

	int lev = -1;  //���
	int dx;		   //ÿ��ֲ�������Ե�ַ��ÿ˵����һ��������dx+1

	int cx = 0;	 //����εĴ�С
	int cx0;	 //���浱ǰ���ɴ���ĵ�ַ

	int mainEntry;	 //��¼���������
	int offset = 1;	 // ��¼һ��ʼ�ճ���ǰ����ָ���ƫ��

	string errors[100];	 //�洢�Ĵ�����Ϣ
	int etop = 0;		 //������Ϣջ��
};

#endif