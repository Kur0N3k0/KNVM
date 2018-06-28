#pragma once

#include <vector>

#include "Asm.h"
#include <Windows.h>

namespace KNVM {
	class Memory {
	private:
		void *address = nullptr;
		void *baseaddr = nullptr;
		DWORD size;
		DWORD codesize;
		DWORD protect;
		DWORD align;

	public:
		Memory(DWORD size, DWORD protect, DWORD align) : codesize(0), size(size), protect(protect), align(align) {
			address = VirtualAlloc(NULL, size, MEM_COMMIT, protect);
			baseaddr = address;
			memset(address, 0xff, size);
		}
		~Memory() {
			baseaddr = nullptr;
			VirtualFree(address, size, MEM_DECOMMIT);
		}
		void *get();
		DWORD getAlign() const;
		DWORD getSize() const;
		DWORD getCodeSize() const;
		Memory &operator=(Memory &mem);
		void *operator+(DWORD range);
		void *operator-(DWORD range);
		void *operator++();
		void *operator--();
		void *operator+=(DWORD p);
		void *operator-=(DWORD p);
		Memory &operator+=(Asm asmbly);
	};
}