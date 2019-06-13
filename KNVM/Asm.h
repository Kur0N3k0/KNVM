#pragma once

#include "Optable.h"

#include <string>
#include <vector>
#include <Windows.h>

namespace KNVM {
	class Memory;
	class Function;
	class CodeGenerator;

	class Asm {
	protected:
		void setOptype(BYTE *op, BYTE type) { op[0] = op[0] | (type << 6); }
		void setOp(BYTE *op, BYTE oper) { op[0] = op[0] | (oper & 0b00111111); }
		void setLReg(BYTE *op, BYTE reg, bool indirect = false) { 
			if (indirect)
				op[0] = (OP_TYPE_PTR_REG << 6) | reg;
			else
				op[0] = (OP_TYPE_REG << 6) | reg;
		}
		void setRReg(BYTE *op, BYTE reg, bool indirect = false) { 
			if (indirect)
				op[0] = (OP_TYPE_PTR_REG << 6) | reg;
			else
				op[0] = (OP_TYPE_REG << 6) | reg;
		}
		void setVal(BYTE *op, DWORD val, bool indirect = false) {
			if (indirect)
				op[0] = (OP_TYPE_PTR_IMM << 6);
			else
				op[0] = (OP_TYPE_IMM << 6);
				
			*(DWORD *)&op[1] = val;
		}
		BYTE getRegByte(DWORD idx) { return (BYTE)idx; }//(BYTE)(0b00000001 << (idx - 1)); }
		BYTE getRegIdx(std::string &r) {
			BYTE i = 0;
			for (i = 0; i < 8; i++)
				if (r == this->reg[i])
					return i;
			return -1;
		}

		std::vector<std::string> reg = { "eax", "ebx", "ecx", "edx", "esi", "edi", "esp", "ebp", "eip", "flags" };

		BYTE code[10] = { 0, };
		BYTE codesize;

	public:
		// Asm(OP_MOV, "eax", 123);
		// Asm(OP_MOV, "eax", "ebx");
		// Asm(OP_PUSH, 1234);
		// Asm(OP_PUSH, "eax");
		// Asm(OP_RET);
		Asm(BYTE op, std::string reg, DWORD imm) {
			bool indirect = false;
			if (reg[0] == '[' && reg[reg.length() - 1] == ']') {
				reg = reg.substr(1, reg.length() - 2);
				indirect = true;
			}

			setOptype(code, 2);
			setOp(code, op);
			setLReg(code + 1, getRegByte(getRegIdx(reg)), indirect);
			setVal(code + 2, imm);
			codesize = 7;
		}
		Asm(BYTE op, std::string lreg, std::string rreg) {
			bool indirect = false;
			if (lreg[0] == '[' && lreg[lreg.length() - 1] == ']') {
				lreg = lreg.substr(1, lreg.length() - 2);
				indirect = true;
			}

			bool indirect2 = false;
			if (rreg[0] == '[' && rreg[rreg.length() - 1] == ']') {
				rreg = rreg.substr(1, rreg.length() - 2);
				indirect2 = true;
			}

			setOptype(code, 2);
			setOp(code, op);
			setLReg(code + 1, getRegByte(getRegIdx(lreg)), indirect);
			setRReg(code + 2, getRegByte(getRegIdx(rreg)), indirect2);
			codesize = 3;
		}
		Asm(BYTE op, DWORD val) {
			setOptype(code, 1);
			setOp(code, op);
			setVal(code + 1, val);
			codesize = 6;
		}
		Asm(BYTE op, std::string lreg) {
			bool indirect = false;
			if (lreg[0] == '[' && lreg[lreg.length() - 1] == ']') {
				lreg = lreg.substr(1, lreg.length() - 2);
				indirect = true;
			}

			setOptype(code, 1);
			setOp(code, op);
			setLReg(code + 1, getRegByte(getRegIdx(lreg)), indirect);
			codesize = 2;
		}
		Asm(BYTE op) {
			setOptype(code, 1);
			setOp(code, op);
			codesize = 1;
		}

		Asm(const Asm &asmbly){
			std::memcpy(code, asmbly.code, asmbly.codesize);
			codesize = asmbly.codesize;
		}
		
		friend class Memory;
		friend class Function;
		friend class Label;
		friend class CodeGenerator;
	};
}