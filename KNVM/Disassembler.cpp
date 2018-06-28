#include "Disassembler.h"
#include "Register.h"
#include "Optable.h"
#include "types.h"

#include <string>
#include <iostream>

namespace KNVM {
	std::string Disassembler::mov(DispatchInfo * dpinfo) {
		auto opsize = dpinfo->opcode_size;
		auto optype = dpinfo->opcode_type;
		auto op = dpinfo->opcodes;
		auto &reg = cpu.getRegister();
		auto lreg = reg[op[0]];
		auto rval = &op[1];
		std::string asmbly = "mov ";

		if (optype == OP_TYPE_IMM) {
			asmbly += join(lreg.getName(), hex(*(DWORD *)rval));
		}
		else if (optype == OP_TYPE_REG) {
			asmbly += join(lreg.getName(), reg[*rval].getName());
		}
		else {
			throw "Unknown Operand Type";
		}

		return asmbly;
	}
	std::string Disassembler::push(DispatchInfo * dpinfo) {
		auto opsize = dpinfo->opcode_size;
		auto optype = dpinfo->opcode_type;
		auto op = dpinfo->opcodes;
		auto &reg = cpu.getRegister();
		auto lval = &op[0];
		std::string asmbly = "push ";

		if (optype == OP_TYPE_IMM) {
			asmbly += join(hex(*(DWORD *)lval));
		}
		else if (optype == OP_TYPE_REG) {
			asmbly += join(reg[*lval].getName());
		}
		else {
			throw "Unknown Operand Type";
		}
		return asmbly;
	}
	std::string Disassembler::pop(DispatchInfo * dpinfo) {
		auto opsize = dpinfo->opcode_size;
		auto optype = dpinfo->opcode_type;
		auto op = dpinfo->opcodes;
		auto &reg = cpu.getRegister();
		auto lval = &op[0];
		std::string asmbly = "pop ";

		if (optype == OP_TYPE_REG) {
			asmbly += join(reg[*lval].getName());
		}
		else {
			throw "Unknown Operand Type";
		}
		return asmbly;
	}
	std::string Disassembler::ret(DispatchInfo * dpinfo) {
		auto opsize = dpinfo->opcode_size;
		auto optype = dpinfo->opcode_type;
		auto op = dpinfo->opcodes;

		if (optype == OP_TYPE_REG) {
			return "ret\n";
		}
		else {
			throw "Unknown Operand Type";
		}
	}
	std::string Disassembler::add(DispatchInfo * dpinfo) {
		auto opsize = dpinfo->opcode_size;
		auto optype = dpinfo->opcode_type;
		auto op = dpinfo->opcodes;
		auto &reg = cpu.getRegister();
		auto &lreg = reg[op[0]];
		auto rval = &op[1];
		std::string asmbly = "add ";

		if (optype == OP_TYPE_IMM) {
			asmbly += join(lreg.getName(), hex(*(DWORD *)rval));
		}
		else if (optype == OP_TYPE_REG) {
			asmbly += join(lreg.getName(), reg[*rval].getName());
		}
		else {
			throw "Unknown Operand Type";
		}
		return asmbly;
	}
	std::string Disassembler::sub(DispatchInfo * dpinfo) {
		auto opsize = dpinfo->opcode_size;
		auto optype = dpinfo->opcode_type;
		auto op = dpinfo->opcodes;
		auto &reg = cpu.getRegister();
		auto &lreg = reg[op[0]];
		auto rval = &op[1];
		std::string asmbly = "sub ";

		if (optype == OP_TYPE_IMM) {
			asmbly += join(lreg.getName(), hex(*(DWORD *)rval));
		}
		else if (optype == OP_TYPE_REG) {
			asmbly += join(lreg.getName(), reg[*rval].getName());
		}
		else {
			throw "Unknown Operand Type";
		}
		return asmbly;
	}
	std::string Disassembler::mul(DispatchInfo * dpinfo) {
		auto opsize = dpinfo->opcode_size;
		auto optype = dpinfo->opcode_type;
		auto op = dpinfo->opcodes;
		auto &reg = cpu.getRegister();
		auto &lreg = reg[op[0]];
		auto rval = &op[1];
		std::string asmbly = "mul ";

		if (optype == OP_TYPE_IMM) {
			asmbly += join(lreg.getName(), hex(*(DWORD *)rval));
		}
		else if (optype == OP_TYPE_REG) {
			asmbly += join(lreg.getName(), reg[*rval].getName());
		}
		else {
			throw "Unknown Operand Type";
		}
		return asmbly;
	}
	std::string Disassembler::div(DispatchInfo * dpinfo) {
		auto opsize = dpinfo->opcode_size;
		auto optype = dpinfo->opcode_type;
		auto op = dpinfo->opcodes;
		auto &reg = cpu.getRegister();
		auto &lreg = reg[op[0]];
		auto rval = &op[1];
		std::string asmbly = "div ";

		if (optype == OP_TYPE_IMM) {
			asmbly += join(lreg.getName(), hex(*(DWORD *)rval));
		}
		else if (optype == OP_TYPE_REG) {
			asmbly += join(lreg.getName(), reg[*rval].getName());
		}
		else {
			throw "Unknown Operand Type";
		}
		return asmbly;
	}

