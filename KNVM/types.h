#pragma once

#include "Operand.h"
#include "Memory.h"
#include "FuncException.h"
#include "Asm.h"

#include <Windows.h>
#include <string.h>

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

	namespace x86Inst {
		enum OPTYPE {
			NOP,
			AL, AH, AX, EAX,
			BL, BH, BX, EBX,
			CL, CH, CX, ECX,
			DL, DH, DX, EDX,
			SI, ESI,
			DI, EDI,
			SP, ESP,
			BP, EBP,
			IP, EIP,
			ES,
			CS,
			SS,
			DS,
			FS,
			GS,
			rM8, rM16, rM32,
			rel8, rel16, rel32,
			r8, r16, r32,
			imm8, imm16, imm32
		};

		typedef struct ModRM {
			BYTE rm : 3;
			BYTE reg : 3;
			BYTE mod : 2;
		} ModRM;

		typedef struct Instruction {
			std::string mnemonic;
			BYTE prefix;
			BYTE opcode[3];
			ModRM modrm;
			BYTE sib;
			DWORD displacement;
			DWORD immediate;
			BYTE opsize;
		} Instruction;
	}

	typedef struct CodeMap {
		std::string mnemonic;
		BYTE op;
		DWORD op1;
		DWORD op2;
		DWORD size;
	} CodeMap;
#pragma pack(pop)

	static ThreadInfoBlock tib = { 0, };
}