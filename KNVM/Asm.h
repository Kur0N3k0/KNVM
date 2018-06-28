#pragma once

#include "Optable.h"

#include <string>

namespace KNVM {
	class Memory;

	class Asm {
	private:
		void setOptype(BYTE *op, BYTE type) { op[0] = op[0] | (type << 6); }
		void setOp(BYTE *op, BYTE oper) { op[0] = op[0] | (oper & 0b00111111); }
		void setLReg(BYTE *op, BYTE reg) { op[1] = reg; }
		void setRReg(BYTE *op, BYTE reg) { op[2] = reg; }
		void setVal(BYTE *op, DWORD val) {
			if (is_imm2)
				*(DWORD *)&op[2] = val;
			else
				*(DWORD *)&op[1] = val;
		}
		BYTE getRegByte(DWORD idx) { return (BYTE)(0b00000001 << (idx - 1)); }
		BYTE getRegIdx(std::string &reg) {
			BYTE i;
			for (i = 0; i < 8; i++)
				if (reg == this->reg[i])
					break;
			return i;
		}

		std::string reg[10] = { "eax", "ebx", "ecx", "edx", "esi", "edi", "esp", "ebp", "eip", "flags" };

		bool is_imm2 = false;
		BYTE code[6] = { 0, };
		BYTE codesize;

	public:
		// Asm(OP_MOV, "eax", 123);
		// Asm(OP_MOV, "eax", "ebx");
		// Asm(OP_PUSH, 1234);
		// Asm(OP_PUSH, "eax");
		// Asm(OP_RET);
		Asm(BYTE op, std::string reg, DWORD imm) : is_imm2(true) {
			setOptype(code, OP_TYPE_IMM2);
			setOp(code, op);
			setLReg(code, getRegByte(getRegIdx(reg)));
			setVal(code, imm);
			codesize = 6;
		}
		Asm(BYTE op, std::string lreg, std::string rreg) {
			setOptype(code, OP_TYPE_REG2);
			setOp(code, op);
			setLReg(code, getRegByte(getRegIdx(lreg)));
			setRReg(code, getRegByte(getRegIdx(rreg)));
			codesize = 3;
		}
		Asm(BYTE op, DWORD val) : is_imm2(false) {
			setOptype(code, OP_TYPE_IMM);
			setOp(code, op);
			setVal(code, val);
			codesize = 5;
		}
		Asm(BYTE op, std::string lreg) {
			setOptype(code, OP_TYPE_REG);
			setOp(code, op);
			setLReg(code, getRegByte(getRegIdx(lreg)));
			codesize = 2;
		}
		Asm(BYTE op) {
			setOptype(code, OP_TYPE_REG);
			setOp(code, op);
			codesize = 1;
		}

		Asm(const Asm &asmbly){
			std::memcpy(code, asmbly.code, 6);
			codesize = asmbly.codesize;
		}
		
		friend class Memory;
	};
}