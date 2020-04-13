#include "gra.h"
/*从词法分析阶段拿过来的数据*/
extern char symTB[29][15];//关键字与算符界符表
extern Lexword word[word_max_num];//单词表
extern int wordnum;//单词表中单词数量



int wordp = 0;//单词表指针


Codes codes;//目标代码类
int dx = 3;//当前程序局部变量相对地址
int cx = 0;//代码索引
int tx = 0;//符号表索引

Lexword td;//临时的单词项




GrammaAnalysis::GrammaAnalysis(string filename) {
	this->filename = "../" + filename;
}


//******************************************
//从词法分析结果中读取一个字符串，如果是标识符则返回它，否则返回空
string GrammaAnalysis::IDent(Node *p) {
	//标识符
	
	if (word[wordp].sym == IDENT) {
		p->makenode(word[wordp].name);
		return word[wordp++].name;
	}
	else {
		cout << "出错行数："<<word[wordp].row<<"\n错误类型：缺少标识符\n";
		system("pause");
		return "";
	}

}
//******************************************
//从词法分析结果中读取一个字符串，如果是数字则返回它，否则返回空
string GrammaAnalysis::UnsignedNum(Node *p) {
	//无符号数
	if (word[wordp].sym == NUMBER) {
		p->makenode(word[wordp].name);
		return word[wordp++].name;
	}
	else {
		cout << "出错行数：" << word[wordp].row << "\n错误类型：缺少数字\n";
		system("pause");
		return "";
	}
}
//******************************************
//从词法分析结果中读取一个字符串，如果是关系运算符则返回它，否则返回空
string GrammaAnalysis::Roperator(Node *p) {
	//无符号数
	if (word[wordp].sym >= 17 && word[wordp].sym <= 22) {
		p->makenode(word[wordp].name);
		return word[wordp++].name;
	}
	else {
		//cout << word[wordp].name << "不是关系运算符\n";
		cout << "出错行数：" << word[wordp].row << "\n错误类型：缺少关系运算符\n";
		system("pause");
		return "";
	}
}
//******************************************
//从词法分析结果中读取一个字符串，如果是加减运算符则返回它，否则返回空
string GrammaAnalysis::ASoperator(Node *p) {
	//无符号数
	if (word[wordp].name == "+" && word[wordp].name == "-") {
		p->makenode(word[wordp].name);
		return word[wordp++].name;
	}
	else {
		cout << "出错行数：" << word[wordp].row << "\n错误类型：缺少加减运算符\n";
		system("pause");
		return "";
	}
}
//******************************************
//从词法分析结果中读取一个字符串，如果是乘除运算符则返回它，否则返回空
string GrammaAnalysis::MDoperator(Node *p) {
	//无符号数
	if (word[wordp].name == "*" && word[wordp].name == "/") {
		p->makenode(word[wordp].name);
		return word[wordp++].name;
	}
	else {
		cout << "出错行数：" << word[wordp].row << "\n错误类型：缺少乘除运算符\n";
		system("pause");
		return "";
	}
}

