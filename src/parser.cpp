#include "parser.h"

parser::parser(const std::string& filename) {
	wa = new lexer(filename);
	wa->readLine();

	t = wa->getSym();
	if (t.sym == INVALID)
		return;

	BLOCK();  //语法分析程序

	vector<CODE>::iterator iter = codeTable.begin();
	codeTable.insert(iter, CODE(JMP, 0, mainEntry));  //在中间代码表首加入

	generateFile(filename);

	if (!etop) {  //没有错误
		cout << endl;
		cout << "compile success" << endl;
	} else {  //输出编译错误

		cout << endl;
		for (int i = 0; i < etop; i++) {
			cout << errors[i] << endl;
		}
		cout << "compile failed" << endl;
		exit(-1);
	}
}

void parser::BLOCK() {
	lev++;
	dx = 3;

	if (t.sym == $CONST) {	//是CONST声明
		t = wa->getSym();
		if (t.sym == INVALID)
			return;

		analyzeConst();

		while (t.sym == $COMMA)	 //逗号
		{
			t = wa->getSym();
			if (t.sym == INVALID)
				return;

			analyzeConst();
		}

		if (t.sym == $SEMICOLON) {	//遇到分号

			t = wa->getSym();
			if (t.sym == INVALID)
				return;
		} else {  //既不是逗号也不是分号
			/*，声明语句没有以;结束*/
			error(10, wa->row);
		}
	}

	if (t.sym == $VAR) {  //是VAR声明

		//cout << "变量说明部分！" << endl;
		t = wa->getSym();
		if (t.sym == INVALID)
			return;

		analyzeVar();

		while (t.sym == $COMMA)	 //逗号
		{
			t = wa->getSym();
			if (t.sym == INVALID)
				return;

			analyzeVar();
		}

		if (t.sym == $SEMICOLON) {	//遇到了分号

			t = wa->getSym();
			if (t.sym == INVALID)
				return;
		}

		else {
			/*声明语句没有以;结束*/
			error(13, wa->row);
		}
	}

	if (t.sym == $PROCEDURE) {	//是过程说明

		// cout << "过程说明部分！" << endl;

		t = wa->getSym();
		if (t.sym == INVALID)
			return;

		analyzePro();
	}
	//}
	/*说明部分结束*/
	if (lev == 0)  //主程序层.ppp
		mainEntry = cx + offset;

	GEN(INT, 0, dx);  // 生成空间分配指令，分配dx个空间（3个空间+变量的数目）

	analyzeSent();	//处理遇到的语句

	GEN(OPR, 0, 0);	 //退出这个过程
}

void parser::GEN(FunctionCode fun, int lev, int offset) {
	codeTable.push_back(CODE(fun, lev, offset));
	cx++;
}

void parser::analyzeConst() {  //常量说明部分

	//cout << "常量说明部分！" << endl;

	if (t.sym == $IDENT) {	//获取标识符

		char* id = new char[MAX_ID_LEN];
		strcpy(id, t.name);

		t = wa->getSym();
		if (t.sym == INVALID)
			return;

		if (t.sym == $ASSIGN)  //是赋值号
		{
			// 如果不是等号，而是赋值符号:=，抛出1号错误
			error(19, wa->row);
		} else if (t.sym == $EQ) {
			t = wa->getSym();
			if (t.sym == INVALID)
				return;

			if (t.sym == $NUMBER) {	 //是数字

				if (!insertSymbol(SymbolKind::CONST, id))
					error(109, wa->row);
				/*重定义*/;	 //插入到符号表失败

				t = wa->getSym();
				if (t.sym == INVALID)
					return;
			}

			else {	//赋值号后不是数字

				error(18, wa->row);
			}
		}

		else {	//标识符后不是等号

			error(19, wa->row);
		}

	} else {  //const 后没有加标识符

		error(11, wa->row);
	}
}

void parser::analyzeVar() {	 //变量说明部分

	//cout << "变量说明部分！" << endl;

	if (t.sym == $IDENT) {
		char* id = new char[MAX_ID_LEN];
		strcpy(id, t.name);
		//char* id = t.name;

		if (!insertSymbol(SymbolKind::VAR, id))
			error(109, wa->row);
		/*重定义*/

		t = wa->getSym();
		if (t.sym == INVALID)
			return;	 //取下一个词
	} else {
		// 如果变量声明过程中遇到的第一个字符不是标识符
		error(12, wa->row);
	}
}

