#include "execute.h"
extern Codes codes;
extern char codeTB[8][5];
//char codeTB[8][5] = {
//	"LIT","LOD","STO","CAL","INT","JMP","JPC","OPR"
//};//�������
//Code ir;//ָ��Ĵ���
//int ip = 0;//�����ַ�Ĵ���
//int sp = 0;//ջ���Ĵ���
//int bp = 0;//����ַ�Ĵ���
//int sl = 1, dl = 2, ra = 0;
////  ��̬��  ��̬��  ���ص�ַ
////
////bp+2-->��̬��     dl
////bp+1-->��̬��     sl
////bp---->���ص�ַ   ra
//int stack[max_stack] = { 0 };

void Execute::execute () {
	while (ip < codes.codenum) {
		ir = codes.code[ip++];//ȡһ��ָ��
		switch (ir.f) {
		case 0://LIT���������ŵ�ջ����aΪ������ֵ
			stack[sp++] = ir.a;
			//print();
			break;
		case 1://LOD���������ŵ�ջ����a��Ϊ��������˵�����е����λ�ã�lΪ���ò���˵����Ĳ��ֵ	
			if (ir.l == 0) {//��ǰ��
				stack[sp++] = stack[bp + ir.a];
			} else {//��̬��ָ�����һ��
				int outbp = stack[bp + sl];
				while (--ir.l)
					outbp = stack[outbp + sl];
				stack[sp++] = stack[outbp + ir.a];
			}
			//print();
			break;
		case 2://STO:��ջ���������͵�ĳ������Ԫ�У�������lod��ͬ
			if (ir.l == 0) {//��ǰ��
				stack[bp + ir.a] = stack[sp - 1];
			} else {//��̬��ָ�����һ��
				int outbp = stack[bp + sl];
				while (--ir.l)
					outbp = stack[outbp + sl];
				stack[outbp + ir.a] = stack[sp - 1];
			}
			//print();
			break;
		case 3://CAL���ù��̵�ָ�aΪ�����ù��̵�Ŀ���������е�ַ��lΪ���
				//��ջ��+0��ջ��+1��ջ��+2�ֱ��ŷ��ص�ַ����̬����ַ����̬����ַ
			stack[sp + ra] = ip;//���ص�ַ��ջ
			stack[sp + dl] = bp;//��̬����ջ
			while (ir.l > 0) {
				bp = stack[bp + sl];
				ir.l = ir.l - 1;
			}
			stack[sp + sl] = bp;//��̬����ջ

			ip = ir.a;//��ת��ַ
			bp = sp;//�ı��ַ
			//print();
			break;
		case 4://INT:Ϊ�����õĹ��̣���������������ջ�п�����������a��Ϊ���ٵĸ���
			sp += ir.a; //sp��¼��procedure��������ջstack�������ݿռ�ĵ�ǰλ��
			//print();
			break;
		case 5://JMP
			ip = ir.a;
			//print();
			break;
		case 6://JPC
			if (stack[sp - 1] == 0) {//����0����ת
				ip = ir.a;
			}
			//print();
			break;
		case 7://OPR
			switch (ir.a) {
			case 0://���̵��ý��������ص��õ㲢��ջ
				ip = stack[bp + ra];//����������������һ�����̵��������ĵĵ�ַ
				sp = bp;
				bp = stack[bp + dl];//bp:ָ����øù���ǰ�������й��̵����ݶεĻ���ַ
				if (sp <= 0) {
					cout << "ִ�����!\n";
					return;
				}
				//print();
				break;
			case 1://ջ��Ԫ��ȡ��
				stack[sp - 2] = stack[sp - 1] - stack[sp - 2];
				sp--;
				//print();
				break;
			case 2://��
				stack[sp - 2] = stack[sp - 1] + stack[sp - 2];
				sp--;
				//print();
				break;
			case 3://����ջ��-ջ������
				stack[sp - 2] = stack[sp - 2] - stack[sp - 1];
				sp--;
				//print();
				break;
			case 4://��
				stack[sp - 2] = stack[sp - 1] * stack[sp - 2];
				sp--;
				//print();
				break;
			case 5://����
				stack[sp - 2] = stack[sp - 2] / stack[sp - 1];
				sp--;
				//print();
				break;
			case 6://��ż�ж�
				stack[sp - 1] = (stack[sp - 1] / 2 != 0) ? 1 : 0;
				//print();
				break;
			case 7://
				break;
			case 8://==
				stack[sp - 2] = (stack[sp - 2] == stack[sp - 1]) ? 1 : 0;
				sp--;
				//print();
				break;
			case 9://!=
				stack[sp - 2] = (stack[sp - 2] != stack[sp - 1]) ? 1 : 0;
				sp--;
				//print();
				break;
			case 10://����ջ��<ջ����<
				stack[sp - 2] = (stack[sp - 2] < stack[sp - 1]) ? 1 : 0;
				sp--;
				//print();
				break;
			case 11://����ջ��>=ջ����>=
				stack[sp - 2] = (stack[sp - 2] >= stack[sp - 1]) ? 1 : 0;
				sp--;
				//print();
				break;
			case 12://����ջ��>ջ����>
				stack[sp - 2] = (stack[sp - 2] > stack[sp - 1]) ? 1 : 0;
				sp--;
				//print();
				break;
			case 13://����ջ��<=ջ����<=
				stack[sp - 2] = (stack[sp - 2] <= stack[sp - 1]) ? 1 : 0;
				sp--;
				//print();
				break;
			case 14://ջ�����
				cout << "���Ϊ:" << stack[sp - 1] << "\n";
				//print();
				break;
			case 15://�������
				cout << "\n";
				//print();
				break;
			case 16://���뵽ջ��
				cout << "������һ������";
				int temp;
				cin >> temp;
				stack[sp++] = temp;
				//print();
				break;
			}
			break;
		}
	}





}
void Execute::print () {
	string t1 = sp - 1 + "--->" + stack[sp - 1];
	string t2 = sp - 2 + "--->" + stack[sp - 2];
	string t3 = sp - 3 + "--->" + stack[sp - 3];
	cout << "�Ĵ�����\nbp = " << bp << "   " << "sp = " << sp << "   " << "ip = " << ip << "   "
		<< "ir : " << codeTB[ir.f] << "   " << ir.l << "   " << ir.a << "\n" << "ջ���������ݣ�\n"
		<< ((sp >= 1) ? t1 : "--") << "\n"
		<< ((sp >= 2) ? t2 : "--")
		<< "\n" << ((sp >= 3) ? t3 : "--") << "\n";
}
