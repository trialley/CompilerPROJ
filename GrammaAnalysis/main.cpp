
#include"lex.h"

int main () {
	string filename = "test.txt";
	LexicalAnalysis lex;
	lex.setFileName (filename);

	if (!lex.lexanalysis ()) {
		cout << "词法分析失败\n";
	} else {
		cout << "词法分析成功\n";
	}

	//lex.

	return 0;
}