void parser::analyzePro() {	 //过程说明部分

	if (t.sym == $IDENT) {
		char* id = new char[MAX_ID_LEN];
		strcpy(id, t.name);
		if (!insertSymbol(SymbolKind::PROD, id))
			error(109, wa->row);
		/*重定义*/

		t = wa->getSym();
		if (t.sym == INVALID)
			return;
	} else {
		/*过程首部没有标识符*/
		error(20, wa->row);
	}

	if (t.sym == $SEMICOLON) {
		t = wa->getSym();
		if (t.sym == INVALID)
			return;
	} else {  //过程名没有以分号结束
		error(17, wa->row);
	}

	//保存当前dx和lev
	int tempdx = dx;
	int templev = lev;

	BLOCK();  //分程序，内部过程说明表的构造

	//恢复
	dx = tempdx;
	lev = templev;

	if (t.sym == $SEMICOLON) {	//end;后的符号。这里有问题

		t = wa->getSym();
		if (t.sym == INVALID)
			return;

		while (t.sym == $PROCEDURE) {  //如果还有并列的过程说明

			t = wa->getSym();
			analyzePro();
		}
	} else {
		/*过程说明end后没有加分号*/
		error(22, wa->row);
	}
}

void parser::analyzeSent() {
	int i;
	int cx1;
	int cx2;

	switch (t.sym) {
	case $IDENT: { /*赋值语句*/

		i = searchSymbol(t.name);
		if (i < 0) { /*没有找到符号的声明*/
			error(108, wa->row);
		}

		else if (symbolTable.at(i).kind != SymbolKind::VAR) {
			// 如果在符号表中找到了，但是该标识符不是变量名，
			/*不可修改的左值*/
			error(110, wa->row);
		}

		t = wa->getSym();
		if (t.sym == INVALID)
			return;

		if (t.sym == $ASSIGN) {	 //是赋值号

			t = wa->getSym();
			if (t.sym == INVALID)
				return;
		} else {
			/*变量名后没有加赋值号*/
			error(14, wa->row);
		}

		analyzeExpr();	//处理赋值号右部的表达式

		if (i >= 0) {
			GEN(STO, lev - symbolTable.at(i).lev, symbolTable.at(i).addr);
		}

		break;
	}

	case $CALL: {
		t = wa->getSym();
		if (t.sym == INVALID)
			return;

		if (t.sym != $IDENT) {	//call 后面不是标识符
			error(34, wa->row);
		} else {
			i = searchSymbol(t.name);  //查找标识符对应的符号表

			if (i < 0) {
				error(108, wa->row);
			}

			else if (symbolTable.at(i).kind == SymbolKind::PROD) {	//类型检查

				GEN(CAL, lev - symbolTable.at(i).lev, symbolTable.at(i).addr);
			}

			else {	//call 后加的不是proc

				/*调用的标示符不是过程名*/
				error(34, wa->row);
			}

			t = wa->getSym();
			if (t.sym == INVALID)
				return;
		}

		break;
	}

	case $IF: {
		t = wa->getSym();
		if (t.sym == INVALID)
			return;

		analyzeCond();	//处理if后面的条件
		cx1 = cx;		//jpc语句占用的位置
		cx++;

		if (t.sym == $THEN) {
			t = wa->getSym();
			if (t.sym == INVALID)
				return;

			analyzeSent();														   //then后的语句
			codeTable.insert(codeTable.begin() + cx1, CODE(JPC, 0, cx + offset));  //回填then语句之后的代码地址

		} else {
			/*条件语句缺少then*/
			error(36, wa->row);
		}

		//cx = codeTable.size();

		//codeTable.push_back(CODE(JPC, 0, 0));
		//codeTable.at(cx1).offset = codeTable.size();
		break;
	}

	case $BEGIN: {
		t = wa->getSym();
		if (t.sym == INVALID)
			return;

		analyzeSent();

		while (t.sym != INVALID) {	//这里写的有点乱

			if (t.sym == $SEMICOLON) {	//t为";",说明后面还有语句

				t = wa->getSym();
				if (t.sym == INVALID)
					return;

				analyzeSent();
			} else if (t.sym == $END) {	 //t为end，表示复合语句结束

				t = wa->getSym();
				//if (t.sym == INVALID) return;
				return;
			}

			else {
				//出现其他字符，则说明复合语句错误
				error(23, wa->row);
				return;
			}
			//analyzeSent();
		}

		/*复合语句出错*/
		break;
	}

	case $WHILE: {
		cx1 = cx;  // 记录当前代码分配位置，这是while循环的开始位置，也就是无条件跳转到的地方

		t = wa->getSym();
		if (t.sym == INVALID)
			return;

		analyzeCond();	//判断while后面的条件语句

		cx2 = cx;  //jpc语句插入的地方
		cx++;	   //占一个Code位置

		if (t.sym == $DO) {
			t = wa->getSym();
			if (t.sym == INVALID)
				return;

			analyzeSent();

			GEN(JMP, 0, cx1 + offset);	//跳转到cx1处，即再次进行判断是否进行循环
			codeTable.insert(codeTable.begin() + cx2, CODE(JPC, 0, cx + offset));
		} else {
			/*while后面没有接do*/
			error(39, wa->row);
		}

		break;
	}

	case $READ: {
		t = wa->getSym();
		if (t.sym == INVALID)
			return;

		if (t.sym == $LPAIR) {
			t = wa->getSym();
			if (t.sym == INVALID)
				return;

			if (t.sym == $IDENT) {
				i = searchSymbol(t.name);
				if (i < 0) {
					/*未说明的标识符*/
					error(108, wa->row);
				}

				else {
					//codeTable.push_back(CODE(OPR, 0, OPR::READ));// 生成16号读指令，从键盘读取数字
					//codeTable.push_back(CODE(STO, lev - symbolTable.at(i).lev, symbolTable.at(i).addr));// 生成sto指令，存入指定变量
					GEN(OPR, 0, OPR::READ);
					GEN(STO, lev - symbolTable.at(i).lev, symbolTable.at(i).addr);
				}
			}

			t = wa->getSym();
			if (t.sym == INVALID)
				return;

			while (t.sym == $COMMA) {  //读多个键盘输入

				t = wa->getSym();
				if (t.sym == INVALID)
					return;

				if (t.sym == $IDENT) {
					i = searchSymbol(t.name);
					if (i < 0) {
						/*没找到标识符*/
						error(108, wa->row);
					} else {
						//	codeTable.push_back(CODE(OPR, 0, OPR::READ));//读键盘
						//codeTable.push_back(CODE(STO, lev - symbolTable.at(i).lev, symbolTable.at(i).addr));
						GEN(OPR, 0, OPR::READ);
						GEN(STO, lev - symbolTable.at(i).lev, symbolTable.at(i).addr);
					}
				}

				t = wa->getSym();
				if (t.sym == INVALID)
					return;
			}

			if (t.sym != $RPAIR) {
				/*左右括号不匹配*/
				error(27, wa->row);
			}

		} else {
			/*不是左括号*/
			error(29, wa->row);
		}

		t = wa->getSym();
		if (t.sym == INVALID)
			return;

		break;
	}

	case $WRITE: {
		t = wa->getSym();
		if (t.sym == INVALID)
			return;

		if (t.sym == $LPAIR) {
			t = wa->getSym();
			analyzeExpr();
			GEN(OPR, 0, OPR::WRITE);

			while (t.sym == $COMMA) {
				t = wa->getSym();
				if (t.sym == INVALID)
					return;

				analyzeExpr();

				GEN(OPR, 0, OPR::WRITE);
			}

			if (t.sym != $RPAIR) {
				/*左右括号不匹配*/
				error(31, wa->row);
			} else {
				t = wa->getSym();
				if (t.sym == INVALID)
					return;
			}
		}

		break;
	}

	default:  //不是begin开头
		//error(8, wa->row);
		break;
	}
}

