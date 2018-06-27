#include "CPU.h"

#include <iostream>
#include <string>

namespace KNVM {
	void CPU::execute(Memory &code) {
		while (true) {
			try {
				DispatchInfo *dpinfo = dispatch(reg["eip"], code);

				if (dpinfo == nullptr)
					throw "DispatchInfo == nullptr";
				
				reg["eip"] += dpinfo->opcode_size;
				
				handler.handle(dpinfo);

				delete dpinfo;
			}
			catch (const char *err) {
				std::cout << __FUNCTION__ << " -> " << err << std::endl;
				break;
			}
		}
	}

	DispatchInfo *CPU::dispatch(Register<> &pc, Memory &code) {
		BYTE *addr = (BYTE *)code.get();
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
		case OP_TYPE_PTR:
			opsize = sizeof(void*);
			break;
		case OP_TYPE_EXCEPT:
			opsize = 4;
		default:
			return nullptr;
		}
		
		DispatchInfo *dpinfo = new DispatchInfo;
		dpinfo->opcode = opcode;
		dpinfo->opcode_type = optype;
		dpinfo->opcode_size = opsize;
		dpinfo->opcodes = new BYTE[opsize];
		std::memcpy(dpinfo->opcodes, &addr[1], opsize);

		return dpinfo;
	}
}