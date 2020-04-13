#include "execute.h"
extern Codes codes;
extern char codeTB[8][5];
//char codeTB[8][5] = {
//	"LIT","LOD","STO","CAL","INT","JMP","JPC","OPR"
//};//操作码表
//Code ir;//指令寄存器
//int ip = 0;//程序地址寄存器
//int sp = 0;//栈顶寄存器
//int bp = 0;//基地址寄存器
//int sl = 1, dl = 2, ra = 0;
////  静态链  动态链  返回地址
////
////bp+2-->动态链     dl
////bp+1-->静态链     sl
////bp---->返回地址   ra
//int stack[max_stack] = { 0 };

void Execute::execute () {
	while (ip < codes.codenum) {
		ir = codes.code[ip++];//取一条指令
		switch (ir.f) {
		case 0://LIT：将常数放到栈顶，a为常数的值
			stack[sp++] = ir.a;
			//print();
			break;
		case 1://LOD：将变量放到栈顶，a域为变量在所说明层中的相对位置，l为调用层与说明层的层差值	
			if (ir.l == 0) {//当前层
				stack[sp++] = stack[bp + ir.a];
			} else {//静态链指向的上一层
				int outbp = stack[bp + sl];
				while (--ir.l)
					outbp = stack[outbp + sl];
				stack[sp++] = stack[outbp + ir.a];
			}
			//print();
			break;
		case 2://STO:将栈顶的内容送到某变量单元中，其他与lod相同
			if (ir.l == 0) {//当前层
				stack[bp + ir.a] = stack[sp - 1];
			} else {//静态链指向的上一层
				int outbp = stack[bp + sl];
				while (--ir.l)
					outbp = stack[outbp + sl];
				stack[outbp + ir.a] = stack[sp - 1];
			}
			//print();
			break;
		case 3://CAL调用过程的指令。a为被调用过程的目标程序的入中地址，l为层差
				//在栈顶+0，栈顶+1，栈顶+2分别存放返回地址，静态链地址，动态链地址
			stack[sp + ra] = ip;//返回地址入栈
			stack[sp + dl] = bp;//动态链入栈
			while (ir.l > 0) {
				bp = stack[bp + sl];
				ir.l = ir.l - 1;
			}
			stack[sp + sl] = bp;//静态链入栈

			ip = ir.a;//跳转地址
			bp = sp;//改变基址
			//print();
			break;
		case 4://INT:为被调用的过程（或主程序）在运行栈中开辟数据区。a域为开辟的个数
			sp += ir.a; //sp记录该procedure的在运行栈stack分配数据空间的当前位置
			//print();
			break;
		case 5://JMP
			ip = ir.a;
			//print();
			break;
		case 6://JPC
			if (stack[sp - 1] == 0) {//等于0则跳转
				ip = ir.a;
			}
			//print();
			break;
		case 7://OPR
			switch (ir.a) {
			case 0://过程调用结束，返回调用点并退栈
				ip = stack[bp + ra];//返回引用它的另外一个过程的那条语句的的地址
				sp = bp;
				bp = stack[bp + dl];//bp:指向调用该过程前正在运行过程的数据段的基地址
				if (sp <= 0) {
					cout << "执行完毕!\n";
					return;
				}
				//print();
				break;
			case 1://栈顶元素取反
				stack[sp - 2] = stack[sp - 1] - stack[sp - 2];
				sp--;
				//print();
				break;
			case 2://加
				stack[sp - 2] = stack[sp - 1] + stack[sp - 2];
				sp--;
				//print();
				break;
			case 3://（次栈顶-栈顶）减
				stack[sp - 2] = stack[sp - 2] - stack[sp - 1];
				sp--;
				//print();
				break;
			case 4://乘
				stack[sp - 2] = stack[sp - 1] * stack[sp - 2];
				sp--;
				//print();
				break;
			case 5://除以
				stack[sp - 2] = stack[sp - 2] / stack[sp - 1];
				sp--;
				//print();
				break;
			case 6://奇偶判断
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
			case 10://（次栈顶<栈顶）<
				stack[sp - 2] = (stack[sp - 2] < stack[sp - 1]) ? 1 : 0;
				sp--;
				//print();
				break;
			case 11://（次栈顶>=栈顶）>=
				stack[sp - 2] = (stack[sp - 2] >= stack[sp - 1]) ? 1 : 0;
				sp--;
				//print();
				break;
			case 12://（次栈顶>栈顶）>
				stack[sp - 2] = (stack[sp - 2] > stack[sp - 1]) ? 1 : 0;
				sp--;
				//print();
				break;
			case 13://（次栈顶<=栈顶）<=
				stack[sp - 2] = (stack[sp - 2] <= stack[sp - 1]) ? 1 : 0;
				sp--;
				//print();
				break;
			case 14://栈顶输出
				cout << "结果为:" << stack[sp - 1] << "\n";
				//print();
				break;
			case 15://输出换行
				cout << "\n";
				//print();
				break;
			case 16://读入到栈顶
				cout << "请输入一个数：";
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
	cout << "寄存器：\nbp = " << bp << "   " << "sp = " << sp << "   " << "ip = " << ip << "   "
		<< "ir : " << codeTB[ir.f] << "   " << ir.l << "   " << ir.a << "\n" << "栈顶部分数据：\n"
		<< ((sp >= 1) ? t1 : "--") << "\n"
		<< ((sp >= 2) ? t2 : "--")
		<< "\n" << ((sp >= 3) ? t3 : "--") << "\n";
}
