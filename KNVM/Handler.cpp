#include "Handler.h"
#include "types.h"
#include "Optable.h"

namespace KNVM {
	DWORD _Private Handler::mov(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack) {
		auto &opreg = dpinfo->operand[0];
		auto &opreg2 = dpinfo->operand[1];

		BYTE *bytes = opreg->getBytes();
		DWORD type = opreg->getType();

		BYTE *bytes2 = opreg2->getBytes();
		DWORD type2 = opreg2->getType();

		DWORD typesize = 1;

		if (type == OP_TYPE_REG) {
			auto &lreg = reg[bytes[0]];

			if (opreg->is_indirect()) {
				if (type2 == OP_TYPE_REG) {
					auto &rreg = reg[bytes2[0]];
					if (opreg2->is_indirect()) {
						*(DWORD *)lreg.get() = *(DWORD *)rreg.get();
					}
					else {
						switch (opreg2->getOperType()) {
						case 0b11:
							*(DWORD *)lreg.get() = rreg.get();
							break;
						case 0b10:
							*(DWORD *)lreg.get() = (rreg.get() & 0xffff0000) >> 16;
							break;
						case 0b01:
							*(DWORD *)lreg.get() = (rreg.get() & 0x0000ff00) >> 4;
							break;
						case 0b00:
							*(DWORD *)lreg.get() = (rreg.get() & 0x000000ff);
							break;
						}
					}
				}
				else if (type2 == OP_TYPE_IMM) {
					DWORD ptr = *(DWORD *)opreg2->getBytes();
					if (opreg2->is_indirect()) {
						*(DWORD *)lreg.get() = *(DWORD *)ptr;
					}
					else {
						switch (opreg2->getOperType()) {
						case 0b11:
						case 0b00:
							*(DWORD *)lreg.get() = ptr;
							break;
						case 0b10:
							*(DWORD *)lreg.get() = (ptr & 0xffff0000) >> 16;
							break;
						case 0b01:
							*(DWORD *)lreg.get() = (ptr & 0x000000ff);
							break;
						}
					}
					typesize++;
				}
			}
			else {
				if (type2 == OP_TYPE_REG) {
					auto &rreg = reg[bytes2[0]];
					if (opreg2->is_indirect()) {
						lreg = *(DWORD *)rreg.get();
					}
					else {
						switch (opreg2->getOperType()) {
						case 0b11:
							lreg = rreg.get();
							break;
						case 0b10:
							lreg = (lreg & 0xffff0000) | (rreg.get() & 0xffff0000) >> 16;
							break;
						case 0b01:
							lreg = (lreg & 0xffff00ff) | (rreg.get() & 0x0000ff00) >> 4;
							break;
						case 0b00:
							lreg = (rreg.get() & 0x000000ff);
							break;
						}
						lreg = rreg;
					}
				}
				else if (type2 == OP_TYPE_IMM) {
					DWORD ptr = *(DWORD *)opreg2->getBytes();
					if (opreg2->is_indirect()) {
						lreg = *(DWORD *)ptr;
					}
					else {
						lreg = ptr;
					}
					typesize++;
				}
			}
		}
		else if (type == OP_TYPE_IMM) {
			typesize++;

			if (opreg->is_indirect()) {
				DWORD *lptr = (DWORD *)opreg->getBytes();
				if (type2 == OP_TYPE_REG) {
					auto &rreg = reg[bytes2[0]];
					if (opreg2->is_indirect()) {
						*lptr = *(DWORD *)rreg.get();
					}
					else {
						*lptr = rreg.get();
					}
				}
				else if (type2 == OP_TYPE_IMM) {
					DWORD ptr = *(DWORD *)opreg2->getBytes();
					if (opreg2->is_indirect()) {
						*lptr = *(DWORD *)ptr;
					}
					else {
						*lptr = ptr;
					}
					typesize++;
				}
			}
			else {
				throw "Unknown Operand Type";
			}
		}

		return typesize + opreg->getSize() + opreg2->getSize();
	}
	DWORD _Private Handler::push(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack) {
		auto &opreg = dpinfo->operand[0];

		BYTE *bytes = opreg->getBytes();
		DWORD type = opreg->getType();

		DWORD sbase = (DWORD)stack.get();
		DWORD typesize = 1;

		if (type == OP_TYPE_REG) {
			auto &lreg = reg[bytes[0]];

			if (opreg->is_indirect()) {
				if (reg["esp"] <= sbase)
					throw "Stack Memory Top Reached";

				reg["esp"] -= stack.getAlign();
				DWORD *ptr = *reg["esp"];
				DWORD val = lreg.get();
				*ptr = *(DWORD *)val;
			}
			else {
				if (reg["esp"] <= sbase)
					throw "Stack Memory Top Reached";

				reg["esp"] -= stack.getAlign();
				DWORD *ptr = *reg["esp"];
				*ptr = lreg.get();
			}
		}
		else if (type == OP_TYPE_IMM) {
			if (opreg->is_indirect()) {
				if (reg["esp"] <= sbase)
					throw "Stack Memory Top Reached";

				reg["esp"] -= stack.getAlign();
				DWORD *ptr = *reg["esp"];
				DWORD val = **(DWORD **)bytes;
				*ptr = *(DWORD *)val;
			}
			else {
				if (reg["esp"] <= sbase)
					throw "Stack Memory Top Reached";

				reg["esp"] -= stack.getAlign();
				DWORD *ptr = *reg["esp"];
				*ptr = *(DWORD *)bytes;
				typesize++;
			}
		}

		return typesize + opreg->getSize();
	}
	DWORD _Private Handler::pop(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack) {
		auto &opreg = dpinfo->operand[0];

		BYTE *bytes = opreg->getBytes();
		DWORD type = opreg->getType();

		DWORD sbase = (DWORD)stack.get();
		DWORD typesize = 1;

		if (type == OP_TYPE_REG) {
			auto &lreg = reg[bytes[0]];

			if (opreg->is_indirect()) {
				throw "Unknown Operand Type";
			}
			else {
				if (reg["esp"] >= sbase + stack.getSize())
					throw "Stack Memory Exceeded";

				DWORD *ptr = *reg["esp"];
				DWORD val = *ptr;
				reg["esp"] += stack.getAlign();

				reg[bytes[0]] = val;
			}
		}
		else if (type == OP_TYPE_IMM) {
			throw "Unknown Operand Type";
		}

		return typesize + opreg->getSize();
	}
	DWORD _Private Handler::ret(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack) {
		DWORD sbase = (DWORD)stack.get();

		if (reg["esp"] >= sbase + stack.getSize())
			throw "Stack Memory Exceeded";

		DWORD *ptr = *reg["esp"];
		reg["eip"] = *ptr;
		reg["esp"] += stack.getAlign();
		return 0;
	}

