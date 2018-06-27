#include "types.h"
#include "Handler.h"
#include "Optable.h"

namespace KNVM {
	void _Private Handler::mov(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack) {
		auto opsize = dpinfo->opcode_size;
		auto optype = dpinfo->opcode_type;
		auto op = dpinfo->opcodes;
		auto lreg = reg[op[0]];
		auto rval = &op[1];

		if (optype == OP_TYPE_IMM) {
			lreg = *(DWORD *)rval;
		}
		else if (optype == OP_TYPE_REG) {
			lreg = reg[*rval];
		}
		else {
			throw "Unknown Operand Type";
		}
	}
	void _Private Handler::push(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack) {
		auto opsize = dpinfo->opcode_size;
		auto optype = dpinfo->opcode_type;
		auto op = dpinfo->opcodes;
		auto lval = &op[0];

		if (optype == OP_TYPE_IMM) {
			reg["esp"] -= stack.getAlign();
			DWORD *ptr = *reg["esp"];
			*ptr = *(DWORD *)lval;
		}
		else if (optype == OP_TYPE_REG) {
			reg["esp"] -= stack.getAlign();
			DWORD *ptr = *reg["esp"];
			*ptr = *(DWORD *)lval;
		}
		else {
			throw "Unknown Operand Type";
		}
	}
	void _Private Handler::pop(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack) {
		auto opsize = dpinfo->opcode_size;
		auto optype = dpinfo->opcode_type;
		auto op = dpinfo->opcodes;
		auto lval = &op[0];

		if (optype == OP_TYPE_REG) {
			reg[*lval] = reg["esp"];
			reg["esp"] += stack.getAlign();
		}
		else {
			throw "Unknown Operand Type";
		}
	}
	void _Private Handler::ret(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack) {
		auto opsize = dpinfo->opcode_size;
		auto optype = dpinfo->opcode_type;
		auto op = dpinfo->opcodes;
		auto lval = &op[0];

		if (optype == OP_TYPE_REG) {
			reg["eip"] = reg["esp"];
			reg["esp"] += stack.getAlign();
		}
		else {
			throw "Unknown Operand Type";
		}
	}

	void _Private Handler::add(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack) {
		auto opsize = dpinfo->opcode_size;
		auto optype = dpinfo->opcode_type;
		auto op = dpinfo->opcodes;
		auto &lreg = reg[op[0]];
		auto rval = &op[1];

		if (optype == OP_TYPE_IMM) {
			lreg += *(DWORD *)rval;
		}
		else if (optype == OP_TYPE_REG) {
			lreg += reg[*rval];
		}
		else {
			throw "Unknown Operand Type";
		}

		if (lreg == 0)
			setZF(reg);
	}
	void _Private Handler::sub(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack) {
		auto opsize = dpinfo->opcode_size;
		auto optype = dpinfo->opcode_type;
		auto op = dpinfo->opcodes;
		auto &lreg = reg[op[0]];
		auto rval = &op[1];

		if (optype == OP_TYPE_IMM) {
			lreg -= *(DWORD *)rval;
		}
		else if (optype == OP_TYPE_REG) {
			lreg -= reg[*rval];
		}
		else {
			throw "Unknown Operand Type";
		}

		if (lreg == 0)
			setZF(reg);
	}
	void _Private Handler::mul(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack) {
		auto opsize = dpinfo->opcode_size;
		auto optype = dpinfo->opcode_type;
		auto op = dpinfo->opcodes;
		auto &lreg = reg[op[0]];
		auto rval = &op[1];

		if (optype == OP_TYPE_IMM) {
			lreg *= *(DWORD *)rval;
		}
		else if (optype == OP_TYPE_REG) {
			lreg *= reg[*rval];
		}
		else {
			throw "Unknown Operand Type";
		}

		if (lreg == 0)
			setZF(reg);
	}
	void _Private Handler::div(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack) {
		auto opsize = dpinfo->opcode_size;
		auto optype = dpinfo->opcode_type;
		auto op = dpinfo->opcodes;
		auto &lreg = reg[op[0]];
		auto rval = &op[1];

		if (optype == OP_TYPE_IMM) {
			lreg /= *(DWORD *)rval;
		}
		else if (optype == OP_TYPE_REG) {
			lreg /= reg[*rval];
		}
		else {
			throw "Unknown Operand Type";
		}

		if (lreg == 0)
			setZF(reg);
	}

