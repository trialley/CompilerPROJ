#include"execute.h"

int main () {
	//cout << "请输入源文件名：";
	//string filename;
	//cin >> filename;
	string filename = "test.txt";
	LexicalAnalysis lex (filename);
	if (!lex.lexanalysis ()) {
		cout << "词法分析失败\n";
	} else {
		cout << "词法分析成功\n";
	}
	//GrammaAnalysis gra(filename);//进行语法分析并建立语法树
	//gra.gramanalysis();

	//Execute exe;
	//exe.execute();

	system ("pause");
	return 0;
}