	DWORD _Private Handler::add(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack) {
		auto &opreg = dpinfo->operand[0];
		auto &opreg2 = dpinfo->operand[1];

		BYTE *bytes = opreg->getBytes();
		DWORD type = opreg->getType();

		BYTE *bytes2 = opreg2->getBytes();
		DWORD type2 = opreg2->getType();
		DWORD prev_value, value;
		DWORD typesize = 1;

		if (type == OP_TYPE_REG) {
			auto &lreg = reg[bytes[0]];

			if (opreg->is_indirect()) {
				prev_value = *(DWORD *)lreg.get();
				if (type2 == OP_TYPE_REG) {
					auto &rreg = reg[bytes2[0]];
					if (opreg2->is_indirect()) {
						*(DWORD *)lreg.get() += *(DWORD *)rreg.get();
					}
					else {
						*(DWORD *)lreg.get() += rreg.get();
					}
				}
				else if (type2 == OP_TYPE_IMM) {
					DWORD ptr = *(DWORD *)opreg2->getBytes();
					if (opreg2->is_indirect()) {
						*(DWORD *)lreg.get() += *(DWORD *)ptr;
					}
					else {
						*(DWORD *)lreg.get() += ptr;
					}
					typesize++;
				}
				value = *(DWORD *)lreg.get();
			}
			else {
				prev_value = lreg.get();
				if (type2 == OP_TYPE_REG) {
					auto &rreg = reg[bytes2[0]];
					if (opreg2->is_indirect()) {
						lreg += *(DWORD *)rreg.get();
					}
					else {
						lreg += rreg;
					}
				}
				else if (type2 == OP_TYPE_IMM) {
					DWORD ptr = *(DWORD *)opreg2->getBytes();
					if (opreg2->is_indirect()) {
						lreg += *(DWORD *)ptr;
					}
					else {
						lreg += ptr;
					}
					typesize++;
				}
				value = lreg.get();
			}
		}
		else if (type == OP_TYPE_IMM) {
			typesize++;

			if (opreg->is_indirect()) {
				DWORD *lptr = (DWORD *)opreg->getBytes();
				prev_value = *lptr;
				if (type2 == OP_TYPE_REG) {
					auto &rreg = reg[bytes2[0]];
					if (opreg2->is_indirect()) {
						*lptr += *(DWORD *)rreg.get();
					}
					else {
						*lptr += rreg.get();
					}
				}
				else if (type2 == OP_TYPE_IMM) {
					DWORD ptr = *(DWORD *)opreg2->getBytes();
					if (opreg2->is_indirect()) {
						*lptr += *(DWORD *)ptr;
					}
					else {
						*lptr += ptr;
					}
					typesize++;
				}
				value = *lptr;
			}
			else {
				throw "Unknown Operand Type";
			}
		}

		if (value == 0)
			setZF(reg);
		if (value < prev_value) {
			setCF(reg);
			setOF(reg);
		}

		return typesize + opreg->getSize() + opreg2->getSize();
	}
	DWORD _Private Handler::sub(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack) {
		auto &opreg = dpinfo->operand[0];
		auto &opreg2 = dpinfo->operand[1];

		BYTE *bytes = opreg->getBytes();
		DWORD type = opreg->getType();

		BYTE *bytes2 = opreg2->getBytes();
		DWORD type2 = opreg2->getType();
		DWORD prev_value, value;
		DWORD typesize = 1;

		if (type == OP_TYPE_REG) {
			auto &lreg = reg[bytes[0]];

			if (opreg->is_indirect()) {
				prev_value = *(DWORD *)lreg.get();
				if (type2 == OP_TYPE_REG) {
					auto &rreg = reg[bytes2[0]];
					if (opreg2->is_indirect()) {
						*(DWORD *)lreg.get() -= *(DWORD *)rreg.get();
					}
					else {
						*(DWORD *)lreg.get() -= rreg.get();
					}
				}
				else if (type2 == OP_TYPE_IMM) {
					DWORD ptr = *(DWORD *)opreg2->getBytes();
					if (opreg2->is_indirect()) {
						*(DWORD *)lreg.get() -= *(DWORD *)ptr;
					}
					else {
						*(DWORD *)lreg.get() -= ptr;
					}
					typesize++;
				}
				value = *(DWORD *)lreg.get();
			}
			else {
				prev_value = lreg.get();
				if (type2 == OP_TYPE_REG) {
					auto &rreg = reg[bytes2[0]];
					if (opreg2->is_indirect()) {
						lreg -= *(DWORD *)rreg.get();
					}
					else {
						lreg -= rreg;
					}
				}
				else if (type2 == OP_TYPE_IMM) {
					DWORD ptr = *(DWORD *)opreg2->getBytes();
					if (opreg2->is_indirect()) {
						lreg -= *(DWORD *)ptr;
					}
					else {
						lreg -= ptr;
					}
					typesize++;
				}
				value = lreg.get();
			}
		}
		else if (type == OP_TYPE_IMM) {
			typesize++;

			if (opreg->is_indirect()) {
				DWORD *lptr = (DWORD *)opreg->getBytes();
				prev_value = *lptr;
				if (type2 == OP_TYPE_REG) {
					auto &rreg = reg[bytes2[0]];
					if (opreg2->is_indirect()) {
						*lptr -= *(DWORD *)rreg.get();
					}
					else {
						*lptr -= rreg.get();
					}
				}
				else if (type2 == OP_TYPE_IMM) {
					DWORD ptr = *(DWORD *)opreg2->getBytes();
					if (opreg2->is_indirect()) {
						*lptr -= *(DWORD *)ptr;
					}
					else {
						*lptr -= ptr;
					}
					typesize++;
				}
				value = *lptr;
			}
			else {
				throw "Unknown Operand Type";
			}
		}

		if (value == 0)
			setZF(reg);
		if (value > prev_value) {
			setCF(reg);
			setOF(reg);
		}

		return typesize + opreg->getSize() + opreg2->getSize();
	}
	DWORD _Private Handler::mul(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack) {
		auto &opreg = dpinfo->operand[0];
		auto &opreg2 = dpinfo->operand[1];

		BYTE *bytes = opreg->getBytes();
		DWORD type = opreg->getType();

		BYTE *bytes2 = opreg2->getBytes();
		DWORD type2 = opreg2->getType();
		DWORD prev_value, value;
		DWORD typesize = 1;

		if (type == OP_TYPE_REG) {
			auto &lreg = reg[bytes[0]];

			if (opreg->is_indirect()) {
				prev_value = *(DWORD *)lreg.get();
				if (type2 == OP_TYPE_REG) {
					auto &rreg = reg[bytes2[0]];
					if (opreg2->is_indirect()) {
						*(DWORD *)lreg.get() *= *(DWORD *)rreg.get();
					}
					else {
						*(DWORD *)lreg.get() *= rreg.get();
					}
				}
				else if (type2 == OP_TYPE_IMM) {
					DWORD ptr = *(DWORD *)opreg2->getBytes();
					if (opreg2->is_indirect()) {
						*(DWORD *)lreg.get() *= *(DWORD *)ptr;
					}
					else {
						*(DWORD *)lreg.get() *= ptr;
					}
					typesize++;
				}
				value = *(DWORD *)lreg.get();
			}
			else {
				prev_value = lreg.get();
				if (type2 == OP_TYPE_REG) {
					auto &rreg = reg[bytes2[0]];
					if (opreg2->is_indirect()) {
						lreg *= *(DWORD *)rreg.get();
					}
					else {
						lreg *= rreg;
					}
				}
				else if (type2 == OP_TYPE_IMM) {
					DWORD ptr = *(DWORD *)opreg2->getBytes();
					if (opreg2->is_indirect()) {
						lreg *= *(DWORD *)ptr;
					}
					else {
						lreg *= ptr;
					}
					typesize++;
				}
				value = lreg.get();
			}
		}
		else if (type == OP_TYPE_IMM) {
			typesize++;

			if (opreg->is_indirect()) {
				DWORD *lptr = (DWORD *)opreg->getBytes();
				prev_value = *lptr;
				if (type2 == OP_TYPE_REG) {
					auto &rreg = reg[bytes2[0]];
					if (opreg2->is_indirect()) {
						*lptr *= *(DWORD *)rreg.get();
					}
					else {
						*lptr *= rreg.get();
					}
				}
				else if (type2 == OP_TYPE_IMM) {
					DWORD ptr = *(DWORD *)opreg2->getBytes();
					if (opreg2->is_indirect()) {
						*lptr *= *(DWORD *)ptr;
					}
					else {
						*lptr *= ptr;
					}
					typesize++;
				}
				value = *lptr;
			}
			else {
				throw "Unknown Operand Type";
			}
		}

		if (value == 0)
			setZF(reg);
		if (value < prev_value) {
			setCF(reg);
			setOF(reg);
		}

		return typesize + opreg->getSize() + opreg2->getSize();
	}
	DWORD _Private Handler::div(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack) {
		auto &opreg = dpinfo->operand[0];
		auto &opreg2 = dpinfo->operand[1];

		BYTE *bytes = opreg->getBytes();
		DWORD type = opreg->getType();

		BYTE *bytes2 = opreg2->getBytes();
		DWORD type2 = opreg2->getType();
		DWORD value;
		DWORD typesize = 1;

		if (type == OP_TYPE_REG) {
			auto &lreg = reg[bytes[0]];

			if (opreg->is_indirect()) {
				if (type2 == OP_TYPE_REG) {
					auto &rreg = reg[bytes2[0]];
					if (opreg2->is_indirect()) {
						*(DWORD *)lreg.get() /= *(DWORD *)rreg.get();
					}
					else {
						*(DWORD *)lreg.get() /= rreg.get();
					}
				}
				else if (type2 == OP_TYPE_IMM) {
					DWORD ptr = *(DWORD *)opreg2->getBytes();
					if (opreg2->is_indirect()) {
						*(DWORD *)lreg.get() /= *(DWORD *)ptr;
					}
					else {
						*(DWORD *)lreg.get() /= ptr;
					}
					typesize++;
				}
				value = *(DWORD *)lreg.get();
			}
			else {
				if (type2 == OP_TYPE_REG) {
					auto &rreg = reg[bytes2[0]];
					if (opreg2->is_indirect()) {
						lreg /= *(DWORD *)rreg.get();
					}
					else {
						lreg /= rreg;
					}
				}
				else if (type2 == OP_TYPE_IMM) {
					DWORD ptr = *(DWORD *)opreg2->getBytes();
					if (opreg2->is_indirect()) {
						lreg /= *(DWORD *)ptr;
					}
					else {
						lreg /= ptr;
					}
					typesize++;
				}
				value = lreg.get();
			}
		}
		else if (type == OP_TYPE_IMM) {
			typesize++;

			if (opreg->is_indirect()) {
				DWORD *lptr = (DWORD *)opreg->getBytes();
				if (type2 == OP_TYPE_REG) {
					auto &rreg = reg[bytes2[0]];
					if (opreg2->is_indirect()) {
						*lptr /= *(DWORD *)rreg.get();
					}
					else {
						*lptr /= rreg.get();
					}
				}
				else if (type2 == OP_TYPE_IMM) {
					DWORD ptr = *(DWORD *)opreg2->getBytes();
					if (opreg2->is_indirect()) {
						*lptr /= *(DWORD *)ptr;
					}
					else {
						*lptr /= ptr;
					}
					typesize++;
				}
				value = *lptr;
			}
			else {
				throw "Unknown Operand Type";
			}
		}

		if (value == 0)
			setZF(reg);

		return typesize + opreg->getSize() + opreg2->getSize();
	}

