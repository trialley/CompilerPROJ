#pragma once
#include "include.h"
#include "lex.h"
#include "tree.h"
#include "codes.h"
#include "symboltable.h"



////符号表中每一条记录对应的结构
//struct Grasym {
//	string name = "";//名字
//	string kind = "";//类型，const var procedur
//					 //int kind;
//	int value = 0;//数值，const使用
//	int level = 0;//所处层，var procedure使用
//	int addr = 0;;//地址，var procedure使用
//				  //int size;//需要分配的空间，procedure使用
//};
//Grasym table[table_max_num];
//int tablenum = 0;//标记符号表中符号数量
//int tablep = 0;//table表指针








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
	ifstream sourfile;//读文件用
	ofstream destfile;//写文件用
	Tree *tree=NULL;//语法树
	SymbolTable symtable;//符号表


};
