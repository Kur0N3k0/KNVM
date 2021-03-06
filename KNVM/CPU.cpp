#include "CPU.h"

#include <iostream>
#include <string>

namespace KNVM {
	void _Private CPU::genTestcase(Memory &code) {
		auto setOptype = [=](BYTE *op, BYTE type) {
			op[0] = op[0] | (type << 6);
		};
		auto setOp = [=](BYTE *op, BYTE oper) {
			op[0] = op[0] | (oper & 0b00111111);
		};

		BYTE *addr = (BYTE *)code.get();
		setOptype(addr, OP_TYPE_REG);
		setOp(addr, OP_POP);
		
		*((BYTE *)(addr + 1)) = 0x01;
	}

	void _Public CPU::execute(Memory &code, Memory &stack) {
		reg["eax"] = (DWORD)code.get();
		reg["ebx"] = (DWORD)code.get() - OFFSET_CODE + OFFSET_DATA;
		reg["eip"] = (DWORD)code.get();
		reg["esp"] = (DWORD)stack.get() + stack.getSize() - 4;
		reg["ebp"] = reg["esp"];

		while (true) {
			try {
				DispatchInfo *dpinfo = dispatch(reg["eip"], code);

				if (dpinfo == nullptr)
					throw "DispatchInfo == nullptr";
				
				reg["eip"] += dpinfo->opcode_size;
				if (dpinfo == nullptr || reg["eip"] > (DWORD)code.get() + code.getCodeSize() + 6)
					throw "Dispatch Fail";
				
				handler.handle(dpinfo, reg, stack);

				delete dpinfo;
			}
			catch (const char *err) {
				std::cout << __FUNCTION__ << " -> " << err << std::endl;
				break;
			}
			catch (std::exception exp) {
				std::cout << __FUNCTION__ << " -> " << exp.what() << std::endl;
			}
		}
	}
	void _Public CPU::execute(Memory &code, void *entrypoint, Memory &stack) {
		reg["eax"] = (DWORD)code.get();
		reg["ebx"] = (DWORD)code.get() - OFFSET_CODE + OFFSET_DATA;
		reg["eip"] = (DWORD)code.get() + (DWORD)entrypoint;
		reg["esp"] = (DWORD)stack.get() + stack.getSize() - 4;

		while (true) {
			try {
				DispatchInfo *dpinfo = dispatch(reg["eip"], code);

				if (dpinfo == nullptr)
					throw "DispatchInfo == nullptr";

				if (dpinfo == nullptr || reg["eip"] > (DWORD)code.get() + code.getCodeSize() + 6)
					throw "Dispatch Fail";

				DWORD handled_size = handler.handle(dpinfo, reg, stack);
				if (handled_size == -1)
					throw "Handling Fail";

				reg["eip"] += handled_size;

				delete dpinfo;
			}
			catch (const char *err) {
				std::cout << __FUNCTION__ << " -> " << err << std::endl;
				break;
			}
			catch (std::exception exp) {
				std::cout << __FUNCTION__ << " -> " << exp.what() << std::endl;
			}
		}
	}

	DispatchInfo * _Public CPU::dispatch(Register<> &pc, Memory &code) {
		BYTE *addr = (BYTE *)pc.get();
		if (addr > (BYTE *)code.get() + code.getSize()) 
			throw "Segmentation Fault";

		/*
		* opcode sequences
		* opcodes  reg, imm, reg_ptr, imm_ptr
		* [][    ] [                                          ]
		* 00000000 00000000 00000000 00000000 00000000 00000000
		*/

		BYTE opcode = (addr[0] & 0b00111111);
		BYTE opercnt = (addr[0] & 0b11000000) >> 6;

		DWORD opsize, opsize2;
		/*
		switch (optype) {
		case OP_TYPE_IMM:
			opsize = 4;
			break;
		case OP_TYPE_REG:
			opsize = 1;
			break;
		case OP_TYPE_REG2:
			opsize = 2;
			break;
		case OP_TYPE_IMM2:
			opsize = 5;
			break;

		default:
			return nullptr;
		}
		*/

		DispatchInfo *dpinfo = new DispatchInfo;
		dpinfo->opcode = opcode;
		//dpinfo->opcode_type = optype;
		//dpinfo->opcode_size = opsize + 1;

		// for register
		// [ type 2bit ][ size 2 bit ][ reg idx 4bit ]
		// for imm
		// [ type 2bit ][ size 2 bit ][  imm (size)byte   ]
		// registers
		// eax ax ah al
		// imm
		// 4 2 1
		BYTE optype = (addr[1] & 0b11000000) >> 6;
		BYTE opertype = (addr[1] & 0b00110000) >> 4;
		BYTE reg = (addr[1] & 0b00001111);
		switch(optype){
		case OP_TYPE_REG:
			opsize = 1;
			dpinfo->operand[0] = new Operand(optype, &reg, 1, opertype);
			break;
		case OP_TYPE_IMM:
			opsize = 4;
			dpinfo->operand[0] = new Operand(optype, &addr[2], 4, opertype);
			break;
		case OP_TYPE_PTR_REG:
			opsize = 1;
			dpinfo->operand[0] = new Operand(OP_TYPE_REG, &reg, 1, opertype, true);
			break;
		case OP_TYPE_PTR_IMM:
			opsize = 4;
			dpinfo->operand[0] = new Operand(OP_TYPE_IMM, &addr[2], 4, opertype, true);
			break;
		}

		if (opercnt == 0b10) {
			optype = addr[1 + opsize];

			BYTE sOptype = optype;
			optype = (sOptype & 0b11000000) >> 6;
			opertype = (sOptype & 0b00110000) >> 4;
			reg = (sOptype & 0b00001111);
			switch (optype) {
			case OP_TYPE_REG: 
				dpinfo->operand[1] = new Operand(optype, &reg, 1, opertype);
				opsize += 1;
				break;
			case OP_TYPE_IMM:
				dpinfo->operand[1] = new Operand(optype, &addr[2 + opsize], 4, opertype);
				opsize += 4;
				break;
			case OP_TYPE_PTR_REG:
				dpinfo->operand[1] = new Operand(OP_TYPE_REG, &reg, 1, opertype, true);
				opsize += 1;
				break;
			case OP_TYPE_PTR_IMM:
				dpinfo->operand[1] = new Operand(OP_TYPE_IMM, &addr[1 + opsize], 4, opertype, true);
				opsize += 4;
				break;
			}
		}
		
		dpinfo->opcodes = new BYTE[opsize + 1];
		std::memcpy(dpinfo->opcodes, &addr[1], opsize + 1);

		return dpinfo;
	}
}