	DWORD _Private Handler::and(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack) {
		auto &opreg = dpinfo->operand[0];
		auto &opreg2 = dpinfo->operand[1];

		BYTE *bytes = opreg->getBytes();
		DWORD type = opreg->getType();

		BYTE *bytes2 = opreg2->getBytes();
		DWORD type2 = opreg2->getType();
		DWORD value;
		DWORD typesize = 1;

		if (type == OP_TYPE_REG) {
			auto &lreg = reg[bytes[0]];

			if (opreg->is_indirect()) {
				if (type2 == OP_TYPE_REG) {
					auto &rreg = reg[bytes2[0]];
					if (opreg2->is_indirect()) {
						*(DWORD *)lreg.get() &= *(DWORD *)rreg.get();
					}
					else {
						*(DWORD *)lreg.get() &= rreg.get();
					}
				}
				else if (type2 == OP_TYPE_IMM) {
					DWORD ptr = *(DWORD *)opreg2->getBytes();
					if (opreg2->is_indirect()) {
						*(DWORD *)lreg.get() &= *(DWORD *)ptr;
					}
					else {
						*(DWORD *)lreg.get() &= ptr;
					}
					typesize++;
				}
				value = *(DWORD *)lreg.get();
			}
			else {
				if (type2 == OP_TYPE_REG) {
					auto &rreg = reg[bytes2[0]];
					if (opreg2->is_indirect()) {
						lreg &= *(DWORD *)rreg.get();
					}
					else {
						lreg &= rreg;
					}
				}
				else if (type2 == OP_TYPE_IMM) {
					DWORD ptr = *(DWORD *)opreg2->getBytes();
					if (opreg2->is_indirect()) {
						lreg &= *(DWORD *)ptr;
					}
					else {
						lreg &= ptr;
					}
					typesize++;
				}
				value = lreg.get();
			}
		}
		else if (type == OP_TYPE_IMM) {
			typesize++;

			if (opreg->is_indirect()) {
				DWORD *lptr = (DWORD *)opreg->getBytes();
				if (type2 == OP_TYPE_REG) {
					auto &rreg = reg[bytes2[0]];
					if (opreg2->is_indirect()) {
						*lptr &= *(DWORD *)rreg.get();
					}
					else {
						*lptr &= rreg.get();
					}
				}
				else if (type2 == OP_TYPE_IMM) {
					DWORD ptr = *(DWORD *)opreg2->getBytes();
					if (opreg2->is_indirect()) {
						*lptr &= *(DWORD *)ptr;
					}
					else {
						*lptr &= ptr;
					}
					typesize++;
				}
				value = *lptr;
			}
			else {
				throw "Unknown Operand Type";
			}
		}

		if (value == 0)
			setZF(reg);

		return typesize + opreg->getSize() + opreg2->getSize();
	}
	DWORD _Private Handler::or(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack) {
		auto &opreg = dpinfo->operand[0];
		auto &opreg2 = dpinfo->operand[1];

		BYTE *bytes = opreg->getBytes();
		DWORD type = opreg->getType();

		BYTE *bytes2 = opreg2->getBytes();
		DWORD type2 = opreg2->getType();
		DWORD value;
		DWORD typesize = 1;

		if (type == OP_TYPE_REG) {
			auto &lreg = reg[bytes[0]];

			if (opreg->is_indirect()) {
				if (type2 == OP_TYPE_REG) {
					auto &rreg = reg[bytes2[0]];
					if (opreg2->is_indirect()) {
						*(DWORD *)lreg.get() |= *(DWORD *)rreg.get();
					}
					else {
						*(DWORD *)lreg.get() |= rreg.get();
					}
				}
				else if (type2 == OP_TYPE_IMM) {
					DWORD ptr = *(DWORD *)opreg2->getBytes();
					if (opreg2->is_indirect()) {
						*(DWORD *)lreg.get() |= *(DWORD *)ptr;
					}
					else {
						*(DWORD *)lreg.get() |= ptr;
					}
					typesize++;
				}
				value = *(DWORD *)lreg.get();
			}
			else {
				if (type2 == OP_TYPE_REG) {
					auto &rreg = reg[bytes2[0]];
					if (opreg2->is_indirect()) {
						lreg |= *(DWORD *)rreg.get();
					}
					else {
						lreg |= rreg;
					}
				}
				else if (type2 == OP_TYPE_IMM) {
					DWORD ptr = *(DWORD *)opreg2->getBytes();
					if (opreg2->is_indirect()) {
						lreg |= *(DWORD *)ptr;
					}
					else {
						lreg |= ptr;
					}
					typesize++;
				}
				value = lreg.get();
			}
		}
		else if (type == OP_TYPE_IMM) {
			typesize++;

			if (opreg->is_indirect()) {
				DWORD *lptr = (DWORD *)opreg->getBytes();
				if (type2 == OP_TYPE_REG) {
					auto &rreg = reg[bytes2[0]];
					if (opreg2->is_indirect()) {
						*lptr |= *(DWORD *)rreg.get();
					}
					else {
						*lptr |= rreg.get();
					}
				}
				else if (type2 == OP_TYPE_IMM) {
					DWORD ptr = *(DWORD *)opreg2->getBytes();
					if (opreg2->is_indirect()) {
						*lptr |= *(DWORD *)ptr;
					}
					else {
						*lptr |= ptr;
					}
					typesize++;
				}
				value = *lptr;
			}
			else {
				throw "Unknown Operand Type";
			}
		}

		if (value == 0)
			setZF(reg);

		return typesize + opreg->getSize() + opreg2->getSize();
	}
	DWORD _Private Handler::xor(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack) {
		auto &opreg = dpinfo->operand[0];
		auto &opreg2 = dpinfo->operand[1];

		BYTE *bytes = opreg->getBytes();
		DWORD type = opreg->getType();

		BYTE *bytes2 = opreg2->getBytes();
		DWORD type2 = opreg2->getType();
		DWORD value;
		DWORD typesize = 1;

		if (type == OP_TYPE_REG) {
			auto &lreg = reg[bytes[0]];

			if (opreg->is_indirect()) {
				if (type2 == OP_TYPE_REG) {
					auto &rreg = reg[bytes2[0]];
					if (opreg2->is_indirect()) {
						*(DWORD *)lreg.get() ^= *(DWORD *)rreg.get();
					}
					else {
						*(DWORD *)lreg.get() ^= rreg.get();
					}
				}
				else if (type2 == OP_TYPE_IMM) {
					DWORD ptr = *(DWORD *)opreg2->getBytes();
					if (opreg2->is_indirect()) {
						*(DWORD *)lreg.get() ^= *(DWORD *)ptr;
					}
					else {
						*(DWORD *)lreg.get() ^= ptr;
					}
					typesize++;
				}
				value = *(DWORD *)lreg.get();
			}
			else {
				if (type2 == OP_TYPE_REG) {
					auto &rreg = reg[bytes2[0]];
					if (opreg2->is_indirect()) {
						lreg ^= *(DWORD *)rreg.get();
					}
					else {
						lreg ^= rreg;
					}
				}
				else if (type2 == OP_TYPE_IMM) {
					DWORD ptr = *(DWORD *)opreg2->getBytes();
					if (opreg2->is_indirect()) {
						lreg ^= *(DWORD *)ptr;
					}
					else {
						lreg ^= ptr;
					}
					typesize++;
				}
				value = lreg.get();
			}
		}
		else if (type == OP_TYPE_IMM) {
			typesize++;

			if (opreg->is_indirect()) {
				DWORD *lptr = (DWORD *)opreg->getBytes();
				if (type2 == OP_TYPE_REG) {
					auto &rreg = reg[bytes2[0]];
					if (opreg2->is_indirect()) {
						*lptr ^= *(DWORD *)rreg.get();
					}
					else {
						*lptr ^= rreg.get();
					}
				}
				else if (type2 == OP_TYPE_IMM) {
					DWORD ptr = *(DWORD *)opreg2->getBytes();
					if (opreg2->is_indirect()) {
						*lptr ^= *(DWORD *)ptr;
					}
					else {
						*lptr ^= ptr;
					}
					typesize++;
				}
				value = *lptr;
			}
			else {
				throw "Unknown Operand Type";
			}
		}

		if (value == 0)
			setZF(reg);

		return typesize + opreg->getSize() + opreg2->getSize();
	}

