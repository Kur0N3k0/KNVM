#pragma once

#include "Memory.h"

#include <fstream>
#include <Windows.h>

namespace KNVM {

#pragma pack(push, 1)
	typedef struct KNF {
		BYTE signature[4];
		enum {
			KNF_X86 = 0x86,
			KFN_X64 = 0x64
		} bits;
		
		void *entrypoint;
		DWORD codesize;
		DWORD datasize;
	} KNF;
#pragma pack(pop)

	class KNFBuilder {
	private:
		KNF header;
		Memory code;
		Memory data;
	public:
		KNFBuilder(KNF &header, Memory &code, Memory &data) : header(header), code(code), data(data) { }
		bool build(const char *path);
	};
}