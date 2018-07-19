#pragma once

#include "Operand.h"
#include "Memory.h"
#include "FuncException.h"

#include <Windows.h>

namespace KNVM {
#define _Private
#define _Public

#pragma pack(push, 1)
	typedef struct DispatchInfo {
		BYTE opcode;
		BYTE opcode_type;
		BYTE *opcodes;
		Operand *operand[2];
		DWORD opcode_size;

		DispatchInfo() : opcodes(nullptr) { }
		~DispatchInfo() { if (opcodes != nullptr) delete opcodes; }
	} DispatchInfo;

	typedef struct DisassembleInfo {
		std::string asmbly;
		DWORD handlesize;
	} DisassembleInfo;

	typedef struct Context {
		DWORD eax;
		DWORD ebx;
		DWORD ecx;
		DWORD edx;
		DWORD esi;
		DWORD edi;
		DWORD esp;
		DWORD ebp;
		DWORD eip;
		DWORD eflags;
	} Context;

	typedef struct ThreadInfoBlock {
		DWORD tid;
		Memory *stack;
		Context context;
		ThreadInfoBlock *next;
	} ThreadInfoBlock;
#pragma pack(pop)

	static ThreadInfoBlock tib = { 0, };
}