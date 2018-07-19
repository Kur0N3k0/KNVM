#include "Disassembler.h"

#include <string>
#include <iostream>

namespace KNVM {
	DisassembleInfo Disassembler::mov(DispatchInfo * dpinfo) {
		auto &reg = cpu.getRegister();
		auto &opreg = dpinfo->operand[0];
		auto &opreg2 = dpinfo->operand[1];

		BYTE *bytes = opreg->getBytes();
		DWORD type = opreg->getType();

		BYTE *bytes2 = opreg2->getBytes();
		DWORD type2 = opreg2->getType();

		DWORD typesize = 1;
		DisassembleInfo di = { "mov ", 0 };

		if (type == OP_TYPE_REG) {
			auto &lreg = reg[bytes[0]];

			if (opreg->is_indirect()) {
				if (type2 == OP_TYPE_REG) {
					auto &rreg = reg[bytes2[0]];
					if (opreg2->is_indirect()) {
						di.asmbly += join("[" + lreg.getName() + "]", "[" + rreg.getName() + "]");
					}
					else {
						di.asmbly += join("[" + lreg.getName() + "]", rreg.getName());
					}
				}
				else if (type2 == OP_TYPE_IMM) {
					DWORD ptr = *(DWORD *)opreg2->getBytes();
					if (opreg2->is_indirect()) {
						di.asmbly += join("[" + lreg.getName() + "]", "[" + hex(ptr) + "]");
					}
					else {
						di.asmbly += join("[" + lreg.getName() + "]", hex(ptr));
					}
					typesize++;
				}
			}
			else {
				if (type2 == OP_TYPE_REG) {
					auto &rreg = reg[bytes2[0]];
					if (opreg2->is_indirect()) {
						di.asmbly += join(lreg.getName(), "[" + rreg.getName() + "]");
					}
					else {
						di.asmbly += join(lreg.getName(), rreg.getName());
					}
				}
				else if (type2 == OP_TYPE_IMM) {
					DWORD ptr = *(DWORD *)opreg2->getBytes();
					if (opreg2->is_indirect()) {
						di.asmbly += join(lreg.getName(), "[" + hex(ptr) + "]");
					}
					else {
						di.asmbly += join(lreg.getName(), hex(ptr));
					}
					typesize++;
				}
			}
		}
		else if (type == OP_TYPE_IMM) {
			typesize++;

			if (opreg->is_indirect()) {
				DWORD **lptr = (DWORD **)opreg->getBytes();
				if (type2 == OP_TYPE_REG) {
					auto &rreg = reg[bytes2[0]];
					if (opreg2->is_indirect()) {
						di.asmbly += join("[" + hex((DWORD)*lptr) + "]", "[" + rreg.getName() + "]");
					}
					else {
						di.asmbly += join("[" + hex((DWORD)*lptr) + "]", rreg.getName());
						**lptr = rreg.get();
					}
				}
				else if (type2 == OP_TYPE_IMM) {
					DWORD ptr = *(DWORD *)opreg2->getBytes();
					if (opreg2->is_indirect()) {
						di.asmbly += join("[" + hex((DWORD)*lptr) + "]", "[" + hex(ptr) + "]");
					}
					else {
						di.asmbly += join("[" + hex((DWORD)*lptr) + "]", hex(ptr));
					}
					typesize++;
				}
			}
			else {
				throw "Unknown Operand Type";
			}
		}

		di.handlesize = typesize + opreg->getSize() + opreg2->getSize();

		return di;
	}
	DisassembleInfo Disassembler::push(DispatchInfo * dpinfo) {
		auto &reg = cpu.getRegister();
		auto &opreg = dpinfo->operand[0];

		BYTE *bytes = opreg->getBytes();
		DWORD type = opreg->getType();

		DWORD typesize = 1;
		DisassembleInfo di = { "push ", 0 };

		if (type == OP_TYPE_REG) {
			auto &lreg = reg[bytes[0]];

			if (opreg->is_indirect()) {
				di.asmbly += join("[" + lreg.getName() + "]");
			}
			else {
				di.asmbly += join(lreg.getName());
			}
		}
		else if (type == OP_TYPE_IMM) {
			if (opreg->is_indirect()) {
				di.asmbly += join("[" + hex(*(DWORD *)bytes) + "]");
			}
			else {
				di.asmbly += join(hex(*(DWORD *)bytes));
				typesize++;
			}
		}

		di.handlesize = typesize + opreg->getSize();
		return di;
	}
	DisassembleInfo Disassembler::pop(DispatchInfo * dpinfo) {
		auto &reg = cpu.getRegister();
		auto &opreg = dpinfo->operand[0];

		BYTE *bytes = opreg->getBytes();
		DWORD type = opreg->getType();

		DWORD typesize = 1;
		DisassembleInfo di = { "pop ", 0 };

		if (type == OP_TYPE_REG) {
			auto &lreg = reg[bytes[0]];

			if (opreg->is_indirect()) {
				throw "Unknown Operand Type";
			}
			else {
				di.asmbly += join(lreg.getName());
			}
		}
		else if (type == OP_TYPE_IMM) {
			throw "Unknown Operand Type";
		}

		di.handlesize = typesize + opreg->getSize();
		return di;
	}
	DisassembleInfo Disassembler::ret(DispatchInfo * dpinfo) {
		auto &reg = cpu.getRegister();
		DisassembleInfo di = { "ret\n", 0 };
		return di;
	}
	DisassembleInfo Disassembler::add(DispatchInfo * dpinfo) {
		auto &reg = cpu.getRegister();
		auto &opreg = dpinfo->operand[0];
		auto &opreg2 = dpinfo->operand[1];

		BYTE *bytes = opreg->getBytes();
		DWORD type = opreg->getType();

		BYTE *bytes2 = opreg2->getBytes();
		DWORD type2 = opreg2->getType();

		DWORD typesize = 1;
		DisassembleInfo di = { "add ", 0 };

		if (type == OP_TYPE_REG) {
			auto &lreg = reg[bytes[0]];

			if (opreg->is_indirect()) {
				if (type2 == OP_TYPE_REG) {
					auto &rreg = reg[bytes2[0]];
					if (opreg2->is_indirect()) {
						di.asmbly += join("[" + lreg.getName() + "]", "[" + rreg.getName() + "]");
					}
					else {
						di.asmbly += join("[" + lreg.getName() + "]", rreg.getName());
					}
				}
				else if (type2 == OP_TYPE_IMM) {
					DWORD ptr = *(DWORD *)opreg2->getBytes();
					if (opreg2->is_indirect()) {
						di.asmbly += join("[" + lreg.getName() + "]", "[" + hex(ptr) + "]");
					}
					else {
						di.asmbly += join("[" + lreg.getName() + "]", hex(ptr));
					}
					typesize++;
				}
			}
			else {
				if (type2 == OP_TYPE_REG) {
					auto &rreg = reg[bytes2[0]];
					if (opreg2->is_indirect()) {
						di.asmbly += join(lreg.getName(), "[" + rreg.getName() + "]");
					}
					else {
						di.asmbly += join(lreg.getName(), rreg.getName());
					}
				}
				else if (type2 == OP_TYPE_IMM) {
					DWORD ptr = *(DWORD *)opreg2->getBytes();
					if (opreg2->is_indirect()) {
						di.asmbly += join(lreg.getName(), "[" + hex(ptr) + "]");
					}
					else {
						di.asmbly += join(lreg.getName(), hex(ptr));
					}
					typesize++;
				}
			}
		}
		else if (type == OP_TYPE_IMM) {
			typesize++;

			if (opreg->is_indirect()) {
				DWORD **lptr = (DWORD **)opreg->getBytes();
				if (type2 == OP_TYPE_REG) {
					auto &rreg = reg[bytes2[0]];
					if (opreg2->is_indirect()) {
						di.asmbly += join("[" + hex((DWORD)*lptr) + "]", "[" + rreg.getName() + "]");
					}
					else {
						di.asmbly += join("[" + hex((DWORD)*lptr) + "]", rreg.getName());
						**lptr = rreg.get();
					}
				}
				else if (type2 == OP_TYPE_IMM) {
					DWORD ptr = *(DWORD *)opreg2->getBytes();
					if (opreg2->is_indirect()) {
						di.asmbly += join("[" + hex((DWORD)*lptr) + "]", "[" + hex(ptr) + "]");
					}
					else {
						di.asmbly += join("[" + hex((DWORD)*lptr) + "]", hex(ptr));
					}
					typesize++;
				}
			}
			else {
				throw "Unknown Operand Type";
			}
		}

		di.handlesize = typesize + opreg->getSize() + opreg2->getSize();

		return di;
	}
	DisassembleInfo Disassembler::sub(DispatchInfo * dpinfo) {
		auto &reg = cpu.getRegister();
		auto &opreg = dpinfo->operand[0];
		auto &opreg2 = dpinfo->operand[1];

		BYTE *bytes = opreg->getBytes();
		DWORD type = opreg->getType();

		BYTE *bytes2 = opreg2->getBytes();
		DWORD type2 = opreg2->getType();

		DWORD typesize = 1;
		DisassembleInfo di = { "sub ", 0 };

		if (type == OP_TYPE_REG) {
			auto &lreg = reg[bytes[0]];

			if (opreg->is_indirect()) {
				if (type2 == OP_TYPE_REG) {
					auto &rreg = reg[bytes2[0]];
					if (opreg2->is_indirect()) {
						di.asmbly += join("[" + lreg.getName() + "]", "[" + rreg.getName() + "]");
					}
					else {
						di.asmbly += join("[" + lreg.getName() + "]", rreg.getName());
					}
				}
				else if (type2 == OP_TYPE_IMM) {
					DWORD ptr = *(DWORD *)opreg2->getBytes();
					if (opreg2->is_indirect()) {
						di.asmbly += join("[" + lreg.getName() + "]", "[" + hex(ptr) + "]");
					}
					else {
						di.asmbly += join("[" + lreg.getName() + "]", hex(ptr));
					}
					typesize++;
				}
			}
			else {
				if (type2 == OP_TYPE_REG) {
					auto &rreg = reg[bytes2[0]];
					if (opreg2->is_indirect()) {
						di.asmbly += join(lreg.getName(), "[" + rreg.getName() + "]");
					}
					else {
						di.asmbly += join(lreg.getName(), rreg.getName());
					}
				}
				else if (type2 == OP_TYPE_IMM) {
					DWORD ptr = *(DWORD *)opreg2->getBytes();
					if (opreg2->is_indirect()) {
						di.asmbly += join(lreg.getName(), "[" + hex(ptr) + "]");
					}
					else {
						di.asmbly += join(lreg.getName(), hex(ptr));
					}
					typesize++;
				}
			}
		}
		else if (type == OP_TYPE_IMM) {
			typesize++;

			if (opreg->is_indirect()) {
				DWORD **lptr = (DWORD **)opreg->getBytes();
				if (type2 == OP_TYPE_REG) {
					auto &rreg = reg[bytes2[0]];
					if (opreg2->is_indirect()) {
						di.asmbly += join("[" + hex((DWORD)*lptr) + "]", "[" + rreg.getName() + "]");
					}
					else {
						di.asmbly += join("[" + hex((DWORD)*lptr) + "]", rreg.getName());
						**lptr = rreg.get();
					}
				}
				else if (type2 == OP_TYPE_IMM) {
					DWORD ptr = *(DWORD *)opreg2->getBytes();
					if (opreg2->is_indirect()) {
						di.asmbly += join("[" + hex((DWORD)*lptr) + "]", "[" + hex(ptr) + "]");
					}
					else {
						di.asmbly += join("[" + hex((DWORD)*lptr) + "]", hex(ptr));
					}
					typesize++;
				}
			}
			else {
				throw "Unknown Operand Type";
			}
		}

		di.handlesize = typesize + opreg->getSize() + opreg2->getSize();

		return di;
	}
	DisassembleInfo Disassembler::mul(DispatchInfo * dpinfo) {
		auto &reg = cpu.getRegister();
		auto &opreg = dpinfo->operand[0];
		auto &opreg2 = dpinfo->operand[1];

		BYTE *bytes = opreg->getBytes();
		DWORD type = opreg->getType();

		BYTE *bytes2 = opreg2->getBytes();
		DWORD type2 = opreg2->getType();

		DWORD typesize = 1;
		DisassembleInfo di = { "mul ", 0 };

		if (type == OP_TYPE_REG) {
			auto &lreg = reg[bytes[0]];

			if (opreg->is_indirect()) {
				if (type2 == OP_TYPE_REG) {
					auto &rreg = reg[bytes2[0]];
					if (opreg2->is_indirect()) {
						di.asmbly += join("[" + lreg.getName() + "]", "[" + rreg.getName() + "]");
					}
					else {
						di.asmbly += join("[" + lreg.getName() + "]", rreg.getName());
					}
				}
				else if (type2 == OP_TYPE_IMM) {
					DWORD ptr = *(DWORD *)opreg2->getBytes();
					if (opreg2->is_indirect()) {
						di.asmbly += join("[" + lreg.getName() + "]", "[" + hex(ptr) + "]");
					}
					else {
						di.asmbly += join("[" + lreg.getName() + "]", hex(ptr));
					}
					typesize++;
				}
			}
			else {
				if (type2 == OP_TYPE_REG) {
					auto &rreg = reg[bytes2[0]];
					if (opreg2->is_indirect()) {
						di.asmbly += join(lreg.getName(), "[" + rreg.getName() + "]");
					}
					else {
						di.asmbly += join(lreg.getName(), rreg.getName());
					}
				}
				else if (type2 == OP_TYPE_IMM) {
					DWORD ptr = *(DWORD *)opreg2->getBytes();
					if (opreg2->is_indirect()) {
						di.asmbly += join(lreg.getName(), "[" + hex(ptr) + "]");
					}
					else {
						di.asmbly += join(lreg.getName(), hex(ptr));
					}
					typesize++;
				}
			}
		}
		else if (type == OP_TYPE_IMM) {
			typesize++;

			if (opreg->is_indirect()) {
				DWORD **lptr = (DWORD **)opreg->getBytes();
				if (type2 == OP_TYPE_REG) {
					auto &rreg = reg[bytes2[0]];
					if (opreg2->is_indirect()) {
						di.asmbly += join("[" + hex((DWORD)*lptr) + "]", "[" + rreg.getName() + "]");
					}
					else {
						di.asmbly += join("[" + hex((DWORD)*lptr) + "]", rreg.getName());
						**lptr = rreg.get();
					}
				}
				else if (type2 == OP_TYPE_IMM) {
					DWORD ptr = *(DWORD *)opreg2->getBytes();
					if (opreg2->is_indirect()) {
						di.asmbly += join("[" + hex((DWORD)*lptr) + "]", "[" + hex(ptr) + "]");
					}
					else {
						di.asmbly += join("[" + hex((DWORD)*lptr) + "]", hex(ptr));
					}
					typesize++;
				}
			}
			else {
				throw "Unknown Operand Type";
			}
		}

		di.handlesize = typesize + opreg->getSize() + opreg2->getSize();

		return di;
	}
	DisassembleInfo Disassembler::div(DispatchInfo * dpinfo) {
		auto &reg = cpu.getRegister();
		auto &opreg = dpinfo->operand[0];
		auto &opreg2 = dpinfo->operand[1];

		BYTE *bytes = opreg->getBytes();
		DWORD type = opreg->getType();

		BYTE *bytes2 = opreg2->getBytes();
		DWORD type2 = opreg2->getType();

		DWORD typesize = 1;
		DisassembleInfo di = { "div ", 0 };

		if (type == OP_TYPE_REG) {
			auto &lreg = reg[bytes[0]];

			if (opreg->is_indirect()) {
				if (type2 == OP_TYPE_REG) {
					auto &rreg = reg[bytes2[0]];
					if (opreg2->is_indirect()) {
						di.asmbly += join("[" + lreg.getName() + "]", "[" + rreg.getName() + "]");
					}
					else {
						di.asmbly += join("[" + lreg.getName() + "]", rreg.getName());
					}
				}
				else if (type2 == OP_TYPE_IMM) {
					DWORD ptr = *(DWORD *)opreg2->getBytes();
					if (opreg2->is_indirect()) {
						di.asmbly += join("[" + lreg.getName() + "]", "[" + hex(ptr) + "]");
					}
					else {
						di.asmbly += join("[" + lreg.getName() + "]", hex(ptr));
					}
					typesize++;
				}
			}
			else {
				if (type2 == OP_TYPE_REG) {
					auto &rreg = reg[bytes2[0]];
					if (opreg2->is_indirect()) {
						di.asmbly += join(lreg.getName(), "[" + rreg.getName() + "]");
					}
					else {
						di.asmbly += join(lreg.getName(), rreg.getName());
					}
				}
				else if (type2 == OP_TYPE_IMM) {
					DWORD ptr = *(DWORD *)opreg2->getBytes();
					if (opreg2->is_indirect()) {
						di.asmbly += join(lreg.getName(), "[" + hex(ptr) + "]");
					}
					else {
						di.asmbly += join(lreg.getName(), hex(ptr));
					}
					typesize++;
				}
			}
		}
		else if (type == OP_TYPE_IMM) {
			typesize++;

			if (opreg->is_indirect()) {
				DWORD **lptr = (DWORD **)opreg->getBytes();
				if (type2 == OP_TYPE_REG) {
					auto &rreg = reg[bytes2[0]];
					if (opreg2->is_indirect()) {
						di.asmbly += join("[" + hex((DWORD)*lptr) + "]", "[" + rreg.getName() + "]");
					}
					else {
						di.asmbly += join("[" + hex((DWORD)*lptr) + "]", rreg.getName());
						**lptr = rreg.get();
					}
				}
				else if (type2 == OP_TYPE_IMM) {
					DWORD ptr = *(DWORD *)opreg2->getBytes();
					if (opreg2->is_indirect()) {
						di.asmbly += join("[" + hex((DWORD)*lptr) + "]", "[" + hex(ptr) + "]");
					}
					else {
						di.asmbly += join("[" + hex((DWORD)*lptr) + "]", hex(ptr));
					}
					typesize++;
				}
			}
			else {
				throw "Unknown Operand Type";
			}
		}

		di.handlesize = typesize + opreg->getSize() + opreg2->getSize();

		return di;
	}