	std::string Disassembler:: and (DispatchInfo * dpinfo) {
		auto opsize = dpinfo->opcode_size;
		auto optype = dpinfo->opcode_type;
		auto op = dpinfo->opcodes;
		auto &reg = cpu.getRegister();
		auto &lreg = reg[op[0]];
		auto rval = &op[1];
		std::string asmbly = "and ";

		if (optype == OP_TYPE_IMM) {
			asmbly += join(lreg.getName(), hex(*(DWORD *)rval));
		}
		else if (optype == OP_TYPE_REG) {
			asmbly += join(lreg.getName(), reg[*rval].getName());
		}
		else {
			throw "Unknown Operand Type";
		}
		return asmbly;
	}
	std::string Disassembler:: or (DispatchInfo * dpinfo) {
		auto opsize = dpinfo->opcode_size;
		auto optype = dpinfo->opcode_type;
		auto op = dpinfo->opcodes;
		auto &reg = cpu.getRegister();
		auto &lreg = reg[op[0]];
		auto rval = &op[1];
		std::string asmbly = "or ";

		if (optype == OP_TYPE_IMM) {
			asmbly += join(lreg.getName(), hex(*(DWORD *)rval));
		}
		else if (optype == OP_TYPE_REG) {
			asmbly += join(lreg.getName(), reg[*rval].getName());
		}
		else {
			throw "Unknown Operand Type";
		}
		return asmbly;
	}
	std::string Disassembler:: xor (DispatchInfo * dpinfo) {
		auto opsize = dpinfo->opcode_size;
		auto optype = dpinfo->opcode_type;
		auto op = dpinfo->opcodes;
		auto &reg = cpu.getRegister();
		auto &lreg = reg[op[0]];
		auto rval = &op[1];
		std::string asmbly = "xor ";

		if (optype == OP_TYPE_IMM) {
			asmbly += join(lreg.getName(), hex(*(DWORD *)rval));
		}
		else if (optype == OP_TYPE_REG) {
			asmbly += join(lreg.getName(), reg[*rval].getName());
		}
		else {
			throw "Unknown Operand Type";
		}
		return asmbly;
	}
	std::string Disassembler::jmp(DispatchInfo * dpinfo) {
		auto opsize = dpinfo->opcode_size;
		auto optype = dpinfo->opcode_type;
		auto op = dpinfo->opcodes;
		auto &reg = cpu.getRegister();
		std::string asmbly = "jmp ";

		if (optype == OP_TYPE_IMM) {
			DWORD offset = *(DWORD *)&op[0];
			asmbly += join("$+" + hex(offset + opsize));
		}
		else if (optype == OP_TYPE_REG) {
			auto &lreg = reg[op[0]];
			asmbly += join(lreg.getName());
		}
		else {
			throw "Unknown Operand Type";
		}
		return asmbly;
	}
	std::string Disassembler::je(DispatchInfo * dpinfo) {
		auto opsize = dpinfo->opcode_size;
		auto optype = dpinfo->opcode_type;
		auto op = dpinfo->opcodes;
		auto &reg = cpu.getRegister();
		std::string asmbly = "je ";

		if (optype == OP_TYPE_IMM) {
			DWORD offset = *(DWORD *)&op[0];
			asmbly += join("$+" + hex(offset + opsize));
		}
		else if (optype == OP_TYPE_REG) {
			auto &lreg = reg[op[0]];
			asmbly += join(lreg.getName());
		}
		else {
			throw "Unknown Operand Type";
		}
		return asmbly;
	}
	std::string Disassembler::jne(DispatchInfo * dpinfo) {
		auto opsize = dpinfo->opcode_size;
		auto optype = dpinfo->opcode_type;
		auto op = dpinfo->opcodes;
		auto &reg = cpu.getRegister();
		std::string asmbly = "jne ";

		if (optype == OP_TYPE_IMM) {
			DWORD offset = *(DWORD *)&op[0];
			asmbly += join("$+" + hex(offset + opsize));
		}
		else if (optype == OP_TYPE_REG) {
			auto &lreg = reg[op[0]];
			asmbly += join(lreg.getName());
		}
		else {
			throw "Unknown Operand Type";
		}
		return asmbly;
	}
	std::string Disassembler::ja(DispatchInfo * dpinfo) {
		auto opsize = dpinfo->opcode_size;
		auto optype = dpinfo->opcode_type;
		auto op = dpinfo->opcodes;
		auto &reg = cpu.getRegister();
		std::string asmbly = "ja ";

		if (optype == OP_TYPE_IMM) {
			DWORD offset = *(DWORD *)&op[0];
			asmbly += join("$+" + hex(offset + opsize));
		}
		else if (optype == OP_TYPE_REG) {
			auto &lreg = reg[op[0]];
			asmbly += join(lreg.getName());
		}
		else {
			throw "Unknown Operand Type";
		}
		return asmbly;
	}
	std::string Disassembler::jb(DispatchInfo * dpinfo) {
		auto opsize = dpinfo->opcode_size;
		auto optype = dpinfo->opcode_type;
		auto op = dpinfo->opcodes;
		auto &reg = cpu.getRegister();
		std::string asmbly = "jb ";

		if (optype == OP_TYPE_IMM) {
			DWORD offset = *(DWORD *)&op[0];
			asmbly += join("$+" + hex(offset + opsize));
		}
		else if (optype == OP_TYPE_REG) {
			auto &lreg = reg[op[0]];
			asmbly += join(lreg.getName());
		}
		else {
			throw "Unknown Operand Type";
		}
		return asmbly;
	}
	std::string Disassembler::jl(DispatchInfo * dpinfo) {
		auto opsize = dpinfo->opcode_size;
		auto optype = dpinfo->opcode_type;
		auto op = dpinfo->opcodes;
		auto &reg = cpu.getRegister();
		std::string asmbly = "jl ";

		if (optype == OP_TYPE_IMM) {
			DWORD offset = *(DWORD *)&op[0];
			asmbly += join("$+" + hex(offset + opsize));
		}
		else if (optype == OP_TYPE_REG) {
			auto &lreg = reg[op[0]];
			asmbly += join(lreg.getName());
		}
		else {
			throw "Unknown Operand Type";
		}
		return asmbly;
	}
	std::string Disassembler::jle(DispatchInfo * dpinfo) {
		auto opsize = dpinfo->opcode_size;
		auto optype = dpinfo->opcode_type;
		auto op = dpinfo->opcodes;
		auto &reg = cpu.getRegister();
		std::string asmbly = "jle ";

		if (optype == OP_TYPE_IMM) {
			DWORD offset = *(DWORD *)&op[0];
			asmbly += join("$+" + hex(offset + opsize));
		}
		else if (optype == OP_TYPE_REG) {
			auto &lreg = reg[op[0]];
			asmbly += join(lreg.getName());
		}
		else {
			throw "Unknown Operand Type";
		}
		return asmbly;
	}
	std::string Disassembler::jz(DispatchInfo * dpinfo) {
		auto opsize = dpinfo->opcode_size;
		auto optype = dpinfo->opcode_type;
		auto op = dpinfo->opcodes;
		auto &reg = cpu.getRegister();
		std::string asmbly = "jz ";

		if (optype == OP_TYPE_IMM) {
			DWORD offset = *(DWORD *)&op[0];
			asmbly += join("$+" + hex(offset + opsize));
		}
		else if (optype == OP_TYPE_REG) {
			auto &lreg = reg[op[0]];
			asmbly += join(lreg.getName());
		}
		else {
			throw "Unknown Operand Type";
		}
		return asmbly;
	}
	std::string Disassembler::add_except(DispatchInfo * dpinfo) {
		auto opsize = dpinfo->opcode_size;
		auto optype = dpinfo->opcode_type;
		auto op = dpinfo->opcodes;
		auto &reg = cpu.getRegister();
		std::string asmbly = "add.except ";

		if (optype == OP_TYPE_IMM) {
			DWORD *ptr = (DWORD *)&op[0];
			asmbly += join(hex(*ptr));
		}
		else if (optype == OP_TYPE_REG) {
			auto &lreg = reg[op[0]];
			asmbly += join(lreg.getName());
		}
		else {
			throw "Unknown Operand Type";
		}
		return asmbly;
	}
	std::string Disassembler::del_except(DispatchInfo * dpinfo) {
		auto opsize = dpinfo->opcode_size;
		auto optype = dpinfo->opcode_type;
		auto op = dpinfo->opcodes;
		auto &reg = cpu.getRegister();
		std::string asmbly = "del.except ";

		if (optype == OP_TYPE_IMM) {
			DWORD *ptr = (DWORD *)&op[0];
			asmbly += join(hex(*ptr));
		}
		else if (optype == OP_TYPE_REG) {
			auto &lreg = reg[op[0]];
			asmbly += join(lreg.getName());
		}
		else {
			throw "Unknown Operand Type";
		}
		return asmbly;
	}
	std::string Disassembler::call_except(DispatchInfo * dpinfo) {
		auto opsize = dpinfo->opcode_size;
		auto optype = dpinfo->opcode_type;
		auto op = dpinfo->opcodes;
		auto &reg = cpu.getRegister();
		std::string asmbly = "call.except ";

		if (optype == OP_TYPE_IMM) {
			DWORD *ptr = (DWORD *)&op[0];
			asmbly += join(hex(*ptr));
		}
		else if (optype == OP_TYPE_REG) {
			auto &lreg = reg[op[0]];
			asmbly += join(lreg.getName());
		}
		else {
			throw "Unknown Operand Type";
		}
		return asmbly;
	}

