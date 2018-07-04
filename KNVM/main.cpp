#include <iostream>
#include "KNVM.h"

using namespace std;
using namespace KNVM;

int main() {
	Memory code(0x1000, PAGE_READWRITE, 4);
	//code += Asm(OP_PUSH, 0x41414141);
	code += Asm(OP_PUSH, "ebp");
	code += Asm(OP_MOV, "ebp", "esp");
	code += Asm(OP_SUB, "esp", 0x100);
	
	code += Asm(OP_MOV, "eax", 0x41424344);
	code += Asm(OP_MOV, "ebx", 0x41424344);
	code += Asm(OP_CMP, "eax", "ebx");
	code += Asm(OP_JE, 0x00);

	code += Asm(OP_MOV, "ebp", "esp");
	code += Asm(OP_POP, "ebp");
	//code += Asm(OP_RET);
	code += Asm(OP_EXIT);

	Disassembler disassembler(code);

	cout << disassembler.disassemble() << endl;

	::KNVM::KNVM knvm;

	knvm.test2(code);

	int i;
	cin >> i;

	return 0;
}