void parser::analyzeCond() {
	int relop;
	if (t.sym == $ODD) {  //一元运算符

		t = wa->getSym();
		if (t.sym == INVALID)
			return;

		analyzeExpr();	//分析表达式

		//codeTable.push_back(CODE(OPR, 0, OPR::ODD));//生成奇偶判断指令
		GEN(OPR, 0, OPR::ODD);
	}

	else {	//二元运算符

		analyzeExpr();	//分析左边表达式
		relop = t.sym;	//保存二元运算符

		t = wa->getSym();
		if (t.sym == INVALID)
			return;

		analyzeExpr();	//分析右边表达式

		switch (relop) {
		case $EQ:
			//codeTable.push_back(CODE(OPR, 0, OPR::EQ));
			GEN(OPR, 0, OPR::EQ);
			break;

		case $UEQ:
			//codeTable.push_back(CODE(OPR, 0, OPR::UE));
			GEN(OPR, 0, OPR::UE);
			break;

		case $LOWER:
			//codeTable.push_back(CODE(OPR, 0, OPR::LT));
			GEN(OPR, 0, OPR::LT);
			break;

		case $BIGGER_EQ:
			//codeTable.push_back(CODE(OPR, 0,OPR::GE));
			GEN(OPR, 0, OPR::GE);
			break;

		case $BIGGER:
			//codeTable.push_back(CODE(OPR, 0, OPR::GT));
			GEN(OPR, 0, OPR::GT);
			break;

		case $LOWER_EQ:
			//codeTable.push_back(CODE(OPR, 0, OPR::LE));
			GEN(OPR, 0, OPR::LE);
			break;

		default:
			/*保存的不是逻辑运算符*/
			break;
		}
	}
}

