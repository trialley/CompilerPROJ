#include "vm.h"
#include <parser.h>

#include <exception>
#include <iomanip>
#include <iostream>
#define STACK_SIZE 10000

vm::vm(const std::string& filename) {
	readPl0(filename.c_str());	//读取pl0文件

	ip = 0;
	sp = -1;
	bp = 0;

	lev = 0;
	SL.push_back(0);  //主程序的静态链是0


	stack.resize(STACK_SIZE);
	std::fill(stack.begin(),stack.end(),0);


	run();
}

void vm::readPl0(const std::string& filename) {
	FILE* fp = fopen(filename.c_str(), "rb");  //打开.pl0文件
	int i = 0;

	if (!fp) {
		std::cout << "打开.pl0文件失败！" << std::endl;
		exit(-1);
	}

	while (!feof(fp)) {
		codeSeg.push_back(CODE());	//先push一个空的
		fread(&codeSeg[i++], sizeof(CODE), 1, fp);
	}
}

//不断执行指令
void vm::run() {
	int n = 0;
	while (ip < codeSeg.size()) {
		runInst(n);
		++n;
		if (n == 80) {
		std::cout << n << "times\n";
		}
	}
	std::cout << "代码翻译并执行完毕!" << std::endl;
}
//执行一条指令
void vm::runInst(int i) {
	auto printonecode=[&](CODE& inst){
			std::cout << std::setw(7) << "num" << std::setw(7) << "op" << std::setw(7) << "level" << std::setw(7) << "offset------------" << std::endl;
	// for (int i = 0; i < codeTable.size(); i++) {
		std::cout << std::setw(7) << i << "\t";
		std::cout << std::setw(7) << TisToString[inst.fun] << "\t";
		std::cout << std::setw(7) << inst.lev << "\t";
		std::cout << std::setw(7) << inst.offset << std::endl;
	// }
	};
	inst = codeSeg.at(ip++);  //取指
	if(ip==18){
		LOG<<"JNC!!!"<<std::endl;
	}
	// printonecode(inst);
	int temp;

	switch (inst.fun) {
	case LIT:  //常量放栈顶
		stack[++sp] = inst.offset;//
		break;

	case LOD: {	 //变量放栈顶
		int tempBp = stack[bp + 2];
		int levOffset = inst.lev;

		while (levOffset-- != 0) {	//沿着静态链往外层找
			tempBp = stack[tempBp + 2];
		}

		temp = stack[tempBp + inst.offset];
		stack[++sp] = temp;
		break;
	}
		
	case STO: {//栈顶内容存到变量中
		int tempBp = stack[bp + 2];
		int levOffset = inst.lev;

		while (levOffset-- != 0) {
			tempBp = stack[tempBp + 2];
		}

		temp = stack[sp--];
		stack[tempBp + inst.offset] = temp;

		break;
	}

	case CAL://过程调用								 //沿着静态链往外层找
	std::cout<<"bp:"<<bp<<" ip:"<<ip<<" sl:"<<SL[lev - inst.lev]<<std::endl;
		stack[sp + 1] = bp;					 //push bp.老bp，即动态链
		stack[sp + 2] = ip;					 //返回地址
		stack[sp + 3] = bp;//SL[lev - inst.lev];	 //静态链，也是老bp

		lev = inst.lev;

		SL.push_back(bp);  //保存当前运行的bp.每call一次保存一次
		bp = sp + 1;	   //记录被调用过程的基地址
		ip = inst.offset;
		break;

	case INT://预留数据空间
		sp += inst.offset;	//栈顶加a
		break;

	case JMP://无条件转移
		ip = inst.offset;  //ip转到a
		break;

	case JPC://条件转移

		if (!stack[sp--])		   //栈顶布尔值为非真
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
	std::cout << "read:" << std::endl;
	int a;	//从键盘读取一个数字
	std::cin >> a;
	stack[++sp] = a;  //放到运行栈的栈顶
}

void vm::writeMem(int addr) {
	std::cout << "write:" << stack[addr];
	std::cout << std::endl;
}

void vm::printStack() {
	std::cout << std::endl;
	std::cout << "SP:" << sp << "\t";
	std::cout << "BP:" << bp << "\t";
	std::cout << "IP:" << ip << std::endl;

	std::cout << "IR:" << TisToString[inst.fun] << "\t";
	std::cout << inst.lev << "\t";
	std::cout << inst.offset << std::endl;

	std::cout << "stack" << std::endl;

	for (int i = sp; i >= 0; i--)
		std::cout << i << " " << stack[i] << std::endl;
}
