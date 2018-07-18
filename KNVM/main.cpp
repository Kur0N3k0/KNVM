#include <iostream>
#include "KNVM.h"

using namespace std;
using namespace KNVM;

int main() {
	Memory code(0x1000, PAGE_READWRITE, 4);
	Memory data(0x1000, PAGE_READWRITE, 4);

	char buffer[1024] = { 0, };

	code += Asm(OP_MOV, "eax", 0x41414141);
	code += Asm(OP_PUSH, "eax");
	code += Asm(OP_POP, "ebx");

	code += Asm(OP_MOV, "eax", INT_EXIT);
	code += Asm(OP_MOV, "ebx", 0);
	code += Asm(OP_SYSCALL);
	cout << "codesize : " << code.getCodeSize() << endl;

	/*
	data += "Input>> ";
	data += "\x0a\x34\x33\x2e\x0f\x24\x2a\x2e"; //0xa34332e0f242a2e
	data += "Woah....";
	data += "Nonono!!";

	Function fn1 = "fn";
	Function fn1cpy = fn1;

	fn1 += Asm(OP_MOV, "eax", INT_WRITE);
	fn1 += Asm(OP_MOV, "ebx", 1);
	fn1 += Asm(OP_MOV, "ecx", OFFSET_DATA + 9 * 3);
	fn1 += Asm(OP_MOV, "edx", 100);
	fn1 += Asm(OP_SYSCALL);
	fn1 += Asm(OP_MOV, "eax", INT_EXIT);
	fn1 += Asm(OP_MOV, "ebx", 0);
	fn1 += Asm(OP_SYSCALL);

	Function fn1cpy2 = fn1;
	fn1 += Asm(OP_MOV, "eax", INT_WRITE);
	fn1 += Asm(OP_MOV, "ebx", 1); // stdout
	fn1 += Asm(OP_MOV, "ecx", OFFSET_DATA);
	fn1 += Asm(OP_MOV, "edx", 9);
	fn1 += Asm(OP_SYSCALL);

	fn1 += Asm(OP_MOV, "eax", INT_READ);
	fn1 += Asm(OP_MOV, "ebx", 0); // stdin
	fn1 += Asm(OP_MOV, "ecx", OFFSET_DATA + 0x100);
	fn1 += Asm(OP_MOV, "edx", 9);
	fn1 += Asm(OP_SYSCALL);

	fn1 += Asm(OP_MOV, "ebx", OFFSET_DATA + 9);
	fn1 += Asm(OP_MOV, "ecx", OFFSET_DATA + 0x100);
	fn1 += Asm(OP_XOR, "[ecx]", 0x41414141);
	fn1 += Asm(OP_XOR, "ebx", "ecx");
	fn1 += Asm(OP_MOV, "ecx", OFFSET_DATA + 9 + 4, true);
	fn1 += Asm(OP_MOV, "edx", OFFSET_DATA + 0x100 + 4, true);
	fn1 += Asm(OP_XOR, "edx", 0x41414141);
	fn1 += Asm(OP_XOR, "ecx", "edx");
	fn1 += Asm(OP_ADD, "ebx", "ecx");
	fn1 += Asm(OP_CMP, "ebx", 0);
	fn1 += Asm(OP_JNE, label(fn1, fn1cpy));

	fn1 += Asm(OP_MOV, "eax", INT_WRITE);
	fn1 += Asm(OP_MOV, "ebx", 1);
	fn1 += Asm(OP_MOV, "ecx", (DWORD)data.get() + 9 * 2);
	fn1 += Asm(OP_MOV, "edx", 100);
	fn1 += Asm(OP_SYSCALL);

	code += fn1;
	code += Asm(OP_ADD_EXCEPT, (DWORD)fn1.getBase() + fn1cpy2.getSize());
	code += Asm(OP_EXCEPT_CALL, (DWORD)fn1.getBase() + fn1cpy2.getSize());
	*/

	//Disassembler disassembler(code);
	//cout << disassembler.disassemble() << endl;

	::KNVM::KNVM knvm;
	KNF knf;
	knf.bits = KNF::KNF_X86;
	knf.codeoffset = sizeof(KNF);
	knf.codesize = code.getCodeSize();
	knf.dataoffset = (DWORD)knf.codeoffset + knf.codesize;
	knf.datasize = 0;
	knf.entrypoint = (void *)0; // va

	KNFBuilder builder(knf, code, data);
	builder.build("sample.knf");

	//knvm.ParseBinary(knf, code, data);
	knvm.ParseBinary("sample.knf");
	knvm.Emulate();

	UI ui;

	int i;
	cin >> i;

	return 0;
}