	DisassembleInfo Disassembler::and(DispatchInfo * dpinfo) {
		auto &reg = cpu.getRegister();
		auto &opreg = dpinfo->operand[0];
		auto &opreg2 = dpinfo->operand[1];

		BYTE *bytes = opreg->getBytes();
		DWORD type = opreg->getType();

		BYTE *bytes2 = opreg2->getBytes();
		DWORD type2 = opreg2->getType();

		DWORD typesize = 1;
		DisassembleInfo di = { "and ", 0 };

		if (type == OP_TYPE_REG) {
			auto &lreg = reg[bytes[0]];

			if (opreg->is_indirect()) {
				if (type2 == OP_TYPE_REG) {
					auto &rreg = reg[bytes2[0]];
					if (opreg2->is_indirect()) {
						di.asmbly += join("[" + lreg.getName() + "]", "[" + rreg.getName() + "]");
					}
					else {
						di.asmbly += join("[" + lreg.getName() + "]", rreg.getName());
					}
				}
				else if (type2 == OP_TYPE_IMM) {
					DWORD ptr = *(DWORD *)opreg2->getBytes();
					if (opreg2->is_indirect()) {
						di.asmbly += join("[" + lreg.getName() + "]", "[" + hex(ptr) + "]");
					}
					else {
						di.asmbly += join("[" + lreg.getName() + "]", hex(ptr));
					}
					typesize++;
				}
			}
			else {
				if (type2 == OP_TYPE_REG) {
					auto &rreg = reg[bytes2[0]];
					if (opreg2->is_indirect()) {
						di.asmbly += join(lreg.getName(), "[" + rreg.getName() + "]");
					}
					else {
						di.asmbly += join(lreg.getName(), rreg.getName());
					}
				}
				else if (type2 == OP_TYPE_IMM) {
					DWORD ptr = *(DWORD *)opreg2->getBytes();
					if (opreg2->is_indirect()) {
						di.asmbly += join(lreg.getName(), "[" + hex(ptr) + "]");
					}
					else {
						di.asmbly += join(lreg.getName(), hex(ptr));
					}
					typesize++;
				}
			}
		}
		else if (type == OP_TYPE_IMM) {
			typesize++;

			if (opreg->is_indirect()) {
				DWORD **lptr = (DWORD **)opreg->getBytes();
				if (type2 == OP_TYPE_REG) {
					auto &rreg = reg[bytes2[0]];
					if (opreg2->is_indirect()) {
						di.asmbly += join("[" + hex((DWORD)*lptr) + "]", "[" + rreg.getName() + "]");
					}
					else {
						di.asmbly += join("[" + hex((DWORD)*lptr) + "]", rreg.getName());
						**lptr = rreg.get();
					}
				}
				else if (type2 == OP_TYPE_IMM) {
					DWORD ptr = *(DWORD *)opreg2->getBytes();
					if (opreg2->is_indirect()) {
						di.asmbly += join("[" + hex((DWORD)*lptr) + "]", "[" + hex(ptr) + "]");
					}
					else {
						di.asmbly += join("[" + hex((DWORD)*lptr) + "]", hex(ptr));
					}
					typesize++;
				}
			}
			else {
				throw "Unknown Operand Type";
			}
		}

		di.handlesize = typesize + opreg->getSize() + opreg2->getSize();

		return di;
	}
	DisassembleInfo Disassembler::or(DispatchInfo * dpinfo) {
		auto &reg = cpu.getRegister();
		auto &opreg = dpinfo->operand[0];
		auto &opreg2 = dpinfo->operand[1];

		BYTE *bytes = opreg->getBytes();
		DWORD type = opreg->getType();

		BYTE *bytes2 = opreg2->getBytes();
		DWORD type2 = opreg2->getType();

		DWORD typesize = 1;
		DisassembleInfo di = { "or ", 0 };

		if (type == OP_TYPE_REG) {
			auto &lreg = reg[bytes[0]];

			if (opreg->is_indirect()) {
				if (type2 == OP_TYPE_REG) {
					auto &rreg = reg[bytes2[0]];
					if (opreg2->is_indirect()) {
						di.asmbly += join("[" + lreg.getName() + "]", "[" + rreg.getName() + "]");
					}
					else {
						di.asmbly += join("[" + lreg.getName() + "]", rreg.getName());
					}
				}
				else if (type2 == OP_TYPE_IMM) {
					DWORD ptr = *(DWORD *)opreg2->getBytes();
					if (opreg2->is_indirect()) {
						di.asmbly += join("[" + lreg.getName() + "]", "[" + hex(ptr) + "]");
					}
					else {
						di.asmbly += join("[" + lreg.getName() + "]", hex(ptr));
					}
					typesize++;
				}
			}
			else {
				if (type2 == OP_TYPE_REG) {
					auto &rreg = reg[bytes2[0]];
					if (opreg2->is_indirect()) {
						di.asmbly += join(lreg.getName(), "[" + rreg.getName() + "]");
					}
					else {
						di.asmbly += join(lreg.getName(), rreg.getName());
					}
				}
				else if (type2 == OP_TYPE_IMM) {
					DWORD ptr = *(DWORD *)opreg2->getBytes();
					if (opreg2->is_indirect()) {
						di.asmbly += join(lreg.getName(), "[" + hex(ptr) + "]");
					}
					else {
						di.asmbly += join(lreg.getName(), hex(ptr));
					}
					typesize++;
				}
			}
		}
		else if (type == OP_TYPE_IMM) {
			typesize++;

			if (opreg->is_indirect()) {
				DWORD **lptr = (DWORD **)opreg->getBytes();
				if (type2 == OP_TYPE_REG) {
					auto &rreg = reg[bytes2[0]];
					if (opreg2->is_indirect()) {
						di.asmbly += join("[" + hex((DWORD)*lptr) + "]", "[" + rreg.getName() + "]");
					}
					else {
						di.asmbly += join("[" + hex((DWORD)*lptr) + "]", rreg.getName());
						**lptr = rreg.get();
					}
				}
				else if (type2 == OP_TYPE_IMM) {
					DWORD ptr = *(DWORD *)opreg2->getBytes();
					if (opreg2->is_indirect()) {
						di.asmbly += join("[" + hex((DWORD)*lptr) + "]", "[" + hex(ptr) + "]");
					}
					else {
						di.asmbly += join("[" + hex((DWORD)*lptr) + "]", hex(ptr));
					}
					typesize++;
				}
			}
			else {
				throw "Unknown Operand Type";
			}
		}

		di.handlesize = typesize + opreg->getSize() + opreg2->getSize();

		return di;
	}
	DisassembleInfo Disassembler::xor(DispatchInfo * dpinfo) {
		auto &reg = cpu.getRegister();
		auto &opreg = dpinfo->operand[0];
		auto &opreg2 = dpinfo->operand[1];

		BYTE *bytes = opreg->getBytes();
		DWORD type = opreg->getType();

		BYTE *bytes2 = opreg2->getBytes();
		DWORD type2 = opreg2->getType();

		DWORD typesize = 1;
		DisassembleInfo di = { "xor ", 0 };

		if (type == OP_TYPE_REG) {
			auto &lreg = reg[bytes[0]];

			if (opreg->is_indirect()) {
				if (type2 == OP_TYPE_REG) {
					auto &rreg = reg[bytes2[0]];
					if (opreg2->is_indirect()) {
						di.asmbly += join("[" + lreg.getName() + "]", "[" + rreg.getName() + "]");
					}
					else {
						di.asmbly += join("[" + lreg.getName() + "]", rreg.getName());
					}
				}
				else if (type2 == OP_TYPE_IMM) {
					DWORD ptr = *(DWORD *)opreg2->getBytes();
					if (opreg2->is_indirect()) {
						di.asmbly += join("[" + lreg.getName() + "]", "[" + hex(ptr) + "]");
					}
					else {
						di.asmbly += join("[" + lreg.getName() + "]", hex(ptr));
					}
					typesize++;
				}
			}
			else {
				if (type2 == OP_TYPE_REG) {
					auto &rreg = reg[bytes2[0]];
					if (opreg2->is_indirect()) {
						di.asmbly += join(lreg.getName(), "[" + rreg.getName() + "]");
					}
					else {
						di.asmbly += join(lreg.getName(), rreg.getName());
					}
				}
				else if (type2 == OP_TYPE_IMM) {
					DWORD ptr = *(DWORD *)opreg2->getBytes();
					if (opreg2->is_indirect()) {
						di.asmbly += join(lreg.getName(), "[" + hex(ptr) + "]");
					}
					else {
						di.asmbly += join(lreg.getName(), hex(ptr));
					}
					typesize++;
				}
			}
		}
		else if (type == OP_TYPE_IMM) {
			typesize++;

			if (opreg->is_indirect()) {
				DWORD **lptr = (DWORD **)opreg->getBytes();
				if (type2 == OP_TYPE_REG) {
					auto &rreg = reg[bytes2[0]];
					if (opreg2->is_indirect()) {
						di.asmbly += join("[" + hex((DWORD)*lptr) + "]", "[" + rreg.getName() + "]");
					}
					else {
						di.asmbly += join("[" + hex((DWORD)*lptr) + "]", rreg.getName());
						**lptr = rreg.get();
					}
				}
				else if (type2 == OP_TYPE_IMM) {
					DWORD ptr = *(DWORD *)opreg2->getBytes();
					if (opreg2->is_indirect()) {
						di.asmbly += join("[" + hex((DWORD)*lptr) + "]", "[" + hex(ptr) + "]");
					}
					else {
						di.asmbly += join("[" + hex((DWORD)*lptr) + "]", hex(ptr));
					}
					typesize++;
				}
			}
			else {
				throw "Unknown Operand Type";
			}
		}

		di.handlesize = typesize + opreg->getSize() + opreg2->getSize();

		return di;
	}
	DisassembleInfo Disassembler::jmp(DispatchInfo * dpinfo) {
		auto &reg = cpu.getRegister();
		auto &opreg = dpinfo->operand[0];

		BYTE *bytes = opreg->getBytes();
		DWORD type = opreg->getType();

		DWORD typesize = 1;
		DisassembleInfo di = { "jmp ", 0 };

		if (type == OP_TYPE_REG) {
			auto &lreg = reg[bytes[0]];

			if (opreg->is_indirect()) {
				di.asmbly += join("[" + lreg.getName() + "]");
			}
			else {
				di.asmbly += join(lreg.getName());
			}
		}
		else if (type == OP_TYPE_IMM) {
			if (opreg->is_indirect()) {
				di.asmbly += join("[" + hex(*(DWORD *)bytes) + "]");
			}
			else {
				di.asmbly += join(hex(*(DWORD *)bytes));
				typesize++;
			}
		}

		di.handlesize = typesize + opreg->getSize();
		return di;
	}
	DisassembleInfo Disassembler::je(DispatchInfo * dpinfo) {
		auto &reg = cpu.getRegister();
		auto &opreg = dpinfo->operand[0];

		BYTE *bytes = opreg->getBytes();
		DWORD type = opreg->getType();

		DWORD typesize = 1;
		DisassembleInfo di = { "je ", 0 };

		if (type == OP_TYPE_REG) {
			auto &lreg = reg[bytes[0]];

			if (opreg->is_indirect()) {
				di.asmbly += join("[" + lreg.getName() + "]");
			}
			else {
				di.asmbly += join(lreg.getName());
			}
		}
		else if (type == OP_TYPE_IMM) {
			if (opreg->is_indirect()) {
				di.asmbly += join("[" + hex(*(DWORD *)bytes) + "]");
			}
			else {
				di.asmbly += join(hex(*(DWORD *)bytes));
				typesize++;
			}
		}

		di.handlesize = typesize + opreg->getSize();
		return di;
	}
	DisassembleInfo Disassembler::jne(DispatchInfo * dpinfo) {
		auto &reg = cpu.getRegister();
		auto &opreg = dpinfo->operand[0];

		BYTE *bytes = opreg->getBytes();
		DWORD type = opreg->getType();

		DWORD typesize = 1;
		DisassembleInfo di = { "jne ", 0 };

		if (type == OP_TYPE_REG) {
			auto &lreg = reg[bytes[0]];

			if (opreg->is_indirect()) {
				di.asmbly += join("[" + lreg.getName() + "]");
			}
			else {
				di.asmbly += join(lreg.getName());
			}
		}
		else if (type == OP_TYPE_IMM) {
			if (opreg->is_indirect()) {
				di.asmbly += join("[" + hex(*(DWORD *)bytes) + "]");
			}
			else {
				di.asmbly += join(hex(*(DWORD *)bytes));
				typesize++;
			}
		}

		di.handlesize = typesize + opreg->getSize();
		return di;
	}
	DisassembleInfo Disassembler::ja(DispatchInfo * dpinfo) {
		auto &reg = cpu.getRegister();
		auto &opreg = dpinfo->operand[0];

		BYTE *bytes = opreg->getBytes();
		DWORD type = opreg->getType();

		DWORD typesize = 1;
		DisassembleInfo di = { "ja ", 0 };

		if (type == OP_TYPE_REG) {
			auto &lreg = reg[bytes[0]];

			if (opreg->is_indirect()) {
				di.asmbly += join("[" + lreg.getName() + "]");
			}
			else {
				di.asmbly += join(lreg.getName());
			}
		}
		else if (type == OP_TYPE_IMM) {
			if (opreg->is_indirect()) {
				di.asmbly += join("[" + hex(*(DWORD *)bytes) + "]");
			}
			else {
				di.asmbly += join(hex(*(DWORD *)bytes));
				typesize++;
			}
		}

		di.handlesize = typesize + opreg->getSize();
		return di;
	}
	DisassembleInfo Disassembler::jb(DispatchInfo * dpinfo) {
		auto &reg = cpu.getRegister();
		auto &opreg = dpinfo->operand[0];

		BYTE *bytes = opreg->getBytes();
		DWORD type = opreg->getType();

		DWORD typesize = 1;
		DisassembleInfo di = { "jb ", 0 };

		if (type == OP_TYPE_REG) {
			auto &lreg = reg[bytes[0]];

			if (opreg->is_indirect()) {
				di.asmbly += join("[" + lreg.getName() + "]");
			}
			else {
				di.asmbly += join(lreg.getName());
			}
		}
		else if (type == OP_TYPE_IMM) {
			if (opreg->is_indirect()) {
				di.asmbly += join("[" + hex(*(DWORD *)bytes) + "]");
			}
			else {
				di.asmbly += join(hex(*(DWORD *)bytes));
				typesize++;
			}
		}

		di.handlesize = typesize + opreg->getSize();
		return di;
	}
	DisassembleInfo Disassembler::jl(DispatchInfo * dpinfo) {
		auto &reg = cpu.getRegister();
		auto &opreg = dpinfo->operand[0];

		BYTE *bytes = opreg->getBytes();
		DWORD type = opreg->getType();

		DWORD typesize = 1;
		DisassembleInfo di = { "push ", 0 };

		if (type == OP_TYPE_REG) {
			auto &lreg = reg[bytes[0]];

			if (opreg->is_indirect()) {
				di.asmbly += join("[" + lreg.getName() + "]");
			}
			else {
				di.asmbly += join(lreg.getName());
			}
		}
		else if (type == OP_TYPE_IMM) {
			if (opreg->is_indirect()) {
				di.asmbly += join("[" + hex(*(DWORD *)bytes) + "]");
			}
			else {
				di.asmbly += join(hex(*(DWORD *)bytes));
				typesize++;
			}
		}

		di.handlesize = typesize + opreg->getSize();
		return di;
	}
	DisassembleInfo Disassembler::jle(DispatchInfo * dpinfo) {
		auto &reg = cpu.getRegister();
		auto &opreg = dpinfo->operand[0];

		BYTE *bytes = opreg->getBytes();
		DWORD type = opreg->getType();

		DWORD typesize = 1;
		DisassembleInfo di = { "jle ", 0 };

		if (type == OP_TYPE_REG) {
			auto &lreg = reg[bytes[0]];

			if (opreg->is_indirect()) {
				di.asmbly += join("[" + lreg.getName() + "]");
			}
			else {
				di.asmbly += join(lreg.getName());
			}
		}
		else if (type == OP_TYPE_IMM) {
			if (opreg->is_indirect()) {
				di.asmbly += join("[" + hex(*(DWORD *)bytes) + "]");
			}
			else {
				di.asmbly += join(hex(*(DWORD *)bytes));
				typesize++;
			}
		}

		di.handlesize = typesize + opreg->getSize();
		return di;
	}
	DisassembleInfo Disassembler::jz(DispatchInfo * dpinfo) {
		auto &reg = cpu.getRegister();
		auto &opreg = dpinfo->operand[0];

		BYTE *bytes = opreg->getBytes();
		DWORD type = opreg->getType();

		DWORD typesize = 1;
		DisassembleInfo di = { "jz ", 0 };

		if (type == OP_TYPE_REG) {
			auto &lreg = reg[bytes[0]];

			if (opreg->is_indirect()) {
				di.asmbly += join("[" + lreg.getName() + "]");
			}
			else {
				di.asmbly += join(lreg.getName());
			}
		}
		else if (type == OP_TYPE_IMM) {
			if (opreg->is_indirect()) {
				di.asmbly += join("[" + hex(*(DWORD *)bytes) + "]");
			}
			else {
				di.asmbly += join(hex(*(DWORD *)bytes));
				typesize++;
			}
		}

		di.handlesize = typesize + opreg->getSize();
		return di;
	}

