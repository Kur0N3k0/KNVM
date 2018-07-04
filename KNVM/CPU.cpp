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
		reg["eip"] = (DWORD)code.get();
		reg["esp"] = (DWORD)stack.get() + stack.getSize() - 4;
		reg["ebp"] = reg["esp"];

		//genTestcase(code);

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

	DispatchInfo * _Public CPU::dispatch(Register<> &pc, Memory &code) {
		BYTE *addr = (BYTE *)pc.get();
		if (addr > (BYTE *)code.get() + code.getSize()) 
			throw "Segmentation Fault";

		BYTE opcode = (addr[0] & 0b00111111);
		BYTE optype = (addr[0] & 0b11000000) >> 6;

		DWORD opsize;
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

		DispatchInfo *dpinfo = new DispatchInfo;
		dpinfo->opcode = opcode;
		dpinfo->opcode_type = optype;
		dpinfo->opcode_size = opsize + 1;
		dpinfo->opcodes = new BYTE[opsize + 1];
		std::memcpy(dpinfo->opcodes, &addr[1], opsize + 1);

		return dpinfo;
	}
}