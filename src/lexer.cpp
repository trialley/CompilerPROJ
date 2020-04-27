#include <exception>
#include <stdexcept>

#include "lexer.h"

lexer::lexer(const std::string& filename) {
	initMap();

	index_pointer = 0;
	openSrc(filename);
}

void lexer::initMap() {
	symMap[CONST] = "CONST";
	symMap[VAR] = "VAR ";
	symMap[PROD] = "PROD";

	opMap[LIT] = "LIT";
	opMap[LOD] = "LOD";
	opMap[STO] = "STO";
	opMap[CAL] = "CAL";
	opMap[INT] = "INT";
	opMap[JMP] = "JMP";
	opMap[JPC] = "JPC";
	opMap[OPR] = "OPR";
}

void lexer::openSrc(const std::string& src) {
	fp = fopen(src.c_str(), "rb");
	if (fp == NULL) {
		cout << "open file error!" << endl;
		return;
	}
	row = 1;
}

bool lexer::readLine() {
	if ((fgets(buffer, BUFFERLEN, fp)) != NULL)
		return 1;

	else
		return 0;
}

void lexer::GetChar() {
	ch = buffer[index_pointer++];

	if (ch == '\n') {  //����

		row++;

		if (readLine()) {
			index_pointer = 0;
			ch = buffer[index_pointer++];  //ȡ���еĵ�һ���ַ�
		} else {
			throw std::exception(std::logic_error("�ļ�������β��"));
		}
	}
}

void lexer::GetBC() {
	while ((ch == ' ') || (ch == '\t') || (ch == '\r')) {
		try {
			GetChar();
		}

		catch (exception e) {
			throw e;
		}
	}
}

void lexer::Retract() {
	if (index_pointer > 0) index_pointer--;
	ch = ' ';
}

bool lexer::IsLetter(const char c) {
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
		return true;

	else
		return false;
}

bool lexer::IsDigit(const char c) {
	if (c >= '0' && c <= '9')
		return true;

	else
		return false;
}

int lexer::Reserve(const char* strToken) {	//�ڹؼ��ֱ��в�ѯ�����ر����ֵı���

	for (int i = 0; i < RESERVE_LEN; i++) {
		if (!strcmp(rsv_[i], strToken)) {
			return i + 1;
		}
	}

	return 0;  //û�ҵ�������
}

tuple3 lexer::GETSYM() {
	int code, value;
	tuple3 t;
	char strToken[MAX_ID_LEN];
	strToken[0] = '\0';

	try {
		GetChar();
	} catch (exception e) {
		t.sym = INVALID;
		return t;
	}

	try {
		GetBC();
	}

	catch (exception e) {
		t.sym = INVALID;
		return t;
	}

	if (IsLetter(ch)) {	 //start with letter,which means it is a identifier

		while ((IsLetter(ch) || IsDigit(ch))) {
			char s[2];
			s[0] = ch, s[1] = '\0';
			strcat(strToken, s);  //�޸���strToken

			try {
				GetChar();
			} catch (exception e) {
				t.sym = INVALID;
				return t;
			}
		}

		Retract();

		code = Reserve(strToken);  //���عؼ��ֵı���

		if (!code) {  //���ڹؼ��ֱ��У��Ǳ�ʶ��

			t.sym = $IDENT;
			strcpy(t.name, strToken);

			cout << "��ʶ��"
				 << "\t";
			cout << t.name << endl;
			return t;

		}

		else  //�ǹؼ���
		{
			t.sym = code;

			cout << "�ؼ���"
				 << "\t";
			cout << rsv_[code - 1] << endl;
			return t;
		}

	}

	else if (IsDigit(ch)) {	 //start with digit,which means it is a constant

		while (IsDigit(ch)) {
			char s[2];
			s[0] = ch, s[1] = '\0';
			strcat(strToken, s);

			try {
				GetChar();
			} catch (exception e) {
				t.sym = INVALID;
				return t;
			}
		}

		Retract();

		t.sym = $NUMBER;
		strcpy(t.name, strToken);

		cout << "����"
			 << "\t";
		cout << t.name << endl;
		return t;

	}

	else if (ch == '>') {
		try {
			GetChar();
		} catch (exception e) {
			t.sym = INVALID;
			return t;
		}
		if (ch == '=') {
			t.sym = $BIGGER_EQ;

			cout << ">=" << endl;
			return t;

		} else {
			Retract();
			t.sym = $BIGGER;

			cout << ">" << endl;
			return t;
		}

	}

	else if (ch == '<') {
		try {
			GetChar();
		} catch (exception e) {
			t.sym = INVALID;
			return t;
		}

		if (ch == '=') {
			t.sym = $LOWER_EQ;

			cout << "<=" << endl;
			return t;

		} else {
			Retract();
			t.sym = $LOWER;

			cout << "<" << endl;
			return t;
		}

	} else if (ch == '=') {
		t.sym = $EQ;
		cout << "=" << endl;
		return t;
	}

	else if (ch == '#') {
		t.sym = $UEQ;
		cout << "#" << endl;
		return t;
	}

	else if (ch == ':') {
		try {
			GetChar();
		}

		catch (exception e) {
			t.sym = INVALID;
			return t;
		}

		if (ch == '=') {
			t.sym = $ASSIGN;
			cout << ":=" << endl;
			return t;
		}

		else {
			Retract();
			/*:���治��=*/
		}
	}

	else if (ch == '+') {
		t.sym = $PLUS;

		cout << "+" << endl;
		return t;
	} else if (ch == '-') {
		t.sym = $MINUS;
		cout << "-" << endl;
		return t;
	} else if (ch == '*') {
		try {
			GetChar();
		} catch (exception e) {
			t.sym = INVALID;
			return t;
		}

		if (ch == '*') {
			t.sym = $POWER;

			cout << "**" << endl;
			return t;
		} else {
			Retract();
			t.sym = $STAR;

			cout << "*" << endl;
			return t;
		}

	}

	else if (ch == '/') {
		t.sym = $DIV;
		cout << "/" << endl;
		return t;
	}

	else if (ch == ',') {
		t.sym = $COMMA;

		cout << "," << endl;
		return t;
	} else if (ch == ';') {
		t.sym = $SEMICOLON;

		cout << ";" << endl;
		return t;
	} else if (ch == '(') {
		t.sym = $LPAIR;

		cout << "(" << endl;
		return t;
	} else if (ch == ')') {
		t.sym = $RPAIR;

		cout << ")" << endl;
		return t;
	} else if (ch == '{') {
		t.sym = $LBRACE;

		cout << "{" << endl;
		return t;
	} else if (ch == '}') {
		t.sym = $RBRACE;

		cout << "}" << endl;
		return t;
	} else {
		printf("Illegal input!\n");

		t.sym = INVALID;
		return t;
	}
}
