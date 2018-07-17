#pragma once

#include "Memory.h"
#include "FuncException.h"

#include <Windows.h>

namespace KNVM {
#define _Private
#define _Public

	typedef struct DispatchInfo {
		BYTE opcode;
		BYTE opcode_type;
		BYTE *opcodes;
		DWORD opcode_size;

		DispatchInfo() : opcodes(nullptr) { }
		~DispatchInfo() { if (opcodes != nullptr) delete opcodes; }
	} DispatchInfo;

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

	static ThreadInfoBlock tib = { 0, };
}