void parser::analyzeExpr() {
	int addop;
	if (t.sym == $PLUS || t.sym == $MINUS) {  //表达式以正负开头

		addop = t.sym;

		t = wa->getSym();
		if (t.sym == INVALID)
			return;

		analyzeTerm();	//进行项的处理

		if (addop == $MINUS) {
			//codeTable.push_back(CODE(OPR, 0, OPR::MINUS));//取反运算
			GEN(OPR, 0, OPR::MINUS);
		}
	} else {
		analyzeTerm();	//表达式项的开头，直接项的分析
	}

	while (t.sym == $PLUS || t.sym == $MINUS) {
		addop = t.sym;	//算术运算符保存在addop中

		t = wa->getSym();
		if (t.sym == INVALID)
			return;

		analyzeTerm();

		if (addop == $PLUS) {  // 项分析完毕后，如果刚才保存的是加号，则生成加法指令

			//codeTable.push_back(CODE(OPR, 0, OPR::ADD));
			GEN(OPR, 0, OPR::ADD);	//加法指令
		} else {
			//codeTable.push_back(CODE(OPR, 0, OPR::SUB));//减法指令
			GEN(OPR, 0, OPR::SUB);
		}
	}
}

void parser::analyzeTerm() {
	int mulop;

	analyzeElem();

	while (t.sym == $STAR || t.sym == $DIV) {
		mulop = t.sym;	/// 把运算符保存在mulop中(乘法或是除法)

		t = wa->getSym();
		if (t.sym == INVALID)
			return;

		analyzeElem();	// 运算符后应该是一个因子，进行因子分析

		if (mulop == $STAR) {
			//codeTable.push_back(CODE(OPR, 0, OPR::MUL));//4号乘法指令
			GEN(OPR, 0, OPR::MUL);
		} else {
			//codeTable.push_back(CODE(OPR, 0, OPR::DIV));//除法指令
			GEN(OPR, 0, OPR::DIV);
		}
	}
}

void parser::analyzeElem() {
	int i;
	while (t.sym == $IDENT || t.sym == $NUMBER || t.sym == $LPAIR) {
		switch (t.sym) {
		case $IDENT: {
			i = searchSymbol(t.name);
			if (i < 0) {
				/*没有找到符号*/
				error(108, wa->row);
			}

			else if (symbolTable.at(i).kind == SymbolKind::CONST) {
				// 如果该标识符为常量,则生成lit指令,把val放到栈顶
				//codeTable.push_back(CODE(LIT, 0, symbolTable.at(i).val));
				GEN(LIT, 0, symbolTable.at(i).val);

				t = wa->getSym();
				if (t.sym == INVALID)
					return;
			}

			else if (symbolTable.at(i).kind == SymbolKind::VAR) {
				//codeTable.push_back(CODE(LOD, lev - symbolTable.at(i).lev,
				//symbolTable.at(i).addr));
				GEN(LOD, lev - symbolTable.at(i).lev,
					symbolTable.at(i).addr);

				t = wa->getSym();
				if (t.sym == INVALID)
					return;
			} else if (symbolTable.at(i).kind == SymbolKind::PROD) {
				/*该标识符为过程名，出错*/
				error(43, wa->row);
				t = wa->getSym();
				if (t.sym == INVALID)
					return;
			}

			break;
		}

		case $NUMBER:  //因子分析遇到数字
		{
			int num = atoi(t.name); /*要判断范围*/

			//codeTable.push_back(CODE(LIT, 0, num));// 生成lit指令，把这个数值字面常量放到栈顶
			GEN(LIT, 0, num);

			t = wa->getSym();
			if (t.sym == INVALID)
				return;

			break;
		}

		case $LPAIR: {
			t = wa->getSym();
			if (t.sym == INVALID)
				return;

			analyzeExpr();

			if (t.sym == $RPAIR) {
				t = wa->getSym();
				if (t.sym == INVALID)
					return;
			} else {
				/*左右括号不匹配*/
				error(41, wa->row);
			}

			break;
		}
		}
	}
}

