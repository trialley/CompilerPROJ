#include "symboltable.h"
int top = 0;
//******************************************//////////////////////////////////////////
//mode=0������������mode=1�����̣�
int SymbolTable::search(string name, int level, int mode) {

	//for (int i = (tablenum - 1); i >= 0; i--) {
	//	if (table[i].kind == "const" && table[i].name == name) {
	//		return i;
	//	}

	//}
	if (mode==0) {//const��varible��ѯ
		//const��ѯ
		for (int i = (tablenum - 1); i >= 0; i--) {
			if (table[i].kind == "const" && table[i].name == name) {
				return i;
			}

		}


		int templevel = level;
        //varible��ѯ����ǰ��Σ�����Σ�������Ρ���
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
	else if(mode == 1){//procedure��ѯ
		
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
//����ű������һ����������ͽ�����Ӧ���ж�
void SymbolTable::enter(string name, string type, int value, int level, int addr) {

	if (type == "const") {
		//cout << "��const�ͱ���" << name << "����table�У���value=" << value << "\n";
		table[tablenum].name = name;
		table[tablenum].kind = type;
		table[tablenum].value = value;
		table[tablenum].top = top;
	}
	else if (type == "varible") {
		//cout << "��varible�ͱ���" << name << "����table�У���level=" << level << ",addr=" << addr << "\n";
		table[tablenum].name = name;
		table[tablenum].kind = type;
		table[tablenum].level = level;
		table[tablenum].addr = addr;
		table[tablenum].top = top;
	}
	else if (type == "procedure") {
		//cout << "��procedure�ͱ���" << name << "����table�У���level=" << level << ",addr=" << addr << "\n";
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
		cout << "�����ļ�δ�򿪳ɹ�\n";
		return ;
	}
	cout << "���ű�\n";
	df << "���ű�\n";
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