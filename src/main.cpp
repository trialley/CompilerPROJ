#include "Iterpreter.h"



int main (int argc, _TCHAR* argv[])
{
	char source[99];
	cout << "����������Ҫ�����Դ�ļ�·��:" << endl;
	cin >> source;

	SyntaxAnal a (source);

	cout << endl;
	cout << "���ű�����������:" << endl;
	a.printTable ();

	cout << endl;
	cout << "���������������:" << endl;

	a.printCode ();

	cout << "���ű�(.sym)��Ŀ�����(.pl0)����������ļ���." << endl;


	char target[99];
	cout << "����������Ҫִ�е�Ŀ������ļ�·����" << endl;
	cin >> target;

	Iterpreter t (target);

	return 0;
}