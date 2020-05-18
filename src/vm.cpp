#include "vm.h"

#define STACK_SIZE 100

vm::vm(const std::string& filename) {
	readPl0(filename.c_str());	//��ȡpl0�ļ�

	ip = 0;
	sp = -1;
	bp = 0;

	lev = 0;
	SL.push_back(0);  //������ľ�̬����0

	stack = new int[STACK_SIZE];
	for (int i = 0; i < STACK_SIZE; i++) {
		stack[i] = 0;
	}

	run();
}

void vm::readPl0(const char* filename) {
	FILE* fp = fopen(filename, "rb");  //��.pl0�ļ�
	int i = 0;

	if (!fp) {
		std::cout << "��.pl0�ļ�ʧ�ܣ�" << std::endl;
		exit(-1);
	}

	while (!feof(fp)) {
		codeSeg.push_back(CODE());	//��pushһ���յ�
		fread(&codeSeg[i++], sizeof(CODE), 1, fp);
	}
}

void vm::run() {
	while (ip < codeSeg.size()) {
		runInst();
	}

	std::cout << "���뷭�벢ִ�����!" << std::endl;
}

void vm::runInst() {
	inst = codeSeg.at(ip++);  //ȡָ
	int temp;

	switch (inst.fun) {
		case LIT:  //������ջ��
			stack[++sp] = inst.offset;
			break;

		case LOD:  //������ջ��
		{
			int tempBp = stack[bp + 2];
			int levOffset = inst.lev;

			while (levOffset-- != 0) {	//���ž�̬���������
				tempBp = stack[tempBp + 2];
			}

			temp = stack[tempBp + inst.offset];
			stack[++sp] = temp;
			break;
		}
			//ջ�����ݴ浽������
		case STO: {
			int tempBp = stack[bp + 2];
			int levOffset = inst.lev;

			while (levOffset-- != 0) {
				tempBp = stack[tempBp + 2];
			}

			temp = stack[sp];
			stack[tempBp + inst.offset] = temp;

			break;
		}  //���ž�̬���������

		case CAL:
			stack[sp + 1] = bp;					 //push bp.��bp������̬��
			stack[sp + 2] = ip;					 //���ص�ַ
			stack[sp + 3] = SL[lev - inst.lev];	 //��̬��

			lev = lev - inst.lev;

			SL.push_back(bp);  //���浱ǰ���е�bp.ÿcallһ�α���һ��
			bp = sp + 1;	   //��¼�����ù��̵Ļ���ַ
			ip = inst.offset;
			break;

		case INT:
			sp += inst.offset;	//ջ����a
			break;

		case JMP:
			ip = inst.offset;  //ipת��a
			break;

		case JPC:

			if (!stack[sp])		   //ջ������ֵΪ����
				ip = inst.offset;  //ת��a�ĵ�ַ
			break;

		case OPR:  //��ϵ����������
		{
			switch (inst.offset) {
				case OPR::ADD:
					temp = stack[sp - 1] + stack[sp];
					stack[--sp] = temp;
					break;

				case OPR::SUB:
					temp = stack[sp - 1] - stack[sp];
					stack[--sp] = temp;
					break;

				case OPR::DIV:
					temp = stack[sp - 1] / stack[sp];
					stack[--sp] = temp;
					break;

				case OPR::MINUS:
					stack[sp] = -stack[sp];
					break;

				case OPR::MUL:
					temp = stack[sp - 1] * stack[sp];
					stack[--sp] = temp;
					break;

				case OPR::EQ:
					temp = (stack[sp - 1] - stack[sp] == 0);
					stack[--sp] = temp;
					break;

				case OPR::UE:
					temp = (stack[sp - 1] - stack[sp] != 0);
					stack[--sp] = temp;
					break;

				case OPR::GE:
					temp = (stack[sp - 1] - stack[sp] >= 0);
					stack[--sp] = temp;
					break;

				case OPR::GT:
					temp = (stack[sp - 1] - stack[sp] > 0);
					stack[--sp] = temp;
					break;

				case OPR::LE:
					temp = (stack[sp - 1] - stack[sp] <= 0);
					stack[--sp] = temp;
					break;

				case OPR::LT:
					temp = (stack[sp - 1] - stack[sp] < 0);
					stack[--sp] = temp;
					break;

				case OPR::ODD:	//�ж�ջ���������Ƿ�Ϊ����
					stack[sp] = (stack[sp] % 2 == 1);
					break;

				case OPR::WRITE:
					writeMem(sp--);	 //���ջ����������,����ջ��-1
					break;

				case OPR::READ:
					readMem();
					break;

				case 0:	 //�˳����������˳��ӳ���
					sp = bp - 1;
					ip = stack[bp + 1];	 //���ص�ַ
					bp = stack[bp];		 //��̬���ĵ�ַ

					SL.pop_back();	//ȥ��

					if (sp == -1) {
						printStack();
						exit(0);
					}  //���������
					break;

				default:
					break;
			}
			break;
		}

		default:
			/*���������*/
			break;
	}

	printStack();
}

void vm::readMem() {
	std::cout << "read:" << std::endl;
	int a;	//�Ӽ��̶�ȡһ������
	std::cin >> a;
	stack[++sp] = a;  //�ŵ�����ջ��ջ��
}

void vm::writeMem(int addr) {
	std::cout << "write:" << stack[addr];
	std::cout << std::endl;
}

void vm::printStack() {
	//cout << std::endl;
	//cout << "���Ĵ�������:" << std::endl;
	//cout << "SP:" << sp << "\t";
	//cout << "BP:" << bp << "\t";
	//cout << "IP:" << ip << std::endl;

	//cout << "IR:" << opMap[inst.fun] << "\t";
	//cout << inst.lev << "\t";
	//cout << inst.offset << std::endl;

	//cout << "ջ����:" << std::endl;

	//for (int i = sp; i >= 0; i--)
	//	std::cout << stack[i]<<endl;
}
