#include "Handler.h"
#include "types.h"
#include "Optable.h"

namespace KNVM {
	void _Private Handler::mov(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack) {
		auto opsize = dpinfo->opcode_size;
		auto optype = dpinfo->opcode_type;
		auto op = dpinfo->opcodes;
		auto lreg = reg[op[0]];
		auto rval = &op[1];

		if (optype == OP_TYPE_IMM2) {
			lreg = *(DWORD *)rval;
		}
		else if (optype == OP_TYPE_REG2) {
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
		DWORD sbase = (DWORD)stack.get();

		if (optype == OP_TYPE_IMM) {
			if (reg["esp"] <= sbase)
				throw "Stack Memory Top Reached";

			reg["esp"] -= stack.getAlign();
			DWORD *ptr = *reg["esp"];
			*ptr = *(DWORD *)lval;
		}
		else if (optype == OP_TYPE_REG) {
			if (reg["esp"] <= sbase)
				throw "Stack Memory Top Reached";

			reg["esp"] -= stack.getAlign();
			DWORD val = reg[*lval].get();
			DWORD *ptr = *reg["esp"];
			*ptr = val;
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
		DWORD sbase = (DWORD)stack.get();

		if (optype == OP_TYPE_REG) {
			if (reg["esp"] >= sbase + stack.getSize())
				throw "Stack Memory Exceeded";

			DWORD *ptr = *reg["esp"];
			DWORD val = *ptr;
			reg["esp"] += stack.getAlign();

			reg[*lval] = val;
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
		DWORD sbase = (DWORD)stack.get();

		if (optype == OP_TYPE_REG) {
			if (reg["esp"] >= sbase + stack.getSize())
				throw "Stack Memory Exceeded";

			reg["eip"] -= 1;

			DWORD *ptr = *reg["esp"];
			reg["eip"] = *ptr;
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
		Register<> prev_lreg = lreg;

		if (optype == OP_TYPE_IMM2) {
			lreg += *(DWORD *)rval;
		}
		else if (optype == OP_TYPE_REG2) {
			lreg += reg[*rval];
		}
		else {
			throw "Unknown Operand Type";
		}

		if (lreg == 0)
			setZF(reg);
		if (lreg < prev_lreg) {
			setCF(reg);
			setOF(reg);
		}
	}
	void _Private Handler::sub(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack) {
		auto opsize = dpinfo->opcode_size;
		auto optype = dpinfo->opcode_type;
		auto op = dpinfo->opcodes;
		auto &lreg = reg[op[0]];
		auto rval = &op[1];
		Register<> prev_lreg = lreg;

		if (optype == OP_TYPE_IMM2) {
			lreg -= *(DWORD *)rval;
		}
		else if (optype == OP_TYPE_REG2) {
			lreg -= reg[*rval];
		}
		else {
			throw "Unknown Operand Type";
		}

		if (lreg == 0)
			setZF(reg);
		if (lreg > prev_lreg) {
			setCF(reg);
			setOF(reg);
		}
	}
	void _Private Handler::mul(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack) {
		auto opsize = dpinfo->opcode_size;
		auto optype = dpinfo->opcode_type;
		auto op = dpinfo->opcodes;
		auto &lreg = reg[op[0]];
		auto rval = &op[1];
		Register<> prev_lreg = lreg;

		if (optype == OP_TYPE_IMM2) {
			lreg *= *(DWORD *)rval;
		}
		else if (optype == OP_TYPE_REG2) {
			lreg *= reg[*rval];
		}
		else {
			throw "Unknown Operand Type";
		}

		if (lreg == 0)
			setZF(reg);
		if (lreg < prev_lreg) {
			setCF(reg);
			setOF(reg);
		}
	}
	void _Private Handler::div(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack) {
		auto opsize = dpinfo->opcode_size;
		auto optype = dpinfo->opcode_type;
		auto op = dpinfo->opcodes;
		auto &lreg = reg[op[0]];
		auto rval = &op[1];

		if (optype == OP_TYPE_IMM2) {
			if (*(DWORD *)rval == 0)
				throw "Divide by zero";
			lreg /= *(DWORD *)rval;
		}
		else if (optype == OP_TYPE_REG2) {
			auto &val = reg[*rval];
			if (val == 0)
				throw "Divide by zero";
			lreg /= val;
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

		if (optype == OP_TYPE_IMM2) {
			lreg &= *(DWORD *)rval;
		}
		else if (optype == OP_TYPE_REG2) {
			lreg &= reg[*rval];
		}
		else {
			throw "Unknown Operand Type";
		}

		if (lreg == 0)
			setZF(reg);
	}
	void _Private Handler::or(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack) {
		auto opsize = dpinfo->opcode_size;
		auto optype = dpinfo->opcode_type;
		auto op = dpinfo->opcodes;
		auto &lreg = reg[op[0]];
		auto rval = &op[1];

		if (optype == OP_TYPE_IMM2) {
			lreg |= *(DWORD *)rval;
		}
		else if (optype == OP_TYPE_REG2) {
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

		if (optype == OP_TYPE_IMM2) {
			lreg ^= *(DWORD *)rval;
		}
		else if (optype == OP_TYPE_REG2) {
			lreg ^= reg[*rval];
		}
		else {
			throw "Unknown Operand Type";
		}

		if (lreg == 0)
			setZF(reg);
	}

	void _Private Handler::jmp(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack) {
		auto opsize = dpinfo->opcode_size;
		auto optype = dpinfo->opcode_type;
		auto op = dpinfo->opcodes;
		auto eip = reg["eip"].get();

		if (optype == OP_TYPE_IMM) {
			DWORD offset = *(DWORD *)&op[0];
			reg["eip"] = eip + opsize + offset;
		}
		else if (optype == OP_TYPE_REG) {
			auto &lreg = reg[op[0]];
			reg["eip"] = lreg;
		}
		else {
			throw "Unknown Operand Type";
		}
	}
	void _Private Handler::je(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack) {
		auto opsize = dpinfo->opcode_size;
		auto optype = dpinfo->opcode_type;
		auto op = dpinfo->opcodes;
		auto eip = reg["eip"].get();

		if (optype == OP_TYPE_IMM) {
			DWORD offset = *(DWORD *)&op[0];
			if(getZF(reg))
				reg["eip"] = eip + opsize + offset;
		}
		else if (optype == OP_TYPE_REG) {
			auto &lreg = reg[op[0]];
			if(getZF(reg))
				reg["eip"] = lreg;
		}
		else {
			throw "Unknown Operand Type";
		}
	}
	void _Private Handler::jne(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack) {
		auto opsize = dpinfo->opcode_size;
		auto optype = dpinfo->opcode_type;
		auto op = dpinfo->opcodes;
		auto eip = reg["eip"].get();

		if (optype == OP_TYPE_IMM) {
			DWORD offset = *(DWORD *)&op[0];
			if (!getZF(reg))
				reg["eip"] = eip + opsize + offset;
		}
		else if (optype == OP_TYPE_REG) {
			auto &lreg = reg[op[0]];
			if (!getZF(reg))
				reg["eip"] = lreg;
		}
		else {
			throw "Unknown Operand Type";
		}
	}
	void _Private Handler::ja(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack) {
		auto opsize = dpinfo->opcode_size;
		auto optype = dpinfo->opcode_type;
		auto op = dpinfo->opcodes;
		auto eip = reg["eip"].get();

		if (optype == OP_TYPE_IMM) {
			DWORD offset = *(DWORD *)&op[0];
			if(!getCF(reg) && !getZF(reg))
				reg["eip"] = eip + opsize + offset;
		}
		else if (optype == OP_TYPE_REG) {
			auto &lreg = reg[op[0]];
			if (!getCF(reg) && !getZF(reg))
				reg["eip"] = lreg;
		}
		else {
			throw "Unknown Operand Type";
		}
	}
	void _Private Handler::jb(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack) {
		auto opsize = dpinfo->opcode_size;
		auto optype = dpinfo->opcode_type;
		auto op = dpinfo->opcodes;
		auto eip = reg["eip"].get();

		if (optype == OP_TYPE_IMM) {
			DWORD offset = *(DWORD *)&op[0];
			if (getCF(reg))
				reg["eip"] = eip + opsize + offset;
		}
		else if (optype == OP_TYPE_REG) {
			auto &lreg = reg[op[0]];
			if (getCF(reg))
				reg["eip"] = lreg;
		}
		else {
			throw "Unknown Operand Type";
		}
	}
	void _Private Handler::jl(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack) {
		auto opsize = dpinfo->opcode_size;
		auto optype = dpinfo->opcode_type;
		auto op = dpinfo->opcodes;
		auto eip = reg["eip"].get();

		if (optype == OP_TYPE_IMM) {
			DWORD offset = *(DWORD *)&op[0];
			if(getNF(reg) != getOF(reg))
				reg["eip"] = eip + opsize + offset;
		}
		else if (optype == OP_TYPE_REG) {
			auto &lreg = reg[op[0]];
			if (getNF(reg) != getOF(reg))
				reg["eip"] = lreg;
		}
		else {
			throw "Unknown Operand Type";
		}
	}
	void _Private Handler::jle(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack) {
		auto opsize = dpinfo->opcode_size;
		auto optype = dpinfo->opcode_type;
		auto op = dpinfo->opcodes;
		auto eip = reg["eip"].get();

		if (optype == OP_TYPE_IMM) {
			DWORD offset = *(DWORD *)&op[0];
			if (getZF(reg) || getNF(reg) != getOF(reg))
				reg["eip"] = eip + opsize + offset;
		}
		else if (optype == OP_TYPE_REG) {
			auto &lreg = reg[op[0]];
			if (getZF(reg) || getNF(reg) != getOF(reg))
				reg["eip"] = lreg;
		}
		else {
			throw "Unknown Operand Type";
		}
	}
	void _Private Handler::jz(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack) {
		auto opsize = dpinfo->opcode_size;
		auto optype = dpinfo->opcode_type;
		auto op = dpinfo->opcodes;
		auto eip = reg["eip"].get();

		if (optype == OP_TYPE_IMM) {
			DWORD offset = *(DWORD *)&op[0];
			if(getZF(reg))
				reg["eip"] = eip + opsize + offset;
		}
		else if (optype == OP_TYPE_REG) {
			auto &lreg = reg[op[0]];
			if(getZF(reg))
				reg["eip"] = lreg;
		}
		else {
			throw "Unknown Operand Type";
		}
	}
	void _Private Handler::exit(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack) {
		auto opsize = dpinfo->opcode_size;
		auto optype = dpinfo->opcode_type;
		auto op = dpinfo->opcodes;
		auto lval = &op[0];
		DWORD sbase = (DWORD)stack.get();

		if (optype == OP_TYPE_REG) {
			throw "Program Exit";
		}
		else {
			throw "Unknown Operand Type";
		}
	}

	void _Private Handler::add_except(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack) {
		auto opsize = dpinfo->opcode_size;
		auto optype = dpinfo->opcode_type;
		auto op = dpinfo->opcodes;

		if (optype == OP_TYPE_IMM) {
			DWORD *ptr = (DWORD *)&op[0];
			fnExp.add_except(ptr);
		}
		else if (optype == OP_TYPE_REG) {
			auto &lreg = reg[op[0]];
			fnExp.add_except((void *)lreg.get());
		}
		else {
			throw "Unknown Operand Type";
		}
	}
	void _Private Handler::del_except(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack) {
		auto opsize = dpinfo->opcode_size;
		auto optype = dpinfo->opcode_type;
		auto op = dpinfo->opcodes;

		if (optype == OP_TYPE_IMM) {
			DWORD *ptr = (DWORD *)&op[0];
			fnExp.del_except(ptr);
		}
		else if (optype == OP_TYPE_REG) {
			auto &lreg = reg[op[0]];
			fnExp.del_except((void *)lreg.get());
		}
		else {
			throw "Unknown Operand Type";
		}
	}
	void _Private Handler::call_except(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack) {
		auto opsize = dpinfo->opcode_size;
		auto optype = dpinfo->opcode_type;
		auto op = dpinfo->opcodes;
		auto eip = reg["eip"].get();

		if (optype == OP_TYPE_IMM) {
			DWORD *ptr = (DWORD *)&op[0];
			if (!fnExp.is_func(ptr))
				throw "Not Defined Function";
			
			DWORD retaddr = eip  + opsize;
			reg["eip"] = (DWORD)ptr;
			reg["esp"] -= stack.getAlign();
			ptr = *reg["esp"];
			*ptr = retaddr;
		}
		else if (optype == OP_TYPE_REG) {
			auto &lreg = reg[op[0]];
			DWORD *ptr = *lreg;
			if (!fnExp.is_func(ptr))
				throw "Not Defined Function";

			DWORD retaddr = eip + opsize;
			reg["eip"] = (DWORD)ptr;
			reg["esp"] -= stack.getAlign();
			ptr = *reg["esp"];
			*ptr = retaddr;
		}
		else {
			throw "Unknown Operand Type";
		}
	}

	void _Private Handler::cmp(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack) {
		auto opsize = dpinfo->opcode_size;
		auto optype = dpinfo->opcode_type;
		auto op = dpinfo->opcodes;
		auto &lreg = reg[op[0]];
		auto rval = &op[1];
		Register<> prev_lreg = lreg;

		DWORD result;
		if (optype == OP_TYPE_IMM2) {
			result = lreg - *(DWORD *)rval;
		}
		else if (optype == OP_TYPE_REG2) {
			result = lreg - reg[*rval];
		}
		else {
			throw "Unknown Operand Type";
		}

		if (result == 0)
			setZF(reg);
		if (prev_lreg < result) {
			setCF(reg);
			setOF(reg);
		}
	}
	void _Private Handler::test(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack) {
		auto opsize = dpinfo->opcode_size;
		auto optype = dpinfo->opcode_type;
		auto op = dpinfo->opcodes;
		auto &lreg = reg[op[0]];
		auto rval = &op[1];
		Register<> prev_lreg = lreg;

		DWORD result;
		if (optype == OP_TYPE_IMM2) {
			result = lreg & *(DWORD *)rval;
		}
		else if (optype == OP_TYPE_REG2) {
			result = lreg & reg[*rval];
		}
		else {
			throw "Unknown Operand Type";
		}

		if (result == 0)
			setZF(reg);
		if (prev_lreg < result) {
			setCF(reg);
			setOF(reg);
		}
	}

	inline void _Private Handler::setZF(RegisterList<> &reg) { reg["flags"] = reg["flags"].get() | 0b00001000; }
	inline bool _Private Handler::getZF(RegisterList<> &reg) { return ((reg["flags"].get() & 0b00000100) >> 3) == 1; }
	inline void _Private Handler::setCF(RegisterList<> &reg) { reg["flags"] = reg["flags"].get() | 0b00010000; }
	inline bool _Private Handler::getCF(RegisterList<> &reg) { return ((reg["flags"].get() & 0b0000100) >> 4) == 1; }
	inline void _Private Handler::setOF(RegisterList<> &reg) { reg["flags"] = reg["flags"].get() | 0b00100000; }
	inline bool _Private Handler::getOF(RegisterList<> &reg) { return ((reg["flags"].get() & 0b0000100) >> 5) == 1; }
	inline void _Private Handler::setNF(RegisterList<> &reg) { reg["flags"] = reg["flags"].get() | 0b00100000; }
	inline bool _Private Handler::getNF(RegisterList<> &reg) { return ((reg["flags"].get() & 0b0000100) >> 6) == 1; }
	inline void _Private Handler::setDF(RegisterList<> &reg) { reg["flags"] = reg["flags"].get() | 0b01000000; }
	inline bool _Private Handler::getDF(RegisterList<> &reg) { return ((reg["flags"].get() & 0b0000100) >> 7) == 1; }

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
		case OP_CMP:
			this->cmp(dpinfo, reg, stack);
			break;
		case OP_TEST:
			this->test(dpinfo, reg, stack);
			break;
		case OP_EXIT:
			this->exit(dpinfo, reg, stack);
			break;
		case OP_ADD_EXCEPT:
			this->add_except(dpinfo, reg, stack);
			break;
		case OP_DEL_EXCEPT:
			this->del_except(dpinfo, reg, stack);
			break;
		case OP_EXCEPT_CALL:
			this->call_except(dpinfo, reg, stack);
			break;
		}
	}
}