	DWORD _Private Handler::jmp(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack) {
		auto &opreg = dpinfo->operand[0];

		BYTE *bytes = opreg->getBytes();
		DWORD type = opreg->getType();

		if (type == OP_TYPE_REG) {
			auto &lreg = reg[bytes[0]];

			if (opreg->is_indirect()) {
				reg["eip"] += *(DWORD *)lreg.get();
			}
			else {
				reg["eip"] += lreg.get();
			}
		}
		else if (type == OP_TYPE_IMM) {
			DWORD value = *(DWORD *)bytes;
			if (opreg->is_indirect()) {
				reg["eip"] += *(DWORD *)value;
			}
			else {
				reg["eip"] += value;
			}
		}

		return 0;
	}
	DWORD _Private Handler::je(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack) {
		auto &opreg = dpinfo->operand[0];

		BYTE *bytes = opreg->getBytes();
		DWORD type = opreg->getType();
		DWORD typesize = 1;

		if (type == OP_TYPE_REG) {
			if (!getZF(reg))
				return typesize + opreg->getSize();
			auto &lreg = reg[bytes[0]];

			if (opreg->is_indirect()) {
				reg["eip"] += *(DWORD *)lreg.get();
			}
			else {
				reg["eip"] += lreg.get();
			}
		}
		else if (type == OP_TYPE_IMM) {
			typesize++;
			if (!getZF(reg))
				return typesize + opreg->getSize();

			DWORD value = *(DWORD *)bytes;
			if (opreg->is_indirect()) {
				reg["eip"] += *(DWORD *)value;
			}
			else {
				reg["eip"] += value;
			}
		}

		return 0;
	}
	DWORD _Private Handler::jne(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack) {
		auto &opreg = dpinfo->operand[0];

		BYTE *bytes = opreg->getBytes();
		DWORD type = opreg->getType();
		DWORD typesize = 1;

		if (type == OP_TYPE_REG) {
			if(getZF(reg))
				return typesize + opreg->getSize();

			auto &lreg = reg[bytes[0]];

			if (opreg->is_indirect()) {
				reg["eip"] += *(DWORD *)lreg.get() + opreg->getSize();
			}
			else {
				reg["eip"] += lreg.get() + opreg->getSize();
			}
		}
		else if (type == OP_TYPE_IMM) {
			typesize++;
			if (getZF(reg))
				return typesize + opreg->getSize();

			DWORD value = *(DWORD *)bytes;
			if (opreg->is_indirect()) {
				reg["eip"] += *(DWORD *)value + opreg->getSize() + 1;
			}
			else {
				reg["eip"] += value;
			}
		}

		return 0;
	}
	DWORD _Private Handler::ja(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack) {
		auto &opreg = dpinfo->operand[0];

		BYTE *bytes = opreg->getBytes();
		DWORD type = opreg->getType();
		DWORD typesize = 1;

		if (type == OP_TYPE_REG) {
			if (!(!getCF(reg) && !getZF(reg)))
				return typesize + opreg->getSize();

			auto &lreg = reg[bytes[0]];

			if (opreg->is_indirect()) {
				reg["eip"] += *(DWORD *)lreg.get();
			}
			else {
				reg["eip"] += lreg.get();
			}
		}
		else if (type == OP_TYPE_IMM) {
			typesize++;
			if (!(!getCF(reg) && !getZF(reg)))
				return typesize + opreg->getSize();

			DWORD value = *(DWORD *)bytes;
			if (opreg->is_indirect()) {
				reg["eip"] += *(DWORD *)value;
			}
			else {
				reg["eip"] += value;
			}
		}
		else {
			throw "Unknown Operand Type";
		}

		return 0;
	}
	DWORD _Private Handler::jb(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack) {
		auto &opreg = dpinfo->operand[0];

		BYTE *bytes = opreg->getBytes();
		DWORD type = opreg->getType();
		DWORD typesize = 1;

		if (type == OP_TYPE_REG) {
			if (!getCF(reg))
				return typesize + opreg->getSize();

			auto &lreg = reg[bytes[0]];

			if (opreg->is_indirect()) {
				reg["eip"] += *(DWORD *)lreg.get();
			}
			else {
				reg["eip"] += lreg.get();
			}
		}
		else if (type == OP_TYPE_IMM) {
			typesize++;
			if (!getCF(reg))
				return typesize + opreg->getSize();

			DWORD value = *(DWORD *)bytes;
			if (opreg->is_indirect()) {
				reg["eip"] += *(DWORD *)value;
			}
			else {
				reg["eip"] += value;
			}
		}
		else {
			throw "Unknown Operand Type";
		}

		return 0;
	}
	DWORD _Private Handler::jl(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack) {
		auto &opreg = dpinfo->operand[0];

		BYTE *bytes = opreg->getBytes();
		DWORD type = opreg->getType();
		DWORD typesize = 1;

		if (type == OP_TYPE_REG) {
			if (getNF(reg) == getOF(reg))
				return typesize + opreg->getSize();

			auto &lreg = reg[bytes[0]];

			if (opreg->is_indirect()) {
				reg["eip"] += *(DWORD *)lreg.get();
			}
			else {
				reg["eip"] += lreg.get();
			}
		}
		else if (type == OP_TYPE_IMM) {
			typesize++;
			if (getNF(reg) == getOF(reg))
				return typesize + opreg->getSize();

			DWORD value = *(DWORD *)bytes;
			if (opreg->is_indirect()) {
				reg["eip"] += *(DWORD *)value;
			}
			else {
				reg["eip"] += value;
			}
		}
		else {
			throw "Unknown Operand Type";
		}

		return 0;
	}
	DWORD _Private Handler::jle(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack) {
		auto &opreg = dpinfo->operand[0];

		BYTE *bytes = opreg->getBytes();
		DWORD type = opreg->getType();
		DWORD typesize = 1;

		if (type == OP_TYPE_REG) {
			if (!(getZF(reg) || getNF(reg) != getOF(reg)))
				return typesize + opreg->getSize();

			auto &lreg = reg[bytes[0]];

			if (opreg->is_indirect()) {
				reg["eip"] += *(DWORD *)lreg.get();
			}
			else {
				reg["eip"] += lreg.get();
			}
		}
		else if (type == OP_TYPE_IMM) {
			typesize++;
			if (!(getZF(reg) || getNF(reg) != getOF(reg)))
				return typesize + opreg->getSize();

			DWORD value = *(DWORD *)bytes;
			if (opreg->is_indirect()) {
				reg["eip"] += *(DWORD *)value;
			}
			else {
				reg["eip"] += value;
			}
		}
		else {
			throw "Unknown Operand Type";
		}

		return 0;
	}
	DWORD _Private Handler::jz(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack) {
		auto &opreg = dpinfo->operand[0];

		BYTE *bytes = opreg->getBytes();
		DWORD type = opreg->getType();
		DWORD typesize = 1;

		if (type == OP_TYPE_REG) {
			if (!getZF(reg))
				return typesize + opreg->getSize();

			auto &lreg = reg[bytes[0]];

			if (opreg->is_indirect()) {
				reg["eip"] += *(DWORD *)lreg.get();
			}
			else {
				reg["eip"] += lreg.get();
			}
		}
		else if (type == OP_TYPE_IMM) {
			typesize++;
			if (!getZF(reg))
				return typesize + opreg->getSize();

			DWORD value = *(DWORD *)bytes;
			if (opreg->is_indirect()) {
				reg["eip"] += *(DWORD *)value;
			}
			else {
				reg["eip"] += value;
			}
		}
		else {
			throw "Unknown Operand Type";
		}

		return 0;
	}
	DWORD _Private Handler::exit(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack) {
		throw "Program Exit";
		return 0;
	}

