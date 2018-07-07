#pragma once

#include <Windows.h>

namespace KNVM {

#pragma pack(push, 1)
	typedef struct KNF {
		BYTE signature[4];
		enum {
			KNF_X86 = 0x86,
			KFN_X64 = 0x64
		} bits;
		
		size_t entrypoint;
		DWORD codesize;
		DWORD datasize;
	} KNF;
#pragma pack(pop)

}