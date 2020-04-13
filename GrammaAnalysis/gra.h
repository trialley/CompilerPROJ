#pragma once
#include "include.h"
#include "lex.h"
#include "tree.h"
#include "codes.h"
#include "symboltable.h"



////���ű���ÿһ����¼��Ӧ�Ľṹ
//struct Grasym {
//	string name = "";//����
//	string kind = "";//���ͣ�const var procedur
//					 //int kind;
//	int value = 0;//��ֵ��constʹ��
//	int level = 0;//�����㣬var procedureʹ��
//	int addr = 0;;//��ַ��var procedureʹ��
//				  //int size;//��Ҫ����Ŀռ䣬procedureʹ��
//};
//Grasym table[table_max_num];
//int tablenum = 0;//��Ƿ��ű��з�������
//int tablep = 0;//table��ָ��








class GrammaAnalysis {
public:
	GrammaAnalysis(string filename);
	GrammaAnalysis() { delete tree; };
	//void enter(string name, string type, int value, int level, int addr);
	string IDent(Node *p);
	string UnsignedNum(Node *p);
	string Roperator(Node *p);
	string ASoperator(Node *p);
	string MDoperator(Node *p);
	void ConstantDefinition(int level, int top, Node *p);
	void Const(int level, int top, Node *p);
	void Varible(int level, int top, Node *p);
	void ProcedureHeader(int level, int top, Node *p);
	void Procedure(int level, int top, Node *p);
	void Factor(int level, int top, Node *p);
	void Item(int level, int top, Node *p);
	void Expression(int level, int top, Node *p);
	void AssignmentStatement(int level, int top, Node *p);
	void Condition(int level, int top, Node *p);
	void ConditionalStatement(int level, int top, Node *p);
	void LoopStatement(int level, int top, Node *p);
	void ProcedureCallStatement(int level, int top, Node *p);
	void ReadStatement(int level, int top, Node *p);
	void WriteStatement(int level, int top, Node *p);
	void CompoundStatement(int level, int top, Node *p);
	void Statement(int level, int top, Node *p);
	void SubProgram(int level, int top, Node *p);
	void Program(Node*p);
	void gramanalysis();
private:
	string filename="";
	ifstream sourfile;//���ļ���
	ofstream destfile;//д�ļ���
	Tree *tree=NULL;//�﷨��
	SymbolTable symtable;//���ű�


};