	DWORD _Private Handler::add_except(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack) {
		auto &opreg = dpinfo->operand[0];

		BYTE *bytes = opreg->getBytes();
		DWORD type = opreg->getType();
		DWORD typesize = 1;

		if (type == OP_TYPE_REG) {
			auto &lreg = reg[bytes[0]];
			DWORD func = lreg.get();
			if (opreg->is_indirect()) {
				fnExp.add_except((void *)(*(DWORD *)func));
			}
			else {
				fnExp.add_except((void *)func);
			}
		}
		else if(type == OP_TYPE_IMM) {
			DWORD func = *(DWORD *)bytes;
			if (opreg->is_indirect()) {
				fnExp.add_except((void *)(*(DWORD *)func));
			}
			else {
				fnExp.add_except((void *)func);
			}
			typesize++;
		}
		else {
			throw "Unknown Operand Type";
		}

		return typesize + opreg->getSize();
	}
	DWORD _Private Handler::del_except(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack) {
		auto &opreg = dpinfo->operand[0];

		BYTE *bytes = opreg->getBytes();
		DWORD type = opreg->getType();
		DWORD typesize = 1;

		if (type == OP_TYPE_REG) {
			auto &lreg = reg[bytes[0]];
			DWORD func = lreg.get();
			if (opreg->is_indirect()) {
				fnExp.del_except((void *)(*(DWORD *)func));
			}
			else {
				fnExp.del_except((void *)func);
			}
		}
		else if (type == OP_TYPE_IMM) {
			DWORD func = *(DWORD *)bytes;
			if (opreg->is_indirect()) {
				fnExp.del_except((void *)(*(DWORD *)func));
			}
			else {
				fnExp.del_except((void *)func);
			}
			typesize++;
		}
		else {
			throw "Unknown Operand Type";
		}

		return typesize + opreg->getSize();
	}
	DWORD _Private Handler::call_except(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack) {
		auto &opreg = dpinfo->operand[0];

		BYTE *bytes = opreg->getBytes();
		DWORD type = opreg->getType();
		DWORD typesize = 1;

		if (type == OP_TYPE_REG) {
			auto &lreg = reg[bytes[0]];
			DWORD func = lreg.get();
			if (opreg->is_indirect()) {
				if(!fnExp.is_func((void *)(*(DWORD *)func)))
					throw "Not Defined Function";

				reg["eip"] = *(DWORD *)func;
			}
			else {
				if (!fnExp.is_func((void *)func))
					throw "Not Defined Function";

				reg["eip"] = func;
			}
		}
		else if (type == OP_TYPE_IMM) {
			typesize++;
			DWORD func = *(DWORD *)bytes;
			if (opreg->is_indirect()) {
				if (!fnExp.is_func((void *)(*(DWORD *)func)))
					throw "Not Defined Function";

				reg["eip"] = *(DWORD *)func;
			}
			else {
				if (!fnExp.is_func((void *)func))
					throw "Not Defined Function";

				reg["eip"] = func;
			}
		}
		else {
			throw "Unknown Operand Type";
		}

		DWORD retaddr = reg["eip"] + typesize + opreg->getSize();
		reg["esp"] -= stack.getAlign();
		*(DWORD *)reg["esp"].get() = retaddr;

		return 0;
	}