	void _Private Handler::and(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack) {
		auto opsize = dpinfo->opcode_size;
		auto optype = dpinfo->opcode_type;
		auto op = dpinfo->opcodes;
		auto &lreg = reg[op[0]];
		auto rval = &op[1];

		if (optype == OP_TYPE_IMM) {
			lreg &= *(DWORD *)rval;
		}
		else if (optype == OP_TYPE_REG) {
			lreg &= reg[*rval];
		}
		else {
			throw "Unknown Operand Type";
		}

		if (lreg == 0)
			setZF(reg);
	}
	void _Private Handler::or (DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack) {
		auto opsize = dpinfo->opcode_size;
		auto optype = dpinfo->opcode_type;
		auto op = dpinfo->opcodes;
		auto &lreg = reg[op[0]];
		auto rval = &op[1];

		if (optype == OP_TYPE_IMM) {
			lreg |= *(DWORD *)rval;
		}
		else if (optype == OP_TYPE_REG) {
			lreg |= reg[*rval];
		}
		else {
			throw "Unknown Operand Type";
		}

		if (lreg == 0)
			setZF(reg);
	}
	void _Private Handler::xor(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack) {
		auto opsize = dpinfo->opcode_size;
		auto optype = dpinfo->opcode_type;
		auto op = dpinfo->opcodes;
		auto &lreg = reg[op[0]];
		auto rval = &op[1];

		if (optype == OP_TYPE_IMM) {
			lreg ^= *(DWORD *)rval;
		}
		else if (optype == OP_TYPE_REG) {
			lreg ^= reg[*rval];
		}
		else {
			throw "Unknown Operand Type";
		}

		if (lreg == 0)
			setZF(reg);
	}

	void _Private Handler::jmp(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack) {

	}
	void _Private Handler::je(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack) {

	}
	void _Private Handler::jne(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack) {

	}
	void _Private Handler::ja(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack) {

	}
	void _Private Handler::jb(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack) {

	}
	void _Private Handler::jl(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack) {

	}
	void _Private Handler::jle(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack) {

	}
	void _Private Handler::jz(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack) {

	}

	void _Private Handler::add_except(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack) {

	}
	void _Private Handler::del_except(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack) {

	}
	void _Private Handler::mod_except(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack) {

	}
	void _Private Handler::call_except(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack) {

	}

	inline void _Private Handler::setZF(RegisterList<> &reg) {
		reg["flags"] = reg["flags"].get() | 0b00000100;
	}

	void _Public Handler::handle(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack) {
		if (dpinfo == nullptr)
			return;
		
		switch (dpinfo->opcode) {
		case OP_PUSH:
			this->push(dpinfo, reg, stack);
			break;
		case OP_POP:
			this->pop(dpinfo, reg, stack);
			break;
		case OP_MOV:
			this->mov(dpinfo, reg, stack);
			break;
		case OP_RET:
			this->ret(dpinfo, reg, stack);
			break;
		case OP_ADD:
			this->add(dpinfo, reg, stack);
			break;
		case OP_SUB:
			this->sub(dpinfo, reg, stack);
			break;
		case OP_MUL:
			this->mul(dpinfo, reg, stack);
			break;
		case OP_DIV:
			this->mul(dpinfo, reg, stack);
			break;
		case OP_AND:
			this->and(dpinfo, reg, stack);
			break;
		case OP_OR:
			this->or(dpinfo, reg, stack);
			break;
		case OP_XOR:
			this->xor(dpinfo, reg, stack);
			break;
		case OP_JMP:
			this->jmp(dpinfo, reg, stack);
			break;
		case OP_JE:
			this->jmp(dpinfo, reg, stack);
			break;
		case OP_JNE:
			this->jne(dpinfo, reg, stack);
			break;
		case OP_JA:
			this->ja(dpinfo, reg, stack);
			break;
		case OP_JB:
			this->jb(dpinfo, reg, stack);
			break;
		case OP_JL:
			this->jl(dpinfo, reg, stack);
			break;
		case OP_JLE:
			this->jle(dpinfo, reg, stack);
			break;
		case OP_JZ:
			this->jz(dpinfo, reg, stack);
			break;
		case OP_ADD_EXCEPT:
			this->add_except(dpinfo, reg, stack);
			break;
		case OP_DEL_EXCEPT:
			this->del_except(dpinfo, reg, stack);
			break;
		case OP_MOD_EXCEPT:
			this->mod_except(dpinfo, reg, stack);
			break;
		case OP_EXCEPT_CALL:
			this->call_except(dpinfo, reg, stack);
			break;
		}
	}
}