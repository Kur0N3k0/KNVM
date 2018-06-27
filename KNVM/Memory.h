#pragma once

#include <Windows.h>

namespace KNVM {
	class Memory {
	private:
		void *address = nullptr;
		DWORD size;
		DWORD protect;
		DWORD align;

	public:
		Memory(DWORD size, DWORD protect, DWORD align) : size(size), protect(protect), align(align) {
			address = VirtualAlloc(NULL, size, MEM_COMMIT, protect);
		}
		~Memory() {
			VirtualFree(address, size, MEM_DECOMMIT);
		}
		void *get() const;
		Memory &operator=(Memory &mem);
		void *operator+(DWORD range);
		void *operator-(DWORD range);
		void *operator++();
		void *operator--();
		void *operator+=(DWORD p);
		void *operator-=(DWORD p);
	};
}