	DWORD _Private Handler::cmp(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack) {
		auto &opreg = dpinfo->operand[0];
		auto &opreg2 = dpinfo->operand[1];

		BYTE *bytes = opreg->getBytes();
		DWORD type = opreg->getType();

		BYTE *bytes2 = opreg2->getBytes();
		DWORD type2 = opreg2->getType();
		DWORD prev_value, value;
		DWORD typesize = 1;

		if (type == OP_TYPE_REG) {
			auto &lreg = reg[bytes[0]];

			if (opreg->is_indirect()) {
				prev_value = *(DWORD *)lreg.get();
				if (type2 == OP_TYPE_REG) {
					auto &rreg = reg[bytes2[0]];
					if (opreg2->is_indirect()) {
						*(DWORD *)lreg.get() -= *(DWORD *)rreg.get();
					}
					else {
						*(DWORD *)lreg.get() -= rreg.get();
					}
				}
				else if (type2 == OP_TYPE_IMM) {
					DWORD ptr = *(DWORD *)opreg2->getBytes();
					if (opreg2->is_indirect()) {
						*(DWORD *)lreg.get() -= *(DWORD *)ptr;
					}
					else {
						*(DWORD *)lreg.get() -= ptr;
					}
					typesize++;
				}
				value = *(DWORD *)lreg.get();
			}
			else {
				prev_value = lreg.get();
				if (type2 == OP_TYPE_REG) {
					auto &rreg = reg[bytes2[0]];
					if (opreg2->is_indirect()) {
						lreg -= *(DWORD *)rreg.get();
					}
					else {
						lreg -= rreg;
					}
				}
				else if (type2 == OP_TYPE_IMM) {
					DWORD ptr = *(DWORD *)opreg2->getBytes();
					if (opreg2->is_indirect()) {
						lreg -= *(DWORD *)ptr;
					}
					else {
						lreg -= ptr;
					}
					typesize++;
				}
				value = lreg.get();
			}
		}
		else if (type == OP_TYPE_IMM) {
			typesize++;

			if (opreg->is_indirect()) {
				DWORD *lptr = (DWORD *)opreg->getBytes();
				prev_value = *lptr;
				if (type2 == OP_TYPE_REG) {
					auto &rreg = reg[bytes2[0]];
					if (opreg2->is_indirect()) {
						*lptr -= *(DWORD *)rreg.get();
					}
					else {
						*lptr -= rreg.get();
					}
				}
				else if (type2 == OP_TYPE_IMM) {
					DWORD ptr = *(DWORD *)opreg2->getBytes();
					if (opreg2->is_indirect()) {
						*lptr -= *(DWORD *)ptr;
					}
					else {
						*lptr -= ptr;
					}
					typesize++;
				}
				value = *lptr;
			}
			else {
				throw "Unknown Operand Type";
			}
		}

		if (value == 0)
			setZF(reg);
		if (value > prev_value) {
			reg["flags"] = 0;
			setCF(reg);
			setOF(reg);
		}

		return typesize + opreg->getSize() + opreg2->getSize();
	}
	DWORD _Private Handler::test(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack) {
		auto &opreg = dpinfo->operand[0];
		auto &opreg2 = dpinfo->operand[1];

		BYTE *bytes = opreg->getBytes();
		DWORD type = opreg->getType();

		BYTE *bytes2 = opreg2->getBytes();
		DWORD type2 = opreg2->getType();
		DWORD value;
		DWORD typesize = 1;

		if (type == OP_TYPE_REG) {
			auto &lreg = reg[bytes[0]];

			if (opreg->is_indirect()) {
				if (type2 == OP_TYPE_REG) {
					auto &rreg = reg[bytes2[0]];
					if (opreg2->is_indirect()) {
						*(DWORD *)lreg.get() &= *(DWORD *)rreg.get();
					}
					else {
						*(DWORD *)lreg.get() &= rreg.get();
					}
				}
				else if (type2 == OP_TYPE_IMM) {
					DWORD ptr = *(DWORD *)opreg2->getBytes();
					if (opreg2->is_indirect()) {
						*(DWORD *)lreg.get() &= *(DWORD *)ptr;
					}
					else {
						*(DWORD *)lreg.get() &= ptr;
					}
					typesize++;
				}
				value = *(DWORD *)lreg.get();
			}
			else {
				if (type2 == OP_TYPE_REG) {
					auto &rreg = reg[bytes2[0]];
					if (opreg2->is_indirect()) {
						lreg &= *(DWORD *)rreg.get();
					}
					else {
						lreg &= rreg;
					}
				}
				else if (type2 == OP_TYPE_IMM) {
					DWORD ptr = *(DWORD *)opreg2->getBytes();
					if (opreg2->is_indirect()) {
						lreg &= *(DWORD *)ptr;
					}
					else {
						lreg &= ptr;
					}
					typesize++;
				}
				value = lreg.get();
			}
		}
		else if (type == OP_TYPE_IMM) {
			typesize++;

			if (opreg->is_indirect()) {
				DWORD *lptr = (DWORD *)opreg->getBytes();
				if (type2 == OP_TYPE_REG) {
					auto &rreg = reg[bytes2[0]];
					if (opreg2->is_indirect()) {
						*lptr &= *(DWORD *)rreg.get();
					}
					else {
						*lptr &= rreg.get();
					}
				}
				else if (type2 == OP_TYPE_IMM) {
					DWORD ptr = *(DWORD *)opreg2->getBytes();
					if (opreg2->is_indirect()) {
						*lptr &= *(DWORD *)ptr;
					}
					else {
						*lptr &= ptr;
					}
					typesize++;
				}
				value = *lptr;
			}
			else {
				throw "Unknown Operand Type";
			}
		}

		if (value == 0) {
			reg["flags"] = 0;
			setZF(reg);
		}

		return typesize + opreg->getSize() + opreg2->getSize();
	}

