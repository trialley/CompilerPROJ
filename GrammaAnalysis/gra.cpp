#include "gra.h"
/*�Ӵʷ������׶��ù���������*/
extern char symTB[29][15];//�ؼ�������������
extern Lexword word[word_max_num];//���ʱ�
extern int wordnum;//���ʱ��е�������



int wordp = 0;//���ʱ�ָ��


Codes codes;//Ŀ�������
int dx = 3;//��ǰ����ֲ�������Ե�ַ
int cx = 0;//��������
int tx = 0;//���ű�����

Lexword td;//��ʱ�ĵ�����




GrammaAnalysis::GrammaAnalysis(string filename) {
	this->filename = "../" + filename;
}


//******************************************
//�Ӵʷ���������ж�ȡһ���ַ���������Ǳ�ʶ���򷵻��������򷵻ؿ�
string GrammaAnalysis::IDent(Node *p) {
	//��ʶ��
	
	if (word[wordp].sym == IDENT) {
		p->makenode(word[wordp].name);
		return word[wordp++].name;
	}
	else {
		cout << "����������"<<word[wordp].row<<"\n�������ͣ�ȱ�ٱ�ʶ��\n";
		system("pause");
		return "";
	}

}
//******************************************
//�Ӵʷ���������ж�ȡһ���ַ���������������򷵻��������򷵻ؿ�
string GrammaAnalysis::UnsignedNum(Node *p) {
	//�޷�����
	if (word[wordp].sym == NUMBER) {
		p->makenode(word[wordp].name);
		return word[wordp++].name;
	}
	else {
		cout << "����������" << word[wordp].row << "\n�������ͣ�ȱ������\n";
		system("pause");
		return "";
	}
}
//******************************************
//�Ӵʷ���������ж�ȡһ���ַ���������ǹ�ϵ������򷵻��������򷵻ؿ�
string GrammaAnalysis::Roperator(Node *p) {
	//�޷�����
	if (word[wordp].sym >= 17 && word[wordp].sym <= 22) {
		p->makenode(word[wordp].name);
		return word[wordp++].name;
	}
	else {
		//cout << word[wordp].name << "���ǹ�ϵ�����\n";
		cout << "����������" << word[wordp].row << "\n�������ͣ�ȱ�ٹ�ϵ�����\n";
		system("pause");
		return "";
	}
}
//******************************************
//�Ӵʷ���������ж�ȡһ���ַ���������ǼӼ�������򷵻��������򷵻ؿ�
string GrammaAnalysis::ASoperator(Node *p) {
	//�޷�����
	if (word[wordp].name == "+" && word[wordp].name == "-") {
		p->makenode(word[wordp].name);
		return word[wordp++].name;
	}
	else {
		cout << "����������" << word[wordp].row << "\n�������ͣ�ȱ�ټӼ������\n";
		system("pause");
		return "";
	}
}
//******************************************
//�Ӵʷ���������ж�ȡһ���ַ���������ǳ˳�������򷵻��������򷵻ؿ�
string GrammaAnalysis::MDoperator(Node *p) {
	//�޷�����
	if (word[wordp].name == "*" && word[wordp].name == "/") {
		p->makenode(word[wordp].name);
		return word[wordp++].name;
	}
	else {
		cout << "����������" << word[wordp].row << "\n�������ͣ�ȱ�ٳ˳������\n";
		system("pause");
		return "";
	}
}

