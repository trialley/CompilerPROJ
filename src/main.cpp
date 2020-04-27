#include "Iterpreter.h"



int main (int argc, _TCHAR* argv[])
{
	char source[99];
	cout << "请输入你想要编译的源文件路径:" << endl;
	cin >> source;

	SyntaxAnal a (source);

	cout << endl;
	cout << "符号表中内容如下:" << endl;
	a.printTable ();

	cout << endl;
	cout << "代码表中内容如下:" << endl;

	a.printCode ();

	cout << "符号表(.sym)和目标代码(.pl0)都已输出在文件中." << endl;


	char target[99];
	cout << "请输入你想要执行的目标代码文件路径：" << endl;
	cin >> target;

	Iterpreter t (target);

	return 0;
}