	/*
	* eax = syscall idx
	* ebx = arg1
	* ecx = arg2
	* edx = arg3
	* esi = arg4
	* edi = arg5
	*/
	DWORD _Private Handler::syscall(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack) {
		auto index = reg["eax"].get();
		auto size = sizeof(SyscallTable) / sizeof(Syscall);

		if (index < size)
			SyscallTable[index].callback(dpinfo, reg, stack);
		else
			throw "Unknown Syscall";

		return 1;
	}

	inline void _Private Handler::setZF(RegisterList<> &reg) { reg["flags"] = reg["flags"].get() | 0b00001000; }
	inline bool _Private Handler::getZF(RegisterList<> &reg) { return ((reg["flags"].get() & 0b00001000) >> 3) == 1; }
	inline void _Private Handler::setCF(RegisterList<> &reg) { reg["flags"] = reg["flags"].get() | 0b00010000; }
	inline bool _Private Handler::getCF(RegisterList<> &reg) { return ((reg["flags"].get() & 0b00010000) >> 4) == 1; }
	inline void _Private Handler::setOF(RegisterList<> &reg) { reg["flags"] = reg["flags"].get() | 0b00100000; }
	inline bool _Private Handler::getOF(RegisterList<> &reg) { return ((reg["flags"].get() & 0b00100000) >> 5) == 1; }
	inline void _Private Handler::setNF(RegisterList<> &reg) { reg["flags"] = reg["flags"].get() | 0b00100000; }
	inline bool _Private Handler::getNF(RegisterList<> &reg) { return ((reg["flags"].get() & 0b00100000) >> 6) == 1; }
	inline void _Private Handler::setDF(RegisterList<> &reg) { reg["flags"] = reg["flags"].get() | 0b01000000; }
	inline bool _Private Handler::getDF(RegisterList<> &reg) { return ((reg["flags"].get() & 0b01000000) >> 7) == 1; }

