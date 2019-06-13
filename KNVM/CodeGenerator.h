#pragma once

#include "types.h"
#include "Register.h"
#include "Memory.h"
#include "Asm.h"

#include <string>
#include <vector>
#include <fstream>

#include <Windows.h>

#include <json/json.h>

namespace KNVM {
	namespace x86Inst {
		//CodeMap codemap[] = {
		//	{ "ADD", '\x00', rM8, r8, 2 },
		//	{ "ADD", '\x01', rM32, }
		//};
		/*
		std::vector<std::string> mnemonic_map = {
			"ADD", "ADD", "ADD", "ADD", "ADD", "ADD", "PUSH ES", "POP ES",
			"ADC", "ADC", "ADC", "ADC", "ADC", "ADC", "PUSH SS", "POP SS",
			"AND", "AND", "AND", "AND", "AND", "AND", "", "DAA",
			"XOR", "XOR", "XOR", "XOR", "XOR", "XOR", "", "AAA",
			"INC EAX", "INC ECX", "INC EDX", "INC EBX", "INC ESP", "INC EBP", "INC ESI", "INC EDI",
			"PUSH EAX", "PUSH ECX", "PUSH EDX", "PUSH EBX", "PUSH ESP", "PUSH EBP", "PUSH ESI", "PUSH EDI",
			"PUSHAD", "POPAD", "BOUND", "ARPL", "", "", "", "",
			"JO", "JNO", "JB", "JNB", "JE", "JNE", "JNA", "JA"
			"", "", "", "", "TEST", "TEST", "XCHG", "XCHG",
			"NOP", "XCHG ECX, ", "XCHG EDX, ", "XCHG EBX, ", "XCHG ESP, ", "XCHG EBP, ", "XCHG ESI, ", "XCHG EDI, ",
			"MOV AL, ", "MOV EAX, ", "MOV", "MOV", "MOVSB", "MOVSD", "CMPSB", "CMPSD",
			"MOV AL, ", "MOV CL, ", "MOV DL, ", "MOV BL, ", "MOV AH, ", "MOV CH, ", "MOV DH, ", "MOV"
		};
		*/
	}

	class CodeGenerator {
	private:
		std::vector<Asm> vAsm;
		x86Inst::Instruction inst;

	private:
		DWORD getVMCodeSize();
		void getModRM(x86Inst::ModRM &modrm);

	private:
		void Prefix(BYTE *opcode, DWORD size);
		void ImmediateGrp(BYTE *opcode, DWORD size);
		void Grp(BYTE *opcode, DWORD size, BYTE id);
		void ShiftGrp(BYTE *opcode, DWORD size);
		void UnaryGrp(BYTE *opcode, DWORD size);
		void Default(BYTE *opcode, DWORD size);

	public:
		CodeGenerator();
		CodeGenerator(BYTE *bytecode, DWORD size);
		BYTE *generate();
	};
}