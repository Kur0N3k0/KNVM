#include "Memory.h"

namespace KNVM {
	void *Memory::get() const { return address; }
	DWORD Memory::getAlign() const { return align; }

	Memory &Memory::operator=(Memory &mem) {
		if (address != nullptr)
			VirtualFree(address, size, MEM_DECOMMIT);

		this->address = mem.address;
		this->size = mem.size;
		this->protect = mem.protect;

		return *this;
	}

	void *Memory::operator+(DWORD range) { return (void *)((DWORD)address + range); }
	void *Memory::operator-(DWORD range) { return (void *)((DWORD)address - range); }
	void *Memory::operator++() {
		address = (void *)((DWORD)address + align);
		return address;
	}
	void *Memory::operator--() {
		address = (void *)((DWORD)address - align);
		return address;
	}
	void *Memory::operator+=(DWORD p) {
		address = (void *)((DWORD)address + align * p);
		return address;
	}
	void *Memory::operator-=(DWORD p) {
		address = (void *)((DWORD)address - align * p);
		return address;
	}
}