//******************************************
//常量定义
void GrammaAnalysis::ConstantDefinition(int level, int top, Node *p) {
	string tempnum;
	int tempnums;
	string tempid=IDent(p->makenode("标识符"));//标识符
	int i = symtable.search(tempid, level, 0);
	if (i != -1) {
		cout << "出错行数：" << word[wordp].row << "\n错误类型：标识符重定义\n";
		system("pause");
		return;
	}
	if (word[wordp].name == "=") {
		p->makenode("=");
		wordp++;

		tempnum = UnsignedNum(p->makenode("数字"));//无符号整数
		tempnums = atoi(tempnum.c_str());
		symtable.enter(tempid,"const",tempnums,-1,-1);//添加到符号表
		tx++;//符号表索引加一
			
	}
	else {
		cout << "出错行数：" << word[wordp].row << "\n错误类型：常量定义部分缺少等于号\n";
		system("pause");
		return;
	}
}
//******************************************
//常量说明部分
void GrammaAnalysis::Const(int level, int top, Node *p) {
	if (word[wordp].name == "const") {
		p->makenode("const");
		wordp++;
		ConstantDefinition(level, top, p->makenode("常量定义"));
		while (word[wordp].name == ",") {
			p->makenode(",");
			wordp++;
			ConstantDefinition(level, top, p->makenode("常量定义"));
		}
		if (word[wordp].name == ";") {
			p->makenode(";");
			wordp++;
		}
	}
	else {
		cout << "出错行数：" << word[wordp].row << "\n错误类型：常量说明部分缺少关键字const\n";
		system("pause");
		return;
	}
	
}
//******************************************
//变量说明部分
void GrammaAnalysis::Varible(int level, int top, Node *p) {
	if (word[wordp].name == "var") {
		p->makenode("var");
		wordp++;
		string tempid=IDent(p->makenode("标识符"));
		int i = symtable.search(tempid, level, 1);
		if (i != -1) {
			cout << "出错行数：" << word[wordp].row << "\n错误类型：标识符重定义\n";
			system("pause");
			return;
		}
		
		symtable.enter(tempid, "varible", -1, level, dx++);
		tx++;


		while (word[wordp].name == ",") {
			p->makenode(",");
			wordp++;
			tempid = IDent(p->makenode("标识符"));
			int i = symtable.search(tempid, level, 0);
			if (i != -1) {
				cout << "出错行数：" << word[wordp].row << "\n错误类型：标识符重定义\n";
				system("pause");
				return;
			}
			symtable.enter(tempid, "varible", -1, level, dx++);
			tx++;
		}
		if (word[wordp].name == ";") {
			p->makenode(";");
			wordp++;
		}
		else {
			cout << "出错行数：" << word[wordp].row << "\n错误类型：变量说明部分缺少分号\n";
			system("pause");
			return;
		}
	}
	else {
		cout << "出错行数：" << word[wordp].row << "\n错误类型：变量说明部分缺少关键字var\n";
		system("pause");
		return;
	}
	
}
//******************************************
//过程首部说明部分
void GrammaAnalysis::ProcedureHeader(int level, int top, Node *p) {
	if (word[wordp].name == "procedure") {
		p->makenode(word[wordp].name);
		wordp++;
		string tempid = IDent(p->makenode("标识符"));
		symtable.enter(tempid, "procedure", -1, level, 0);
		tx++;

		if (word[wordp].name == ";") {
			p->makenode(";");
			wordp++;
		}
		else {
			cout << "出错行数：" << word[wordp].row << "\n错误类型：过程首部部分缺少分号\n";
			system("pause");
			return;
		}
	}
}
//******************************************
//过程说明部分
void GrammaAnalysis::Procedure(int level, int top, Node *p) {
	ProcedureHeader(level, top, p->makenode("过程首部"));
	top++;//目前暂时还不明白top的作用
	SubProgram(level, top, p->makenode("分程序"));


	if (word[wordp].name == ";") {
		wordp++;
		p->makenode(";");
		while (word[wordp].name == "procedure") {
			Procedure(level, top, p);
		}	
	}
	else  {
		cout << "出错行数：" << word[wordp].row << "\n错误类型：过程说明部分缺少分号\n";
		system("pause");
		return;
	}

}
//******************************************
//因子
void GrammaAnalysis::Factor(int level, int top, Node *p) {
	string temp;
	if (word[wordp].sym == IDENT) {
		temp = IDent(p->makenode("标识符"));
		int i = symtable.search(temp, level, 0);
		if (i == -1) {
			cout << "出错行数：" << word[wordp].row << "\n错误类型：未定义标识符\n";
			system("pause");
			return;
		}
		if (symtable.table[i].kind == "const") {
			codes.gen(0, 0, symtable.table[i].value);
			cx++;
		}
		else if (symtable.table[i].kind == "varible"){
			codes.gen(1, level - symtable.table[i].level, symtable.table[i].addr);
			cx++;
		}
		else {
			cout << "出错行数：" << word[wordp].row << "\n错误类型：非数值标识符\n";
			system("pause");
			return;
		}
		


	}
	else if (word[wordp].sym == NUMBER) {
		temp = UnsignedNum(p->makenode("数字"));
		codes.gen(0, 0, atoi(temp.c_str()));
		cx++;
	}
	else if (word[wordp].name == "(") {
		p->makenode("(");
		wordp++;
		Expression(level, top, p->makenode("表达式"));
		if (word[wordp].name == ")") {
			p->makenode(")");
			wordp++;
		}
		else {
			cout << "出错行数：" << word[wordp].row << "\n错误类型：缺少右括号\n";
			system("pause");
			return;
		}
	}
	else {
		cout << "出错行数：" << word[wordp].row << "\n错误类型：缺少因子\n";
		system("pause");
		return;
	}
}
//******************************************
//项
void GrammaAnalysis::Item(int level, int top, Node *p) {
	Factor(level, top, p->makenode("因子"));
	while (word[wordp].name == "*" || word[wordp].name == "/") {
		string temp = word[wordp].name;
		p->makenode(word[wordp].name);
		wordp++;
		Factor(level, top, p->makenode("因子"));
		if (temp == "*"){
			codes.gen(7, 0, 4);
			cx++;
		}
		else if (temp == "/") {
			codes.gen(7, 0, 5);
			cx++;
		}
			

	}
}
//******************************************
//表达式
void GrammaAnalysis::Expression(int level, int top, Node *p) {
	string temp = "";
	if (word[wordp].name == "+" || word[wordp].name == "-") {
		temp = word[wordp].name;
		p->makenode(word[wordp].name);
		wordp++;
	}

	Item(level, top, p->makenode("项"));
	if (temp == "-") {
		codes.gen(7, 0, 1);//取反
		cx++;
	}
		

	
	while (word[wordp].name == "+" || word[wordp].name == "-") {
		temp = word[wordp].name;
		p->makenode(word[wordp].name);
		wordp++;
		Item(level, top, p->makenode("项"));
		if (temp == "+") {
			codes.gen(7, 0, 2);//加
			cx++;
		}
		else if (temp == "-") {
			codes.gen(7, 0, 3);//减
			cx++;
		}
			
	}
}
//******************************************
//赋值语句
void GrammaAnalysis::AssignmentStatement(int level, int top, Node *p) {
	string temp = IDent(p->makenode("标识符"));
	int i = symtable.search(temp, level, 0);
	if (i == -1) {
		cout << "出错行数：" << word[wordp].row << "标识符\"" << temp << "\"未找到定义\n";
		system("pause");
		return;
	}
	if (symtable.table[i].kind == "const") {
		cout << "出错行数：" << word[wordp].row << "常量\"" << temp << "\"不能作为变量使用\n";
		system("pause");
		return;
	}
	
	if (temp == "") {
		cout << "赋值语句的赋值对象非标识符\n";
		system("pause");
		return;
	}
	else {
		if (word[wordp].name == ":=") {
			p->makenode(":=");
			wordp++;
			
			Expression(level, top, p->makenode("表达式"));

			codes.gen(2,level-symtable.table[i].level, symtable.table[i].addr);
			cx++;
		}
		else {
			cout << "出错行数：" << word[wordp].row << "\n错误类型：赋值语句缺少:=\n";
			system("pause");
			return;
		}
	}
}
//******************************************
//条件
void GrammaAnalysis::Condition(int level, int top, Node *p) {
	if (word[wordp].sym == IDENT) {
		Expression(level, top, p->makenode("表达式"));
		string temp = Roperator(p->makenode("比较运算符"));
		Expression(level, top, p->makenode("表达式"));
		if (temp == ">") {
			codes.gen(7, 0, 12);
			cx++;
		}
		else if (temp == "<") {
			codes.gen(7, 0, 10);
			cx++;
		}
		else if (temp == "=") {
			codes.gen(7, 0, 8);
			cx++;
		}
		else if (temp == "#") {
			codes.gen(7, 0, 9);
			cx++;
		}
		else if (temp == ">=") {
			codes.gen(7, 0, 11);
			cx++;
		}
		else if (temp == "<=") {
			codes.gen(7, 0, 13);
			cx++;
		}
		
	}
	else if (word[wordp].name == "odd") {
		p->makenode("odd");
		wordp++;
		Expression(level, top, p->makenode("表达式"));
		codes.gen(7, 0, 6);
		cx++;
	}
	else {
		cout << "出错行数：" << word[wordp].row << "\n错误类型：条件表达式错误\n";
		system("pause");
		return;
	}
}
//******************************************
//条件语句
void GrammaAnalysis::ConditionalStatement(int level, int top, Node *p) {
	if (word[wordp].name == "if") {
		
		p->makenode(word[wordp].name);
		wordp++;
		
		Condition(level, top, p->makenode("条件"));
		int cx0 = cx;//待返填的指令的地址
		codes.gen(6, 0, 0);//JPC，跳转地址未定
		cx++;
		if (word[wordp].name == "then") {
			p->makenode(word[wordp].name);
			wordp++;
			Statement(level, top, p->makenode("语句"));
			codes.code[cx0].a = cx;//返填地址
		}
		else {
			cout << "条件语句缺少then\n";
			system("pause");
			return;
		}
	}
	else {
		cout << "出错行数：" << word[wordp].row << "\n错误类型：条件语句缺少if\n";
		system("pause");
		return;
	}
}
//******************************************
//循环语句
void GrammaAnalysis::LoopStatement(int level, int top, Node *p) {
	if (word[wordp].name == "while") {
		p->makenode(word[wordp].name);
		wordp++;
		int cx0 = cx;
		Condition(level, top, p->makenode("条件"));
		int cx1 = cx;//待返填的指令的地址
		codes.gen(6, 0, 0);//JPC，跳转地址未定
		cx++;
		if (word[wordp].name == "do") {
			p->makenode(word[wordp].name);
			wordp++;
			Statement(level, top, p->makenode("语句"));
			codes.gen(5, 0, cx0);//无条件跳转回去
			cx++;
			codes.code[cx1].a = cx;//返填指令
		}
		else {
			cout << "出错行数：" << word[wordp].row << "\n错误类型：循环语句缺少do\n";
			system("pause");
			return;
		}
	}
	else {
		cout << "出错行数：" << word[wordp].row << "\n错误类型：条件语句缺少while\n";
		system("pause");
		return;
	}
}
//******************************************
//过程调用语句
void GrammaAnalysis::ProcedureCallStatement(int level, int top, Node *p) {
	if (word[wordp].name == "call") {
		p->makenode(word[wordp].name);
		wordp++;
		//td = word[wordp];
		string temp = IDent(p->makenode("标识符"));
		int i = symtable.search(temp, level, 1);
		if (i < 0) {
			cout << "出错行数：" << word[wordp-1].row
				<< "\n错误类型：" << word[wordp-1].name << "是未定义或者非法调用的过程标识符\n";
			system("pause");
			return;
		}
		if (symtable.table[i].kind != "procedure") {
			cout << "出错行数：" << word[wordp-1].row
				<< "\n错误类型：" << word[wordp-1].row << "非过程标识符\n";
			system("pause");
			return;
		}
		codes.gen(3, level - symtable.table[i].level, symtable.table[i].addr);
		cx++;
	}
	else {
		cout << "出错行数：" << word[wordp].row << "\n错误类型：过程调用语句缺少call\n";
		system("pause");
		return;
	}
}
//******************************************
//读语句
void GrammaAnalysis::ReadStatement(int level, int top, Node *p) {
	if (word[wordp].name == "read") {
		p->makenode(word[wordp].name);
		wordp++;
		if (word[wordp].name == "(") {
			p->makenode(word[wordp].name);
			wordp++;
			string temp = IDent(p->makenode("标识符"));
			int i = symtable.search(temp, level, 0);
			if (i < 0) {
				cout << "出错行数：" << word[wordp].row << "\n错误类型：未定义变量\n";
				system("pause");
				return;
			}
			if (symtable.table[i].kind != "varible") {
				cout << "出错行数：" << word[wordp].row << "\n错误类型：非变量\n";
				system("pause");
				return;
			}
			codes.gen(7, 0, 16);
			codes.gen(2, level - symtable.table[i].level, symtable.table[i].addr);
			cx += 2;


			while (word[wordp].name == ",") {
				p->makenode(word[wordp].name);
				wordp++;
				temp = IDent(p->makenode("标识符"));
				i = symtable.search(temp, level, 0);
				if (i < 0) {
					cout << "出错行数：" << word[wordp].row << "\n错误类型：未定义变量\n";
					system("pause");
					return;
				}
				if (symtable.table[i].kind != "varible") {
					cout << "出错行数：" << word[wordp].row << "\n错误类型：非变量\n";
					system("pause");
					return;
				}
				codes.gen(7, 0, 16);
				codes.gen(2, level - symtable.table[i].level, symtable.table[i].addr);
				cx += 2;
			}
			if (word[wordp].name == ")") {
				p->makenode(word[wordp].name);
				wordp++;
				return;
			}
			else {
				cout << "出错行数：" << word[wordp].row << "\n错误类型：读语句缺少右括号\n";
				system("pause");
				return;
			}
		}
		else {
			cout << "出错行数：" << word[wordp].row << "\n错误类型：读语句缺少左括号\n";
			system("pause");
			return;
		}
	}
	else {
		cout << "出错行数：" << word[wordp].row << "\n错误类型：读语句缺少关键字read\n";
		system("pause");
		return;
	}
}
//******************************************
//写语句
void GrammaAnalysis::WriteStatement(int level, int top, Node *p) {
	if (word[wordp].name == "write") {
		p->makenode(word[wordp].name);
		wordp++;
		if (word[wordp].name == "(") {
			p->makenode(word[wordp].name);
			wordp++;
			Expression(level, top, p->makenode("表达式"));
			codes.gen(7, 0, 14);
			codes.gen(7, 0, 15);
			cx += 2;
			while (word[wordp].name == ",") {
				p->makenode(word[wordp].name);
				wordp++;
				Expression(level, top, p->makenode("表达式"));
				codes.gen(7, 0, 14);
				codes.gen(7, 0, 15);
				cx += 2;
			}
			if (word[wordp].name == ")") {
				p->makenode(word[wordp].name);
				wordp++;
				return;
			}
			else {
				cout << "出错行数：" << word[wordp].row << "\n错误类型：写语句缺少右括号\n";
				system("pause");
				return;
			}
		}
		else {
			cout << "出错行数：" << word[wordp].row << "\n错误类型：写语句缺少左括号\n";
			system("pause");
			return;
		}
	}
	else {
		cout << "出错行数：" << word[wordp].row << "\n错误类型：写语句缺少关键字write\n";
		system("pause");
		return;
	}
}
//******************************************
//复合语句
void GrammaAnalysis::CompoundStatement(int level, int top, Node *p) {
	if (word[wordp].name == "begin") {
		p->makenode(word[wordp].name);
		wordp++;
		Statement(level, top, p->makenode("语句"));
		while (word[wordp].name == ";") {
			p->makenode(word[wordp].name);
			wordp++;
			Statement(level, top, p->makenode("语句"));
		}
		if (word[wordp].name == "end") {
			p->makenode(word[wordp].name);
			wordp++;
			return;
		}
		else {
			cout << "出错行数：" << word[wordp].row << "\n错误类型：复合语句缺少关键字end\n";
			system("pause");
			return;
		}
	}
	else {
		cout << "出错行数：" << word[wordp].row << "\n错误类型：复合语句缺少关键字begin\n";
		system("pause");
		return;
	}
}