bool parser::insertSymbol(SymbolKind kind, const char* id) {
	if (searchSymbol(id) >= 0)	//已经找到符号的声明
		return false;

	switch (kind) {
	case SymbolKind::CONST: {
		int num = atoi(t.name);	 //越界检查
		symbolTable.push_back(SYMBOL(id, kind, num, 0, 0));
		break;
	}

	case SymbolKind::VAR: {
		symbolTable.push_back(SYMBOL(id, kind, 0xFFFFFFFF, lev, dx++));	 //变量默认值是-1
		break;
	}

	case SymbolKind::PROD: {
		symbolTable.push_back(SYMBOL(id, kind, 0, lev, cx + offset));
		break;
	}
	default:
		break;
	}

	return true;
}

int parser::searchSymbol(const char* id) {
	for (int i = 0; i < symbolTable.size(); i++) {
		SYMBOL s = symbolTable.at(i);
		if (!strcmp(id, s.name) && lev >= s.lev)  //s在当前层及以上且名字相同
			return i;
	}

	return -1;
}

int parser::error(int e, int eline) {
	ostringstream oss;

	switch (e) {
	case 0: {
		oss << e << ": " << eline << "行有非法字符";
		errors[etop++] = oss.str();
		break;
	}
	case 1: {
		//errors[etop++] = itos(e) + ": " + itos(eline) + "行有超过14个字母的单词";
		oss << e << ": " << eline << "行有超过10个字母的单词";
		errors[etop++] = oss.str();
		break;
	}
	case 2: {
		//errors[etop++] = itos(e) + ": " + itos(eline) + "行有数字开头的标识符";
		oss << e << ": " << eline << "行有数字开头的标识符";
		errors[etop++] = oss.str();
		break;
	}
	case 3: {
		//errors[etop++] = itos(e) + ": " + itos(eline) + "行有超过10个字母的标识符";
		oss << e << ": " << eline << "行有超过10个字母的标识符";
		errors[etop++] = oss.str();
		break;
	}
	case 4: {
		//errors[etop++] = itos(e) + ": " + itos(eline) + "行程序结尾没有.";
		oss << e << ": " << eline << "行程序结尾没有.";
		errors[etop++] = oss.str();
		break;
	}
	case 5: {
		//errors[etop++] = itos(e) + ": " + itos(eline) + "行程序首部标识符后面没有;";
		oss << e << ": " << eline << "行程序首部标识符后面没有;";
		errors[etop++] = oss.str();
		break;
	}
	case 6: {  //
		//errors[etop++] = itos(e) + ": " + itos(eline) + "行程序首部PRAGRAM后面没有标识符";
		oss << e << ": " << eline << "行程序首部PROCEDURE后面没有标识符";
		errors[etop++] = oss.str();
		break;
	}
	case 7: {
		//errors[etop++] = itos(e) + ": " + itos(eline) + "行没有程序首部";
		oss << e << ": " << eline << "行没有程序首部";
		errors[etop++] = oss.str();
		break;
	}
	case 8: {
		oss << e << ": " << eline << "行分程序后缺少语句部分";
		errors[etop++] = oss.str();
		break;
	}
	case 9: {
		oss << e << ": " << eline << "行常量说明的,字符后面有错误";
		errors[etop++] = oss.str();
		break;
	}
	case 10: {
		oss << e << ": " << eline << "行常量说明后面没有;结尾";
		errors[etop++] = oss.str();
		break;
	}
	case 11: {
		oss << e << ": " << eline << "行常量定义有错误";
		errors[etop++] = oss.str();
		break;
	}
	case 12: {
		oss << e << ": " << eline << "行变量说明,后面缺少标识符";
		errors[etop++] = oss.str();
		break;
	}
	case 13: {
		oss << e << ": " << eline << "行变量说明没有;结尾";
		errors[etop++] = oss.str();
		break;
	}
	case 14: {
		oss << e << ": " << eline << "行VAR后面没有定义的标识符";
		errors[etop++] = oss.str();
		break;
	}

	case 15: {
		oss << e << ": " << eline << "行分程序后面没有;结尾";
		errors[etop++] = oss.str();
		break;
	}
	case 16: {
		oss << e << ": " << eline << "行过程说明后面没有分程序";
		errors[etop++] = oss.str();
		break;
	}
	case 17: {
		oss << e << ": " << eline << "行过程首部后面没有;结尾";
		errors[etop++] = oss.str();
		break;
	}
	case 18: {
		oss << e << ": " << eline << "行常量定义没有指定数字";
		errors[etop++] = oss.str();
		break;
	}
	case 19: {
		oss << e << ": " << eline << "行常量定义没有=字符";
		errors[etop++] = oss.str();
		break;
	}
	case 20: {
		oss << e << ": " << eline << "行过程首部PROCEDURE后面并没有标识符";
		errors[etop++] = oss.str();
		break;
	}
	case 21: {
		oss << e << ": " << eline << "行语句后面没有;结尾";
		errors[etop++] = oss.str();
		break;
	}
	case 22: {
		oss << e << ": " << eline << "行没有END结尾，或者此行语句缺少;结尾";
		errors[etop++] = oss.str();
		break;
	}
	case 23: {
		oss << e << ": " << eline << "行复合语句的BEGIN后面不是语句";
		errors[etop++] = oss.str();
		break;
	}
	case 24: {
		oss << e << ": " << eline << "行赋值语句后面不是表达式";
		errors[etop++] = oss.str();
		break;
	}
	case 25: {
		oss << e << ": " << eline << "行赋值语句没有:=赋值号";
		errors[etop++] = oss.str();
		break;
	}
	case 26: {
		oss << e << ": " << eline << "行READ语句参数中有非法,字符";
		errors[etop++] = oss.str();
		break;
	}
	case 27: {
		oss << e << ": " << eline << "行READ语句缺少)字符";
		errors[etop++] = oss.str();
		break;
	}
	case 28: {
		oss << e << ": " << eline << "行READ语句缺少标识符";
		errors[etop++] = oss.str();
		break;
	}
	case 29: {
		oss << e << ": " << eline << "行READ语句缺少(字符";
		errors[etop++] = oss.str();
		break;
	}
	case 30: {
		oss << e << ": " << eline << "行WRITE语句参数中有非法,字符";
		errors[etop++] = oss.str();
		break;
	}
	case 31: {
		oss << e << ": " << eline << "行WRITE语句缺少)字符";
		errors[etop++] = oss.str();
		break;
	}
	case 32: {
		oss << e << ": " << eline << "行WRITE语句缺少标识符";
		errors[etop++] = oss.str();
		break;
	}
	case 33: {
		oss << e << ": " << eline << "行WRITE语句缺少(字符";
		errors[etop++] = oss.str();
		break;
	}
	case 34: {
		oss << e << ": " << eline << "行CALL语句没有过程名字";
		errors[etop++] = oss.str();
		break;
	}
	case 35: {
		oss << e << ": " << eline << "行IF语句的THEN后面不是合法语句";
		errors[etop++] = oss.str();
		break;
	}
	case 36: {
		oss << e << ": " << eline << "行IF语句的条件后面没有THEN";
		errors[etop++] = oss.str();
		break;
	}
	case 37: {
		oss << e << ": " << eline << "行IF语句后面不是合法条件";
		errors[etop++] = oss.str();
		break;
	}
	case 38: {
		oss << e << ": " << eline << "行WHILE语句的DO后面不是合法语句";
		errors[etop++] = oss.str();
		break;
	}
	case 39: {
		oss << e << ": " << eline << "行WHILE语句没有DO";
		errors[etop++] = oss.str();
		break;
	}
	case 40: {
		oss << e << ": " << eline << "行WHILE语句后面不是合法的条件";
		errors[etop++] = oss.str();
		break;
	}
	case 41: {
		oss << e << ": " << eline << "行表达式的右边缺少)字符";
		errors[etop++] = oss.str();
		break;
	}
	case 42: {
		oss << e << ": " << eline << "行有非法表达式";
		errors[etop++] = oss.str();
		break;
	}
	case 43: {
		oss << e << ": " << eline << "行有非法因子";
		errors[etop++] = oss.str();
		break;
	}
	case 44: {
		oss << e << ": " << eline << "行有非法项";
		errors[etop++] = oss.str();
		break;
	}
	case 45: {
		oss << e << ": " << eline << "行有非法项";
		errors[etop++] = oss.str();
		break;
	}
	case 46: {
		oss << e << ": " << eline << "行有非法项";
		errors[etop++] = oss.str();
		break;
	}
	case 47: {
		oss << e << ": " << eline << "行有非法项";
		errors[etop++] = oss.str();
		break;
	}
	case 48: {
		oss << e << ": " << eline << "行关系运算符有非法表达式";
		errors[etop++] = oss.str();
		break;
	}
	case 49: {
		oss << e << ": " << eline << "行ODD有非法表达式";
		errors[etop++] = oss.str();
		break;
	}
	case 101: {
		oss << e << ": " << eline << "行给非变量赋值或变量没有定义";
		errors[etop++] = oss.str();
		break;
	}
	case 102: {
		oss << e << ": " << eline << "行read语句读入非变量或变量没有定义";
		errors[etop++] = oss.str();
		break;
	}
	case 103: {
		oss << e << ": " << eline << "行read语句读入非变量或变量没有定义";
		errors[etop++] = oss.str();
		break;
	}
	case 106: {
		oss << e << ": " << eline << "行call语句后面不是过程名称或过程没有定义";
		errors[etop++] = oss.str();
		break;
	}
	case 107: {
		oss << e << ": " << eline << "行因子出错";
		errors[etop++] = oss.str();
		break;
	}

	case 108: {
		oss << e << ": " << eline << "行该层中未声明的标识符";
		errors[etop++] = oss.str();
		break;
	}

	case 109: {
		oss << e << ": " << eline << "行标识符重复定义";
		errors[etop++] = oss.str();
		break;
	}

	case 110: {
		oss << e << ": " << eline << "行不可修改的左值";
		errors[etop++] = oss.str();
		break;
	}
	}
	return e;
}

