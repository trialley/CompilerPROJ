#include "vm.h"

#define STACK_SIZE 100

vm::vm(const char* filename) {
	readPl0(filename);	//读取pl0文件

	ip = 0;
	sp = -1;
	bp = 0;

	lev = 0;
	SL.push_back(0);  //主程序的静态链是0

	stack = new int[STACK_SIZE];
	for (int i = 0; i < STACK_SIZE; i++) {
		stack[i] = 0;
	}

	run();
}

void vm::readPl0(const char* filename) {
	FILE* fp = fopen(filename, "rb");  //打开.pl0文件
	int i = 0;

	if (!fp) {
		cout << "打开.pl0文件失败！" << endl;
		exit(-1);
	}

	while (!feof(fp)) {
		codeSeg.push_back(CODE());	//先push一个空的
		fread(&codeSeg[i++], sizeof(CODE), 1, fp);
	}
}

void vm::run() {
	while (ip < codeSeg.size()) {
		runInst();
	}

	cout << "代码翻译并执行完毕!" << endl;
}

void vm::runInst() {
	inst = codeSeg.at(ip++);  //取指
	int temp;

	switch (inst.fun) {
		case LIT:  //常量放栈顶
			stack[++sp] = inst.offset;
			break;

		case LOD:  //变量放栈顶
		{
			int tempBp = stack[bp + 2];
			int levOffset = inst.lev;

			while (levOffset-- != 0) {	//沿着静态链往外层找
				tempBp = stack[tempBp + 2];
			}

			temp = stack[tempBp + inst.offset];
			stack[++sp] = temp;
			break;
		}

		case STO:  //栈顶内容存到变量中
		{
			int tempBp = stack[bp + 2];
			int levOffset = inst.lev;

			while (levOffset-- != 0) {	//沿着静态链往外层找
				tempBp = stack[tempBp + 2];
			}

			temp = stack[sp];
			stack[tempBp + inst.offset] = temp;

			break;
		}

		case CAL:
			stack[sp + 1] = bp;					 //push bp.老bp，即动态链
			stack[sp + 2] = ip;					 //返回地址
			stack[sp + 3] = SL[lev - inst.lev];	 //静态链

			lev = lev - inst.lev;

			SL.push_back(bp);  //保存当前运行的bp.每call一次保存一次
			bp = sp + 1;	   //记录被调用过程的基地址
			ip = inst.offset;
			break;

		case INT:
			sp += inst.offset;	//栈顶加a
			break;

		case JMP:
			ip = inst.offset;  //ip转到a
			break;

		case JPC:

			if (!stack[sp])		   //栈顶布尔值为非真
				ip = inst.offset;  //转到a的地址
			break;

		case OPR:  //关系和算术运算
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

				case OPR::ODD:	//判断栈顶操作数是否为奇数
					stack[sp] = (stack[sp] % 2 == 1);
					break;

				case OPR::WRITE:
					writeMem(sp--);	 //输出栈顶的运算结果,并将栈顶-1
					break;

				case OPR::READ:
					readMem();
					break;

				case 0:	 //退出数据区，退出子程序
					sp = bp - 1;
					ip = stack[bp + 1];	 //返回地址
					bp = stack[bp];		 //动态链的地址

					SL.pop_back();	//去掉

					if (sp == -1) {
						printStack();
						exit(0);
					}  //主程序结束
					break;

				default:
					break;
			}
			break;
		}

		default:
			/*操作码错误*/
			break;
	}

	printStack();
}

void vm::readMem() {
	cout << "请输入read函数的参数:" << endl;
	int a;	//从键盘读取一个数字
	cin >> a;
	stack[++sp] = a;  //放到运行栈的栈顶
}

void vm::writeMem(int addr) {
	cout << "输出写语句写的内容是:" << stack[addr];
	cout << endl;
}

void vm::printStack() {
	//cout << endl;
	//cout << "各寄存器内容:" << endl;
	//cout << "SP:" << sp << "\t";
	//cout << "BP:" << bp << "\t";
	//cout << "IP:" << ip << endl;

	//cout << "IR:" << opMap[inst.fun] << "\t";
	//cout << inst.lev << "\t";
	//cout << inst.offset << endl;

	//cout << "栈内容:" << endl;

	//for (int i = sp; i >= 0; i--)
	//	cout << stack[i]<<endl;
}
