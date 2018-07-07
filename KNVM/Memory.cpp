#include "Memory.h"
#include "types.h"
#include "Optable.h"

#include <iterator>

namespace KNVM {
	void * _Public Memory::get() { return address; }
	DWORD _Public Memory::getAlign() const { return align; }
	DWORD _Public Memory::getSize() const { return size; }
	DWORD _Public Memory::getCodeSize() const { return codesize; }
	void _Public Memory::setCodeSize(DWORD codesize) { this->codesize = codesize; }
	DWORD _Public Memory::getDataSize() const { return datasize; }
	void _Public Memory::setDataSize(DWORD datasize) { this->datasize = datasize; }
	Memory _Public Memory::getCodePage() const { return Memory((void *)((BYTE *)address + OFFSET_CODE), codesize); }
	Memory _Public Memory::getDataPage() const { return Memory((void *)((BYTE *)address + OFFSET_DATA), datasize); }
	Memory _Public Memory::getStackPage() const { return Memory((void *)((BYTE *)address + OFFSET_STACK), MAX_MEMORY_SIZE - OFFSET_STACK); }

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