//******************************************
//��������
void GrammaAnalysis::ConstantDefinition(int level, int top, Node *p) {
	string tempnum;
	int tempnums;
	string tempid=IDent(p->makenode("��ʶ��"));//��ʶ��
	int i = symtable.search(tempid, level, 0);
	if (i != -1) {
		cout << "����������" << word[wordp].row << "\n�������ͣ���ʶ���ض���\n";
		system("pause");
		return;
	}
	if (word[wordp].name == "=") {
		p->makenode("=");
		wordp++;

		tempnum = UnsignedNum(p->makenode("����"));//�޷�������
		tempnums = atoi(tempnum.c_str());
		symtable.enter(tempid,"const",tempnums,-1,-1);//��ӵ����ű�
		tx++;//���ű�������һ
			
	}
	else {
		cout << "����������" << word[wordp].row << "\n�������ͣ��������岿��ȱ�ٵ��ں�\n";
		system("pause");
		return;
	}
}
//******************************************
//����˵������
void GrammaAnalysis::Const(int level, int top, Node *p) {
	if (word[wordp].name == "const") {
		p->makenode("const");
		wordp++;
		ConstantDefinition(level, top, p->makenode("��������"));
		while (word[wordp].name == ",") {
			p->makenode(",");
			wordp++;
			ConstantDefinition(level, top, p->makenode("��������"));
		}
		if (word[wordp].name == ";") {
			p->makenode(";");
			wordp++;
		}
	}
	else {
		cout << "����������" << word[wordp].row << "\n�������ͣ�����˵������ȱ�ٹؼ���const\n";
		system("pause");
		return;
	}
	
}
//******************************************
//����˵������
void GrammaAnalysis::Varible(int level, int top, Node *p) {
	if (word[wordp].name == "var") {
		p->makenode("var");
		wordp++;
		string tempid=IDent(p->makenode("��ʶ��"));
		int i = symtable.search(tempid, level, 1);
		if (i != -1) {
			cout << "����������" << word[wordp].row << "\n�������ͣ���ʶ���ض���\n";
			system("pause");
			return;
		}
		
		symtable.enter(tempid, "varible", -1, level, dx++);
		tx++;


		while (word[wordp].name == ",") {
			p->makenode(",");
			wordp++;
			tempid = IDent(p->makenode("��ʶ��"));
			int i = symtable.search(tempid, level, 0);
			if (i != -1) {
				cout << "����������" << word[wordp].row << "\n�������ͣ���ʶ���ض���\n";
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
			cout << "����������" << word[wordp].row << "\n�������ͣ�����˵������ȱ�ٷֺ�\n";
			system("pause");
			return;
		}
	}
	else {
		cout << "����������" << word[wordp].row << "\n�������ͣ�����˵������ȱ�ٹؼ���var\n";
		system("pause");
		return;
	}
	
}
//******************************************
//�����ײ�˵������
void GrammaAnalysis::ProcedureHeader(int level, int top, Node *p) {
	if (word[wordp].name == "procedure") {
		p->makenode(word[wordp].name);
		wordp++;
		string tempid = IDent(p->makenode("��ʶ��"));
		symtable.enter(tempid, "procedure", -1, level, 0);
		tx++;

		if (word[wordp].name == ";") {
			p->makenode(";");
			wordp++;
		}
		else {
			cout << "����������" << word[wordp].row << "\n�������ͣ������ײ�����ȱ�ٷֺ�\n";
			system("pause");
			return;
		}
	}
}
//******************************************
//����˵������
void GrammaAnalysis::Procedure(int level, int top, Node *p) {
	ProcedureHeader(level, top, p->makenode("�����ײ�"));
	top++;//Ŀǰ��ʱ��������top������
	SubProgram(level, top, p->makenode("�ֳ���"));


	if (word[wordp].name == ";") {
		wordp++;
		p->makenode(";");
		while (word[wordp].name == "procedure") {
			Procedure(level, top, p);
		}	
	}
	else  {
		cout << "����������" << word[wordp].row << "\n�������ͣ�����˵������ȱ�ٷֺ�\n";
		system("pause");
		return;
	}

}
//******************************************
//����
void GrammaAnalysis::Factor(int level, int top, Node *p) {
	string temp;
	if (word[wordp].sym == IDENT) {
		temp = IDent(p->makenode("��ʶ��"));
		int i = symtable.search(temp, level, 0);
		if (i == -1) {
			cout << "����������" << word[wordp].row << "\n�������ͣ�δ�����ʶ��\n";
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
			cout << "����������" << word[wordp].row << "\n�������ͣ�����ֵ��ʶ��\n";
			system("pause");
			return;
		}
		


	}
	else if (word[wordp].sym == NUMBER) {
		temp = UnsignedNum(p->makenode("����"));
		codes.gen(0, 0, atoi(temp.c_str()));
		cx++;
	}
	else if (word[wordp].name == "(") {
		p->makenode("(");
		wordp++;
		Expression(level, top, p->makenode("���ʽ"));
		if (word[wordp].name == ")") {
			p->makenode(")");
			wordp++;
		}
		else {
			cout << "����������" << word[wordp].row << "\n�������ͣ�ȱ��������\n";
			system("pause");
			return;
		}
	}
	else {
		cout << "����������" << word[wordp].row << "\n�������ͣ�ȱ������\n";
		system("pause");
		return;
	}
}
//******************************************
//��
void GrammaAnalysis::Item(int level, int top, Node *p) {
	Factor(level, top, p->makenode("����"));
	while (word[wordp].name == "*" || word[wordp].name == "/") {
		string temp = word[wordp].name;
		p->makenode(word[wordp].name);
		wordp++;
		Factor(level, top, p->makenode("����"));
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
//���ʽ
void GrammaAnalysis::Expression(int level, int top, Node *p) {
	string temp = "";
	if (word[wordp].name == "+" || word[wordp].name == "-") {
		temp = word[wordp].name;
		p->makenode(word[wordp].name);
		wordp++;
	}

	Item(level, top, p->makenode("��"));
	if (temp == "-") {
		codes.gen(7, 0, 1);//ȡ��
		cx++;
	}
		

	
	while (word[wordp].name == "+" || word[wordp].name == "-") {
		temp = word[wordp].name;
		p->makenode(word[wordp].name);
		wordp++;
		Item(level, top, p->makenode("��"));
		if (temp == "+") {
			codes.gen(7, 0, 2);//��
			cx++;
		}
		else if (temp == "-") {
			codes.gen(7, 0, 3);//��
			cx++;
		}
			
	}
}
//******************************************
//��ֵ���
void GrammaAnalysis::AssignmentStatement(int level, int top, Node *p) {
	string temp = IDent(p->makenode("��ʶ��"));
	int i = symtable.search(temp, level, 0);
	if (i == -1) {
		cout << "����������" << word[wordp].row << "��ʶ��\"" << temp << "\"δ�ҵ�����\n";
		system("pause");
		return;
	}
	if (symtable.table[i].kind == "const") {
		cout << "����������" << word[wordp].row << "����\"" << temp << "\"������Ϊ����ʹ��\n";
		system("pause");
		return;
	}
	
	if (temp == "") {
		cout << "��ֵ���ĸ�ֵ����Ǳ�ʶ��\n";
		system("pause");
		return;
	}
	else {
		if (word[wordp].name == ":=") {
			p->makenode(":=");
			wordp++;
			
			Expression(level, top, p->makenode("���ʽ"));

			codes.gen(2,level-symtable.table[i].level, symtable.table[i].addr);
			cx++;
		}
		else {
			cout << "����������" << word[wordp].row << "\n�������ͣ���ֵ���ȱ��:=\n";
			system("pause");
			return;
		}
	}
}
//******************************************
//����
void GrammaAnalysis::Condition(int level, int top, Node *p) {
	if (word[wordp].sym == IDENT) {
		Expression(level, top, p->makenode("���ʽ"));
		string temp = Roperator(p->makenode("�Ƚ������"));
		Expression(level, top, p->makenode("���ʽ"));
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
		Expression(level, top, p->makenode("���ʽ"));
		codes.gen(7, 0, 6);
		cx++;
	}
	else {
		cout << "����������" << word[wordp].row << "\n�������ͣ��������ʽ����\n";
		system("pause");
		return;
	}
}
//******************************************
//�������
void GrammaAnalysis::ConditionalStatement(int level, int top, Node *p) {
	if (word[wordp].name == "if") {
		
		p->makenode(word[wordp].name);
		wordp++;
		
		Condition(level, top, p->makenode("����"));
		int cx0 = cx;//�������ָ��ĵ�ַ
		codes.gen(6, 0, 0);//JPC����ת��ַδ��
		cx++;
		if (word[wordp].name == "then") {
			p->makenode(word[wordp].name);
			wordp++;
			Statement(level, top, p->makenode("���"));
			codes.code[cx0].a = cx;//�����ַ
		}
		else {
			cout << "�������ȱ��then\n";
			system("pause");
			return;
		}
	}
	else {
		cout << "����������" << word[wordp].row << "\n�������ͣ��������ȱ��if\n";
		system("pause");
		return;
	}
}
//******************************************
//ѭ�����
void GrammaAnalysis::LoopStatement(int level, int top, Node *p) {
	if (word[wordp].name == "while") {
		p->makenode(word[wordp].name);
		wordp++;
		int cx0 = cx;
		Condition(level, top, p->makenode("����"));
		int cx1 = cx;//�������ָ��ĵ�ַ
		codes.gen(6, 0, 0);//JPC����ת��ַδ��
		cx++;
		if (word[wordp].name == "do") {
			p->makenode(word[wordp].name);
			wordp++;
			Statement(level, top, p->makenode("���"));
			codes.gen(5, 0, cx0);//��������ת��ȥ
			cx++;
			codes.code[cx1].a = cx;//����ָ��
		}
		else {
			cout << "����������" << word[wordp].row << "\n�������ͣ�ѭ�����ȱ��do\n";
			system("pause");
			return;
		}
	}
	else {
		cout << "����������" << word[wordp].row << "\n�������ͣ��������ȱ��while\n";
		system("pause");
		return;
	}
}
//******************************************
//���̵������
void GrammaAnalysis::ProcedureCallStatement(int level, int top, Node *p) {
	if (word[wordp].name == "call") {
		p->makenode(word[wordp].name);
		wordp++;
		//td = word[wordp];
		string temp = IDent(p->makenode("��ʶ��"));
		int i = symtable.search(temp, level, 1);
		if (i < 0) {
			cout << "����������" << word[wordp-1].row
				<< "\n�������ͣ�" << word[wordp-1].name << "��δ������߷Ƿ����õĹ��̱�ʶ��\n";
			system("pause");
			return;
		}
		if (symtable.table[i].kind != "procedure") {
			cout << "����������" << word[wordp-1].row
				<< "\n�������ͣ�" << word[wordp-1].row << "�ǹ��̱�ʶ��\n";
			system("pause");
			return;
		}
		codes.gen(3, level - symtable.table[i].level, symtable.table[i].addr);
		cx++;
	}
	else {
		cout << "����������" << word[wordp].row << "\n�������ͣ����̵������ȱ��call\n";
		system("pause");
		return;
	}
}
//******************************************
//�����
void GrammaAnalysis::ReadStatement(int level, int top, Node *p) {
	if (word[wordp].name == "read") {
		p->makenode(word[wordp].name);
		wordp++;
		if (word[wordp].name == "(") {
			p->makenode(word[wordp].name);
			wordp++;
			string temp = IDent(p->makenode("��ʶ��"));
			int i = symtable.search(temp, level, 0);
			if (i < 0) {
				cout << "����������" << word[wordp].row << "\n�������ͣ�δ�������\n";
				system("pause");
				return;
			}
			if (symtable.table[i].kind != "varible") {
				cout << "����������" << word[wordp].row << "\n�������ͣ��Ǳ���\n";
				system("pause");
				return;
			}
			codes.gen(7, 0, 16);
			codes.gen(2, level - symtable.table[i].level, symtable.table[i].addr);
			cx += 2;


			while (word[wordp].name == ",") {
				p->makenode(word[wordp].name);
				wordp++;
				temp = IDent(p->makenode("��ʶ��"));
				i = symtable.search(temp, level, 0);
				if (i < 0) {
					cout << "����������" << word[wordp].row << "\n�������ͣ�δ�������\n";
					system("pause");
					return;
				}
				if (symtable.table[i].kind != "varible") {
					cout << "����������" << word[wordp].row << "\n�������ͣ��Ǳ���\n";
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
				cout << "����������" << word[wordp].row << "\n�������ͣ������ȱ��������\n";
				system("pause");
				return;
			}
		}
		else {
			cout << "����������" << word[wordp].row << "\n�������ͣ������ȱ��������\n";
			system("pause");
			return;
		}
	}
	else {
		cout << "����������" << word[wordp].row << "\n�������ͣ������ȱ�ٹؼ���read\n";
		system("pause");
		return;
	}
}
//******************************************
//д���
void GrammaAnalysis::WriteStatement(int level, int top, Node *p) {
	if (word[wordp].name == "write") {
		p->makenode(word[wordp].name);
		wordp++;
		if (word[wordp].name == "(") {
			p->makenode(word[wordp].name);
			wordp++;
			Expression(level, top, p->makenode("���ʽ"));
			codes.gen(7, 0, 14);
			codes.gen(7, 0, 15);
			cx += 2;
			while (word[wordp].name == ",") {
				p->makenode(word[wordp].name);
				wordp++;
				Expression(level, top, p->makenode("���ʽ"));
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
				cout << "����������" << word[wordp].row << "\n�������ͣ�д���ȱ��������\n";
				system("pause");
				return;
			}
		}
		else {
			cout << "����������" << word[wordp].row << "\n�������ͣ�д���ȱ��������\n";
			system("pause");
			return;
		}
	}
	else {
		cout << "����������" << word[wordp].row << "\n�������ͣ�д���ȱ�ٹؼ���write\n";
		system("pause");
		return;
	}
}
//******************************************
//�������
void GrammaAnalysis::CompoundStatement(int level, int top, Node *p) {
	if (word[wordp].name == "begin") {
		p->makenode(word[wordp].name);
		wordp++;
		Statement(level, top, p->makenode("���"));
		while (word[wordp].name == ";") {
			p->makenode(word[wordp].name);
			wordp++;
			Statement(level, top, p->makenode("���"));
		}
		if (word[wordp].name == "end") {
			p->makenode(word[wordp].name);
			wordp++;
			return;
		}
		else {
			cout << "����������" << word[wordp].row << "\n�������ͣ��������ȱ�ٹؼ���end\n";
			system("pause");
			return;
		}
	}
	else {
		cout << "����������" << word[wordp].row << "\n�������ͣ��������ȱ�ٹؼ���begin\n";
		system("pause");
		return;
	}
}

//******************************************
//���
void GrammaAnalysis::Statement(int level, int top, Node *p) {

	if (word[wordp].sym == IDENT) {
		AssignmentStatement(level, top, p->makenode("��ֵ���"));
	}
	else if (word[wordp].name == "if") {
		ConditionalStatement(level, top, p->makenode("�������"));
	}
	else if (word[wordp].name == "while") {
		LoopStatement(level, top, p->makenode("ѭ�����"));
	}
	else if (word[wordp].name == "call") {
		ProcedureCallStatement(level, top, p->makenode("���̵������"));
	}
	else if (word[wordp].name == "read") {
		ReadStatement(level, top, p->makenode("�����"));
	}
	else if (word[wordp].name == "write") {
		WriteStatement(level, top, p->makenode("д���"));
	}
	else if (word[wordp].name == "begin") {
		CompoundStatement(level, top, p->makenode("�������"));
	}
	if (p->count == 0) {
		p->makenode("��");
	}
}


//******************************************
//�ֳ���
void GrammaAnalysis::SubProgram(int level, int top, Node *p) {

	
	int cx0 = cx;//���浱ǰ�ֳ�����ָ������ʼλ��
	int tx0 = tx - 1;//���浱ǰ�ֳ����ڷ��ű��е���ʼλ��
	dx = 3;//�ֳ��������λ���ó�ֵΪ3
	level++;//��μ�һ
	
	codes.gen(5, 0, 0);//����һ����ת��䣬��ת��ַ��ʱΪ0
	cx++;//ָ��������1

	if(word[wordp].name == "const")
		Const(level, top, p->makenode("����˵������"));//����˵������
	if(word[wordp].name == "var")
		Varible(level, top, p->makenode("����˵������"));//����˵������

	int dx0 = dx;//���浱ǰ�ֳ����У�����������˵��֮������λ�ã���Ϊ����˵�����ݹ���õ�ǰ������dx��ȫ�ֱ���������dx�ı䣬��������ʱ���棩
	if(word[wordp].name=="procedure")
		Procedure(level, top, p->makenode("����˵������"));//����˵������
	dx = dx0;
	if (tx0 >= 0 && symtable.table[tx0].kind == "procedure") {
		symtable.table[tx0].addr = cx;//������ű���procedure�е�addr
	}

	codes.code[cx0].a = cx;//����ָ����и÷ֳ������ת���ĵ�ַ
	codes.gen(4, 0, dx);//����int���
	cx++;//ָ��������һ

	if (word[wordp].sym == IDENT || word[wordp].name == "begin" || word[wordp].name == "if" ||  word[wordp].name =="while"
		|| word[wordp].name == "read" || word[wordp].name == "write" || word[wordp].name == "call") {
		Statement(level, top, p->makenode("���"));//���
	}
	codes.gen(7, 0, 0);
	cx++;
	

}
//******************************************
//����
void GrammaAnalysis::Program(Node*p) {
	int level = -1;//��γ�ʼ��Ϊ-1
	int top = 0;
	SubProgram(level, top, p->makenode("�ֳ���"));//�ֳ���
	if (word[wordp].name == ".") {
		p->makenode(".");
		wordp++;
		cout << "�﷨�����ɹ�\n";
	}
	else {
		cout << "����������" << word[wordp].row << "\n�������ͣ�����ȱ��\".\"\n";
	}
}
void GrammaAnalysis::gramanalysis() {
	//�򿪴ʷ������ļ����ж�ȡ�����ʷ�
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
	//		cout << "���ʹ���\n";
	//		return;
	//	}
	//}
	//openfile.close();
	//for (int i = 0; i < wordnum; i++) {
	//	cout << word[i] << endl;
	//}
	string tempfilename = filename.substr(0, filename.length() - 4) + "_code.txt";
	tree = new Tree(filename);//�����﷨��
	
	Program(tree->getroot());//���϶��µ��﷨����
	tree->showtree();//��ӡ�﷨��
	
	symtable.print(tempfilename);
	codes.print(tempfilename);
}