	std::string Disassembler::disassemble() {
		auto &reg = cpu.getRegister();
		reg["eip"] = (DWORD)code.get();

		std::string result = "";
		while(true){
			try {
				DispatchInfo *dpinfo = cpu.dispatch(reg["eip"], code);
				std::cout << "----------------------------" << std::endl;
				std::cout << "eip : 0x" << std::hex << reg["eip"].get() << std::endl;
				std::cout << "opsize : " << dpinfo->opcode_size << std::endl;
				reg["eip"] += dpinfo->opcode_size;
				if (dpinfo == nullptr || reg["eip"] > (DWORD)code.get() + code.getSize())
					throw "Dispatch Fail";

				switch (dpinfo->opcode) {
				case OP_PUSH:
					result += this->push(dpinfo);
					break;
				case OP_POP:
					result += this->pop(dpinfo);
					break;
				case OP_MOV:
					result += this->mov(dpinfo);
					break;
				case OP_RET:
					result += this->ret(dpinfo);
					break;
				case OP_ADD:
					result += this->add(dpinfo);
					break;
				case OP_SUB:
					result += this->sub(dpinfo);
					break;
				case OP_MUL:
					result += this->mul(dpinfo);
					break;
				case OP_DIV:
					result += this->mul(dpinfo);
					break;
				case OP_AND:
					result += this-> and (dpinfo);
					break;
				case OP_OR:
					result += this-> or (dpinfo);
					break;
				case OP_XOR:
					result += this-> xor (dpinfo);
					break;
				case OP_JMP:
					result += this->jmp(dpinfo);
					break;
				case OP_JE:
					result += this->jmp(dpinfo);
					break;
				case OP_JNE:
					result += this->jne(dpinfo);
					break;
				case OP_JA:
					result += this->ja(dpinfo);
					break;
				case OP_JB:
					result += this->jb(dpinfo);
					break;
				case OP_JL:
					result += this->jl(dpinfo);
					break;
				case OP_JLE:
					result += this->jle(dpinfo);
					break;
				case OP_JZ:
					result += this->jz(dpinfo);
					break;
				case OP_ADD_EXCEPT:
					result += this->add_except(dpinfo);
					break;
				case OP_DEL_EXCEPT:
					result += this->del_except(dpinfo);
					break;
				case OP_EXCEPT_CALL:
					result += this->call_except(dpinfo);
					break;
				}

				delete dpinfo;
				dpinfo = cpu.dispatch(reg["eip"], code);
			}
			catch (const char *err) {
				break;
			}
			catch (std::exception exp) {
				std::cout << exp.what() << std::endl;
				break;
			}
		}

		return result;
	}
}