	DisassembleInfo Disassembler::cmp(DispatchInfo *dpinfo) {
		auto &reg = cpu.getRegister();
		auto &opreg = dpinfo->operand[0];
		auto &opreg2 = dpinfo->operand[1];

		BYTE *bytes = opreg->getBytes();
		DWORD type = opreg->getType();

		BYTE *bytes2 = opreg2->getBytes();
		DWORD type2 = opreg2->getType();

		DWORD typesize = 1;
		DisassembleInfo di = { "cmp ", 0 };

		if (type == OP_TYPE_REG) {
			auto &lreg = reg[bytes[0]];

			if (opreg->is_indirect()) {
				if (type2 == OP_TYPE_REG) {
					auto &rreg = reg[bytes2[0]];
					if (opreg2->is_indirect()) {
						di.asmbly += join("[" + lreg.getName() + "]", "[" + rreg.getName() + "]");
					}
					else {
						di.asmbly += join("[" + lreg.getName() + "]", rreg.getName());
					}
				}
				else if (type2 == OP_TYPE_IMM) {
					DWORD ptr = *(DWORD *)opreg2->getBytes();
					if (opreg2->is_indirect()) {
						di.asmbly += join("[" + lreg.getName() + "]", "[" + hex(ptr) + "]");
					}
					else {
						di.asmbly += join("[" + lreg.getName() + "]", hex(ptr));
					}
					typesize++;
				}
			}
			else {
				if (type2 == OP_TYPE_REG) {
					auto &rreg = reg[bytes2[0]];
					if (opreg2->is_indirect()) {
						di.asmbly += join(lreg.getName(), "[" + rreg.getName() + "]");
					}
					else {
						di.asmbly += join(lreg.getName(), rreg.getName());
					}
				}
				else if (type2 == OP_TYPE_IMM) {
					DWORD ptr = *(DWORD *)opreg2->getBytes();
					if (opreg2->is_indirect()) {
						di.asmbly += join(lreg.getName(), "[" + hex(ptr) + "]");
					}
					else {
						di.asmbly += join(lreg.getName(), hex(ptr));
					}
					typesize++;
				}
			}
		}
		else if (type == OP_TYPE_IMM) {
			typesize++;

			if (opreg->is_indirect()) {
				DWORD **lptr = (DWORD **)opreg->getBytes();
				if (type2 == OP_TYPE_REG) {
					auto &rreg = reg[bytes2[0]];
					if (opreg2->is_indirect()) {
						di.asmbly += join("[" + hex((DWORD)*lptr) + "]", "[" + rreg.getName() + "]");
					}
					else {
						di.asmbly += join("[" + hex((DWORD)*lptr) + "]", rreg.getName());
						**lptr = rreg.get();
					}
				}
				else if (type2 == OP_TYPE_IMM) {
					DWORD ptr = *(DWORD *)opreg2->getBytes();
					if (opreg2->is_indirect()) {
						di.asmbly += join("[" + hex((DWORD)*lptr) + "]", "[" + hex(ptr) + "]");
					}
					else {
						di.asmbly += join("[" + hex((DWORD)*lptr) + "]", hex(ptr));
					}
					typesize++;
				}
			}
			else {
				throw "Unknown Operand Type";
			}
		}

		di.handlesize = typesize + opreg->getSize() + opreg2->getSize();

		return di;
	}
	DisassembleInfo Disassembler::test(DispatchInfo *dpinfo) {
		auto &reg = cpu.getRegister();
		auto &opreg = dpinfo->operand[0];
		auto &opreg2 = dpinfo->operand[1];

		BYTE *bytes = opreg->getBytes();
		DWORD type = opreg->getType();

		BYTE *bytes2 = opreg2->getBytes();
		DWORD type2 = opreg2->getType();

		DWORD typesize = 1;
		DisassembleInfo di = { "test ", 0 };

		if (type == OP_TYPE_REG) {
			auto &lreg = reg[bytes[0]];

			if (opreg->is_indirect()) {
				if (type2 == OP_TYPE_REG) {
					auto &rreg = reg[bytes2[0]];
					if (opreg2->is_indirect()) {
						di.asmbly += join("[" + lreg.getName() + "]", "[" + rreg.getName() + "]");
					}
					else {
						di.asmbly += join("[" + lreg.getName() + "]", rreg.getName());
					}
				}
				else if (type2 == OP_TYPE_IMM) {
					DWORD ptr = *(DWORD *)opreg2->getBytes();
					if (opreg2->is_indirect()) {
						di.asmbly += join("[" + lreg.getName() + "]", "[" + hex(ptr) + "]");
					}
					else {
						di.asmbly += join("[" + lreg.getName() + "]", hex(ptr));
					}
					typesize++;
				}
			}
			else {
				if (type2 == OP_TYPE_REG) {
					auto &rreg = reg[bytes2[0]];
					if (opreg2->is_indirect()) {
						di.asmbly += join(lreg.getName(), "[" + rreg.getName() + "]");
					}
					else {
						di.asmbly += join(lreg.getName(), rreg.getName());
					}
				}
				else if (type2 == OP_TYPE_IMM) {
					DWORD ptr = *(DWORD *)opreg2->getBytes();
					if (opreg2->is_indirect()) {
						di.asmbly += join(lreg.getName(), "[" + hex(ptr) + "]");
					}
					else {
						di.asmbly += join(lreg.getName(), hex(ptr));
					}
					typesize++;
				}
			}
		}
		else if (type == OP_TYPE_IMM) {
			typesize++;

			if (opreg->is_indirect()) {
				DWORD **lptr = (DWORD **)opreg->getBytes();
				if (type2 == OP_TYPE_REG) {
					auto &rreg = reg[bytes2[0]];
					if (opreg2->is_indirect()) {
						di.asmbly += join("[" + hex((DWORD)*lptr) + "]", "[" + rreg.getName() + "]");
					}
					else {
						di.asmbly += join("[" + hex((DWORD)*lptr) + "]", rreg.getName());
						**lptr = rreg.get();
					}
				}
				else if (type2 == OP_TYPE_IMM) {
					DWORD ptr = *(DWORD *)opreg2->getBytes();
					if (opreg2->is_indirect()) {
						di.asmbly += join("[" + hex((DWORD)*lptr) + "]", "[" + hex(ptr) + "]");
					}
					else {
						di.asmbly += join("[" + hex((DWORD)*lptr) + "]", hex(ptr));
					}
					typesize++;
				}
			}
			else {
				throw "Unknown Operand Type";
			}
		}

		di.handlesize = typesize + opreg->getSize() + opreg2->getSize();

		return di;
	}