void parser::printTable() {
	for (int i = 0; i < symbolTable.size(); i++) {
		SYMBOL symbol = symbolTable.at(i);

		cout << "name:" << symbol.name << "\t";

		if (symMap.find(symbol.kind) != symMap.end())
			cout << "sym:" << symMap[symbol.kind] << "\t";
		else
			cout << "sym:"
				 << "没有"
				 << "\t";

		cout << "val:" << symbol.val << "\t";
		cout << "lev:" << symbol.lev << "\t";
		cout << "addr:" << symbol.addr << endl;
	}
}

void parser::printCode() {
	for (int i = 0; i < codeTable.size(); i++) {
		CODE inst = codeTable.at(i);

		cout << "No:" << i << "\t";
		cout << "op:" << opMap[inst.fun] << "\t";
		cout << "lev:" << inst.lev << "\t";
		cout << "addr:" << inst.offset << endl;
	}
}

void parser::generateFile(const std::string& filename) {
	// char _Drive[9];	 //文件后缀
	// char _Dir[99];
	// char _Filename[99];

	// _splitpath(filename, _Drive, _Dir, _Filename, NULL);

	// char targetSym[99], targetCode[99];	 //符号表文件和目标代码文件

	// strcat(targetSym, ".sym");

	// strcat(targetCode, ".pl0");

	FILE *fp1, *fp2;
	if ((fp1 = fopen((filename + ".sym").c_str(), "wb")) == NULL) {
		cout << "创建.sym文件失败!" << endl;
		exit(-1);
	}
	for (int i = 0; i < symbolTable.size(); i++) {
		fwrite(&symbolTable[i], sizeof(SYMBOL), 1, fp1);
	}

	if ((fp2 = fopen((filename + ".pl0cache").c_str(), "wb")) == NULL) {
		cout << "创建.pl0文件失败!" << endl;
		exit(-1);
	}
	for (int i = 0; i < codeTable.size(); i++) {
		fwrite(&codeTable[i], sizeof(CODE), 1, fp2);
	}

	fclose(fp1);
	fclose(fp2);
}
