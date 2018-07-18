#pragma once

#include <iostream>
#include <Windows.h>

namespace KNVM {
	class Operand {
	private:
		DWORD type;
		BYTE *bytes;
		DWORD size;
		bool indirect = false;

	public:
		Operand(DWORD type, BYTE *bytes, DWORD size, bool indirect = false) : type(type), size(size), indirect(indirect) {
			this->bytes = new BYTE[size];
			std::memcpy(this->bytes, bytes, size);
		}
		~Operand() {
			if (this->bytes)
				delete this->bytes;
		}

		DWORD getType() const { return type; }
		BYTE *getBytes() const { return bytes; }
		DWORD getSize() const { return size; }
		bool is_indirect() const { return indirect; }
	};
}