	DisassembleInfo Disassembler::exit(DispatchInfo * dpinfo) {
		DisassembleInfo di = { "ret", 1 };
		return di;
	}
	DisassembleInfo Disassembler::add_except(DispatchInfo * dpinfo) {
		auto &reg = cpu.getRegister();
		auto &opreg = dpinfo->operand[0];

		BYTE *bytes = opreg->getBytes();
		DWORD type = opreg->getType();

		DWORD typesize = 1;
		DisassembleInfo di = { "add.except ", 0 };

		if (type == OP_TYPE_REG) {
			auto &lreg = reg[bytes[0]];

			if (opreg->is_indirect()) {
				di.asmbly += join("[" + lreg.getName() + "]");
			}
			else {
				di.asmbly += join(lreg.getName());
			}
		}
		else if (type == OP_TYPE_IMM) {
			if (opreg->is_indirect()) {
				di.asmbly += join("[" + hex(*(DWORD *)bytes) + "]");
			}
			else {
				di.asmbly += join(hex(*(DWORD *)bytes));
				typesize++;
			}
		}

		di.handlesize = typesize + opreg->getSize();
		return di;
	}
	DisassembleInfo Disassembler::del_except(DispatchInfo * dpinfo) {
		auto &reg = cpu.getRegister();
		auto &opreg = dpinfo->operand[0];

		BYTE *bytes = opreg->getBytes();
		DWORD type = opreg->getType();

		DWORD typesize = 1;
		DisassembleInfo di = { "del.except ", 0 };

		if (type == OP_TYPE_REG) {
			auto &lreg = reg[bytes[0]];

			if (opreg->is_indirect()) {
				di.asmbly += join("[" + lreg.getName() + "]");
			}
			else {
				di.asmbly += join(lreg.getName());
			}
		}
		else if (type == OP_TYPE_IMM) {
			if (opreg->is_indirect()) {
				di.asmbly += join("[" + hex(*(DWORD *)bytes) + "]");
			}
			else {
				di.asmbly += join(hex(*(DWORD *)bytes));
				typesize++;
			}
		}

		di.handlesize = typesize + opreg->getSize();
		return di;
	}
	DisassembleInfo Disassembler::call_except(DispatchInfo * dpinfo) {
		auto &reg = cpu.getRegister();
		auto &opreg = dpinfo->operand[0];

		BYTE *bytes = opreg->getBytes();
		DWORD type = opreg->getType();

		DWORD typesize = 1;
		DisassembleInfo di = { "call.except ", 0 };

		if (type == OP_TYPE_REG) {
			auto &lreg = reg[bytes[0]];

			if (opreg->is_indirect()) {
				di.asmbly += join("[" + lreg.getName() + "]");
			}
			else {
				di.asmbly += join(lreg.getName());
			}
		}
		else if (type == OP_TYPE_IMM) {
			if (opreg->is_indirect()) {
				di.asmbly += join("[" + hex(*(DWORD *)bytes) + "]");
			}
			else {
				di.asmbly += join(hex(*(DWORD *)bytes));
				typesize++;
			}
		}

		di.handlesize = typesize + opreg->getSize();
		return di;
	}

