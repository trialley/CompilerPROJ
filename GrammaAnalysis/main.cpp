
#include"lex.h"

int main () {
	string filename = "test.txt";
	LexicalAnalysis lex;
	lex.setFileName (filename);

	if (!lex.lexanalysis ()) {
		cout << "�ʷ�����ʧ��\n";
	} else {
		cout << "�ʷ������ɹ�\n";
	}

	//lex.

	return 0;
}