#include <iostream>
#include "KNVM.h"

using namespace std;
using namespace KNVM;

int main() {
	Memory code(0x1000, PAGE_READWRITE, 4);
	code += Asm(OP_PUSH, "ebp");
	code += Asm(OP_MOV, "ebp", "esp");
	code += Asm(OP_SUB, "esp", 0x100);
	code += Asm(OP_ADD, "esp", 0x100);
	code += Asm(OP_MOV, "ebp", "esp");
	code += Asm(OP_EXIT);
	code += Asm(OP_RET);

	Disassembler disassembler(code);

	cout << disassembler.disassemble() << endl;

	::KNVM::KNVM knvm;

	knvm.test2(code);

	int i;
	cin >> i;

	return 0;
}