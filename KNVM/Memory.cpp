#include "Memory.h"
#include "types.h"
#include "Optable.h"

#include <iterator>

namespace KNVM {
	void * _Public Memory::get() { return address; }
	DWORD _Public Memory::getAlign() const { return align; }
	DWORD _Public Memory::getSize() const { return size; }
	DWORD _Public Memory::getCodeSize() const { return codesize; }

	Memory & _Public Memory::operator=(Memory &mem) {
		if (address != nullptr)
			VirtualFree(address, size, MEM_DECOMMIT);

		this->address = mem.address;
		this->size = mem.size;
		this->protect = mem.protect;

		return *this;
	}

	void * _Public Memory::operator+(DWORD range) { return (void *)((DWORD)address + range); }
	void * _Public Memory::operator-(DWORD range) { return (void *)((DWORD)address - range); }
	void * _Public Memory::operator++() {
		address = (void *)((DWORD)address + align);
		if ((DWORD)address > (DWORD)baseaddr + size)
			throw "Memory Exceeded";
		return address;
	}
	void * _Public Memory::operator--() {
		address = (void *)((DWORD)address - align);
		if ((DWORD)address < (DWORD)baseaddr)
			throw "Memory Top Reached";
		return address;
	}
	void * _Public Memory::operator+=(DWORD p) {
		address = (void *)((DWORD)address + align * p);
		if ((DWORD)address > (DWORD)baseaddr + size)
			throw "Memory Exceeded";
		return address;
	}
	void * _Public Memory::operator-=(DWORD p) {
		address = (void *)((DWORD)address - align * p);
		if ((DWORD)address < (DWORD)baseaddr)
			throw "Memory Top Reached";
		return address;
	}

	Memory &_Public Memory::operator+=(Asm asmbly) {
		BYTE *code = (BYTE *)address + codesize;
		std::memcpy(code, asmbly.code, asmbly.codesize);

		codesize += asmbly.codesize;
		
		return *this;
	}
}