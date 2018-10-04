#pragma once

#include <iostream>
#include <Windows.h>

namespace KNVM {
	class Operand {
	private:
		DWORD type;
		BYTE *bytes;
		BYTE opertype;
		DWORD size;
		bool indirect = false;

	public:
		Operand(DWORD type, BYTE *bytes, DWORD size, BYTE opertype, bool indirect = false) : type(type), size(size), opertype(opertype), indirect(indirect) {
			this->bytes = new BYTE[size];
			std::memcpy(this->bytes, bytes, size);
		}
		~Operand() {
			if (this->bytes)
				delete this->bytes;
		}

		DWORD getType() const { return type; }
		BYTE *getBytes() const { return bytes; }
		BYTE getOperType() const { return opertype; }
		DWORD getSize() const { return size; }
		bool is_indirect() const { return indirect; }
		
	};
}