	DWORD _Public Handler::handle(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack) {
		if (dpinfo == nullptr)
			return 0;

		switch (dpinfo->opcode) {
		case OP_PUSH:
			return this->push(dpinfo, reg, stack);
		case OP_POP:
			return this->pop(dpinfo, reg, stack);
		case OP_MOV:
			return this->mov(dpinfo, reg, stack);
		case OP_RET:
			return this->ret(dpinfo, reg, stack);
		case OP_ADD:
			return this->add(dpinfo, reg, stack);
		case OP_SUB:
			return this->sub(dpinfo, reg, stack);
		case OP_MUL:
			return this->mul(dpinfo, reg, stack);
		case OP_DIV:
			return this->div(dpinfo, reg, stack);
		case OP_AND:
			return this->and(dpinfo, reg, stack);
		case OP_OR:
			return this->or(dpinfo, reg, stack);
		case OP_XOR:
			return this->xor(dpinfo, reg, stack);
		case OP_JMP:
			return this->jmp(dpinfo, reg, stack);
		case OP_JE:
			return this->je(dpinfo, reg, stack);
		case OP_JNE:
			return this->jne(dpinfo, reg, stack);
		case OP_JA:
			return this->ja(dpinfo, reg, stack);
		case OP_JB:
			return this->jb(dpinfo, reg, stack);
		case OP_JL:
			return this->jl(dpinfo, reg, stack);
		case OP_JLE:
			return this->jle(dpinfo, reg, stack);
		case OP_JZ:
			return this->jz(dpinfo, reg, stack);
		case OP_CMP:
			return this->cmp(dpinfo, reg, stack);
		case OP_TEST:
			return this->test(dpinfo, reg, stack);
		case OP_EXIT:
			return this->exit(dpinfo, reg, stack);
		case OP_SYSCALL:
			return this->syscall(dpinfo, reg, stack);
		case OP_ADD_EXCEPT:
			return this->add_except(dpinfo, reg, stack);
		case OP_DEL_EXCEPT:
			return this->del_except(dpinfo, reg, stack);
		case OP_EXCEPT_CALL:
			return this->call_except(dpinfo, reg, stack);
		default:
			throw "Illegal Instruction";
		}
	}
}