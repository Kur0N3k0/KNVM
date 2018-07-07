#pragma once

#include <vector>

#include "Asm.h"
#include <Windows.h>

namespace KNVM {
	size_t OFFSET_CODE = 0x1000;
	size_t OFFSET_DATA = 0x4000;
	size_t OFFSET_STACK = 0x8000;
	size_t MAX_MEMORY_SIZE = 0xc000;

	class Memory {
	private:
		void *address = nullptr;
		void *baseaddr = nullptr;
		DWORD size;
		DWORD codesize;
		DWORD datasize;
		DWORD protect;
		DWORD align;
		bool is_sub = false;

	public:
		Memory(void *addr, DWORD size) : address(addr), baseaddr(addr), size(size) { this->is_sub = true; }
		Memory(DWORD size, DWORD protect, DWORD align) : codesize(0), size(size), protect(protect), align(align) {
			address = VirtualAlloc(NULL, size, MEM_COMMIT, protect);
			baseaddr = address;
			memset(address, 0xff, size);
		}
		~Memory() {
			baseaddr = nullptr;
			if(!is_sub)
				VirtualFree(address, size, MEM_DECOMMIT);
		}
		void *get();
		DWORD getAlign() const;
		DWORD getSize() const;
		DWORD getCodeSize() const;
		DWORD getDataSize() const;
		void setCodeSize(DWORD codesize);
		void setDataSize(DWORD codesize);
		Memory getCodePage() const;
		Memory getDataPage() const;
		Memory getStackPage() const;
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