//******************************************
//语句
void GrammaAnalysis::Statement(int level, int top, Node *p) {

	if (word[wordp].sym == IDENT) {
		AssignmentStatement(level, top, p->makenode("赋值语句"));
	}
	else if (word[wordp].name == "if") {
		ConditionalStatement(level, top, p->makenode("条件语句"));
	}
	else if (word[wordp].name == "while") {
		LoopStatement(level, top, p->makenode("循环语句"));
	}
	else if (word[wordp].name == "call") {
		ProcedureCallStatement(level, top, p->makenode("过程调用语句"));
	}
	else if (word[wordp].name == "read") {
		ReadStatement(level, top, p->makenode("读语句"));
	}
	else if (word[wordp].name == "write") {
		WriteStatement(level, top, p->makenode("写语句"));
	}
	else if (word[wordp].name == "begin") {
		CompoundStatement(level, top, p->makenode("复合语句"));
	}
	if (p->count == 0) {
		p->makenode("空");
	}
}


//******************************************
//分程序
void GrammaAnalysis::SubProgram(int level, int top, Node *p) {

	
	int cx0 = cx;//保存当前分程序在指令表的起始位置
	int tx0 = tx - 1;//保存当前分程序在符号表中的起始位置
	dx = 3;//分程序中相对位置置初值为3
	level++;//层次加一
	
	codes.gen(5, 0, 0);//生成一条跳转语句，跳转地址暂时为0
	cx++;//指令条数加1

	if(word[wordp].name == "const")
		Const(level, top, p->makenode("常量说明部分"));//常量说明部分
	if(word[wordp].name == "var")
		Varible(level, top, p->makenode("变量说明部分"));//变量说明部分

	int dx0 = dx;//保存当前分程序中，常量、变量说明之后的相对位置（因为过程说明里会递归调用当前程序且dx是全局变量而导致dx改变，所以先暂时保存）
	if(word[wordp].name=="procedure")
		Procedure(level, top, p->makenode("过程说明部分"));//过程说明部分
	dx = dx0;
	if (tx0 >= 0 && symtable.table[tx0].kind == "procedure") {
		symtable.table[tx0].addr = cx;//返填符号表中procedure中的addr
	}

	codes.code[cx0].a = cx;//返填指令表中该分程序的跳转语句的地址
	codes.gen(4, 0, dx);//生成int语句
	cx++;//指令条数加一

	if (word[wordp].sym == IDENT || word[wordp].name == "begin" || word[wordp].name == "if" ||  word[wordp].name =="while"
		|| word[wordp].name == "read" || word[wordp].name == "write" || word[wordp].name == "call") {
		Statement(level, top, p->makenode("语句"));//语句
	}
	codes.gen(7, 0, 0);
	cx++;
	

}
//******************************************
//程序
void GrammaAnalysis::Program(Node*p) {
	int level = -1;//层次初始化为-1
	int top = 0;
	SubProgram(level, top, p->makenode("分程序"));//分程序
	if (word[wordp].name == ".") {
		p->makenode(".");
		wordp++;
		cout << "语法分析成功\n";
	}
	else {
		cout << "出错行数：" << word[wordp].row << "\n错误类型：代码缺少\".\"\n";
	}
}
void GrammaAnalysis::gramanalysis() {
	//打开词法分析文件进行读取，将词法
	//string pfilename = "../" + filename;
	//string tempfilename = pfilename.substr(0, pfilename.length() - 4) + "_lex.txt";

	//ifstream openfile;
	//openfile.open(tempfilename);
	//string name="";
	//int sym=0;
	////string other="";
	//int row = 0;

	//while (openfile >> name >> sym >> row) {
	//	word[wordnum].name = name;
	//	word[wordnum].sym = sym;
	//	word[wordnum].row = row;
	//	wordnum++;
	//	if (wordnum >= word_max_num) {
	//		cout << "单词过多\n";
	//		return;
	//	}
	//}
	//openfile.close();
	//for (int i = 0; i < wordnum; i++) {
	//	cout << word[i] << endl;
	//}
	string tempfilename = filename.substr(0, filename.length() - 4) + "_code.txt";
	tree = new Tree(filename);//建立语法树
	
	Program(tree->getroot());//自上而下的语法分析
	tree->showtree();//打印语法树
	
	symtable.print(tempfilename);
	codes.print(tempfilename);
}