	DisassembleInfo Disassembler::syscall(DispatchInfo *dpinfo) {
		DisassembleInfo di = { "syscall\n", 1 };
		return di;
	}

	std::string Disassembler::disassemble() {
		auto &reg = cpu.getRegister();
		reg["eip"] = (DWORD)code.get();

		std::string result = "";
		while(true){
			try {
				DispatchInfo *dpinfo = cpu.dispatch(reg["eip"], code);
				if (dpinfo == nullptr)
					throw "Dispatchinfo is null";

				if (reg["eip"] > (DWORD)code.get() + code.getCodeSize())
					throw "";
				 
				DisassembleInfo di;

				switch (dpinfo->opcode) {
				case OP_PUSH:
					di = this->push(dpinfo);
					break;
				case OP_POP:
					di = this->pop(dpinfo);
					break;
				case OP_MOV:
					di = this->mov(dpinfo);
					break;
				case OP_RET:
					di = this->ret(dpinfo);
					break;
				case OP_ADD:
					di = this->add(dpinfo);
					break;
				case OP_SUB:
					di = this->sub(dpinfo);
					break;
				case OP_MUL:
					di = this->mul(dpinfo);
					break;
				case OP_DIV:
					di = this->mul(dpinfo);
					break;
				case OP_AND:
					di = this->and(dpinfo);
					break;
				case OP_OR:
					di = this->or(dpinfo);
					break;
				case OP_XOR:
					di = this->xor(dpinfo);
					break;
				case OP_JMP:
					di = this->jmp(dpinfo);
					break;
				case OP_JE:
					di = this->je(dpinfo);
					break;
				case OP_JNE:
					di = this->jne(dpinfo);
					break;
				case OP_JA:
					di = this->ja(dpinfo);
					break;
				case OP_JB:
					di = this->jb(dpinfo);
					break;
				case OP_JL:
					di = this->jl(dpinfo);
					break;
				case OP_JLE:
					di = this->jle(dpinfo);
					break;
				case OP_JZ:
					di = this->jz(dpinfo);
					break;
				case OP_CMP:
					di = this->cmp(dpinfo);
					break;
				case OP_TEST:
					di = this->test(dpinfo);
					break;
				case OP_EXIT:
					di = this->exit(dpinfo);
					break;
				case OP_SYSCALL:
					di = this->syscall(dpinfo);
					break;
				case OP_ADD_EXCEPT:
					di = this->add_except(dpinfo);
					break;
				case OP_DEL_EXCEPT:
					di = this->del_except(dpinfo);
					break;
				case OP_EXCEPT_CALL:
					di = this->call_except(dpinfo);
					break;
				}

				reg["eip"] += di.handlesize;
				result += di.asmbly;

				delete dpinfo;
			}
			catch (const char *err) {
				//std::cout << __FUNCTION__ << " -> " << err << std::endl;
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