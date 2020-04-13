#include "symboltable.h"
int top = 0;
//******************************************//////////////////////////////////////////
//mode=0，常量变量；mode=1，过程；
int SymbolTable::search(string name, int level, int mode) {

	//for (int i = (tablenum - 1); i >= 0; i--) {
	//	if (table[i].kind == "const" && table[i].name == name) {
	//		return i;
	//	}

	//}
	if (mode==0) {//const或varible查询
		//const查询
		for (int i = (tablenum - 1); i >= 0; i--) {
			if (table[i].kind == "const" && table[i].name == name) {
				return i;
			}

		}


		int templevel = level;
        //varible查询，当前层次，父层次，父父层次……
		for (int i = (tablenum - 1); i >= 0; i--) {
			if (templevel > table[i].level) {
				templevel = table[i].level;
			}

			if (templevel == table[i].level && table[i].name == name) {
				return i;
			}

		}
		return -1;
	}
	else if(mode == 1){//procedure查询
		
		int templevel = level;
		int step = 0;
		for (int i = (tablenum - 1); i >= 0; i--) {
			if (templevel > table[i].level) {
				templevel = table[i].level;
				step++;
			}
			if (step == 2) {
				break;
			}

			if (templevel == table[i].level && table[i].name == name) {
				return i;
			}

		}

		
	}
	

	return -1;
}
//******************************************
//向符号表中添加一项，并根据类型进行相应的判断
void SymbolTable::enter(string name, string type, int value, int level, int addr) {

	if (type == "const") {
		//cout << "将const型变量" << name << "放入table中，且value=" << value << "\n";
		table[tablenum].name = name;
		table[tablenum].kind = type;
		table[tablenum].value = value;
		table[tablenum].top = top;
	}
	else if (type == "varible") {
		//cout << "将varible型变量" << name << "放入table中，且level=" << level << ",addr=" << addr << "\n";
		table[tablenum].name = name;
		table[tablenum].kind = type;
		table[tablenum].level = level;
		table[tablenum].addr = addr;
		table[tablenum].top = top;
	}
	else if (type == "procedure") {
		//cout << "将procedure型变量" << name << "放入table中，且level=" << level << ",addr=" << addr << "\n";
		table[tablenum].name = name;
		table[tablenum].kind = type;
		table[tablenum].level = level;
		table[tablenum].addr = addr;
		table[tablenum].top = ++top;
	}
	tablenum++;
}
void SymbolTable::print(string tp) {
	ofstream df;
	df.open(tp, ios::out);
	if (!df.is_open() ) {
		cout << "错误！文件未打开成功\n";
		return ;
	}
	cout << "符号表：\n";
	df << "符号表：\n";
	cout << left << setw(10) << "name" << left << setw(10) << "kind" << left << setw(10) << "value" << left << setw(10) <<
		"level" << left << setw(10) << "addr" << left << setw(10) << "top" <<"\n";
	
	df << left << setw(10) << "name" << left << setw(10) << "kind" << left << setw(10) << "value" << left << setw(10) <<
		"level" << left << setw(10) << "addr" << left << setw(10) << "top" << "\n";
	for (int i = 0; i < tablenum; i++) {
		if (table[i].kind == "const") {
			cout << left << setw(10) << table[i].name << left << setw(10) << table[i].kind << left << setw(10) << table[i].value << left << setw(10) <<
				"--" << left << setw(10) << "--" << left << setw(10) << table[i].top << "\n";
			df<< left << setw(10) << table[i].name << left << setw(10) << table[i].kind << left << setw(10) << table[i].value << left << setw(10) <<
				"--" << left << setw(10) << "--" << left << setw(10) << table[i].top << "\n";
		}
		else if (table[i].kind == "varible") {
			cout << left << setw(10) << table[i].name << left << setw(10) << table[i].kind << left << setw(10) << "--" << left << setw(10) <<
				table[i].level << left << setw(10) << table[i].addr << left << setw(10) << table[i].top << "\n";
		    df << left << setw(10) << table[i].name << left << setw(10) << table[i].kind << left << setw(10) << "--" << left << setw(10) <<
				table[i].level << left << setw(10) << table[i].addr << left << setw(10) << table[i].top << "\n";
		}
		else if (table[i].kind == "procedure") {
			cout << left << setw(10) << table[i].name << left << setw(10) << table[i].kind << left << setw(10) << "--" << left << setw(10) <<
				table[i].level << left << setw(10) << table[i].addr << left << setw(10) << table[i].top << "\n";
			df<< left << setw(10) << table[i].name << left << setw(10) << table[i].kind << left << setw(10) << "--" << left << setw(10) <<
				table[i].level << left << setw(10) << table[i].addr << left << setw(10) << table[i].top << "\n";
		}
		
	}
}