#include "codes.h"
char codeTB[8][5] = {
	"LIT","LOD","STO","CAL","INT","JMP","JPC","OPR"
};//�������
void Codes::gen(int f, int l, int a) {
	code[codenum].f = f;
	code[codenum].l = l;
	code[codenum].a = a;
	codenum++;
}


void Codes::print(string tp) {
	ofstream df;
	df.open(tp, ios::app);
	if (!df.is_open()) {
		cout << "�����ļ�δ�򿪳ɹ�\n";
		return;
	}
	cout << "ָ�\n";
	df << "ָ�\n";
	cout << left << setw(7) << "index" << left << setw(6) << "f" << left << setw(6) << "l" << left << setw(6) << "a" << "\n";
	df << left << setw(7) << "index" << left << setw(6) << "f" << left << setw(6) << "l" << left << setw(6) << "a" << "\n";
	for (int i = 0; i < codenum; i++) {
		cout << left << setw(6) << i << left << setw(6) << codeTB[code[i].f] << left << setw(6) << code[i].l << left << setw(6) << code[i].a << "\n";
		df << left << setw(6) << i << left << setw(6) << codeTB[code[i].f] << left << setw(6) << code[i].l << left << setw(6) << code[i].a << "\n";
	}
}