#include <iostream>
#include "KNVM.h"

using namespace std;
using namespace KNVM;

int main() {
	Memory code(0x1000, PAGE_READWRITE, 4);
	//code += Asm(OP_PUSH, 0x41414141);
	/*
	code += Asm(OP_PUSH, "ebp");
	code += Asm(OP_MOV, "ebp", "esp");
	code += Asm(OP_SUB, "esp", 0x100);
	
	code += Asm(OP_MOV, "eax", 0x41424344);
	code += Asm(OP_MOV, "ebx", 0x41424344);
	code += Asm(OP_CMP, "eax", "ebx");
	code += Asm(OP_JE, 0x00);

	code += Asm(OP_MOV, "ebp", "esp");
	code += Asm(OP_POP, "ebp");
	*/
	//code += Asm(OP_RET);
	//code += Asm(OP_EXIT);

	char buffer[1024] = { 0, };

	Function fn1 = "fn";

	fn1 += Asm(OP_MOV, "eax", INT_READ);
	fn1 += Asm(OP_MOV, "ebx", 0); // stdin
	fn1 += Asm(OP_MOV, "ecx", (DWORD)buffer);
	fn1 += Asm(OP_MOV, "edx", 100);
	fn1 += Asm(OP_SYSCALL);

	fn1 += Asm(OP_MOV, "eax", INT_WRITE);
	fn1 += Asm(OP_MOV, "ebx", 1);
	fn1 += Asm(OP_MOV, "ecx", (DWORD)buffer);
	fn1 += Asm(OP_MOV, "edx", 100);
	fn1 += Asm(OP_SYSCALL);

	fn1 += Asm(OP_MOV, "eax", INT_EXIT);
	fn1 += Asm(OP_MOV, "ebx", 0);
	fn1 += Asm(OP_SYSCALL);

	code += fn1;
	code += Asm(OP_ADD_EXCEPT, (DWORD)fn1.getBase());
	code += Asm(OP_EXCEPT_CALL, (DWORD)fn1.getBase());

	//code += Asm(OP_MOV, "eax", INT_READ);
	//code += Asm(OP_MOV, "ebx", 0); // stdin
	//code += Asm(OP_MOV, "ecx", (DWORD)buffer);
	//code += Asm(OP_MOV, "edx", 100);
	//code += Asm(OP_SYSCALL);

	//code += Asm(OP_MOV, "eax", INT_WRITE);
	//code += Asm(OP_MOV, "ebx", 1);
	//code += Asm(OP_MOV, "ecx", (DWORD)buffer);
	//code += Asm(OP_MOV, "edx", 100);
	//code += Asm(OP_SYSCALL);

	//code += Asm(OP_MOV, "eax", INT_EXIT);
	//code += Asm(OP_MOV, "ebx", 0);
	//code += Asm(OP_SYSCALL);
	
	//Disassembler disassembler(code);

	//cout << disassembler.disassemble() << endl;
	//knvm.test2(code);

	Memory data(0x1000, PAGE_READWRITE, 4);
	data.setDataSize(0);

	::KNVM::KNVM knvm;
	KNF knf;
	knf.bits = KNF::KNF_X86;
	knf.codesize = code.getCodeSize();
	knf.datasize = 0;
	knf.entrypoint = (void *)fn1.getSize(); // va

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