#pragma once

#include "Optable.h"

#include <string>

namespace KNVM {
	class Memory;

	class Asm {
	private:
		void setOptype(BYTE *op, BYTE type) { op[0] = op[0] | (type << 6); }
		void setOp(BYTE *op, BYTE oper) { op[0] = op[0] | (oper & 0b00111111); }
		void setReg(BYTE *op, BYTE reg) { op[1] = reg; }
		void setVal(BYTE *op, DWORD val) { *(DWORD *)&op[2] = val; }

		std::string reg[10] = { "eax", "ebx", "ecx", "edx", "esi", "edi", "esp", "ebp", "eip", "flags" };

		bool is_imm = false;
		bool is_reg = false;
		BYTE code[6];

	public:
		// Asm(OP_MOV, "eax", 123);
		// Asm(OP_MOV, "eax", "ebx");
		// Asm(OP_PUSH, 1234);
		// Asm(OP_PUSH, "eax");
		// Asm(OP_RET);
		Asm(BYTE op, std::string reg, DWORD imm) : is_imm(true) {
			setOptype(code, OP_TYPE_IMM);
			setOp(code, op);
			setVal(code, imm);
		}
		Asm(BYTE op, std::string lreg, std::string rreg) : is_reg(true) {
			setOptype(code, OP_TYPE_IMM);
			setOp(code, op);
			//setVal(code, rreg.);
		}

		friend class Memory;
	};
}