#include"execute.h"

int main () {
	//cout << "������Դ�ļ�����";
	//string filename;
	//cin >> filename;
	string filename = "test.txt";
	LexicalAnalysis lex (filename);
	if (!lex.lexanalysis ()) {
		cout << "�ʷ�����ʧ��\n";
	} else {
		cout << "�ʷ������ɹ�\n";
	}
	//GrammaAnalysis gra(filename);//�����﷨�����������﷨��
	//gra.gramanalysis();

	//Execute exe;
	//exe.execute();

	system ("pause");
	return 0;
}