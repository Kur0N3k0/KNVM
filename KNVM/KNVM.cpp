#include "KNVM.h"

namespace KNVM {
	bool KNVM::ParseBinary(KNF &knf, Memory &code, Memory &data) {
		if (knf.bits == KNF::KFN_X64)
			return false;

		memory->setCodeSize(knf.codesize);
		memory->setDataSize(knf.datasize);
		entrypoint = knf.entrypoint;

		void *codebase = memory->getCodePage().get();
		std::memcpy(codebase, code.get(), code.getCodeSize());
	}
	bool KNVM::ParseBinary(const char *path) {
		std::ifstream file(path, std::ios::in | std::ios::binary);
		auto fpos = file.tellg();
		file.seekg(0, std::ios::end);
		fpos = file.tellg() - fpos;
		file.seekg(0);

		DWORD filesize = fpos;
		
		KNF knf;
		char *buffer = new char[filesize];
		file.read(buffer, filesize);

		std::memcpy(&knf, buffer, sizeof(KNF));

		// currently not supported
		if (knf.bits == KNF::KFN_X64)
			return false;

		if (OFFSET_DATA - knf.codesize > OFFSET_DATA)
			return false;

		if (OFFSET_STACK - knf.datasize > OFFSET_STACK)
			return false;

		//if (knf.entrypoint < (void *)OFFSET_CODE || knf.entrypoint >= (void *)OFFSET_DATA)
		//	return false;

		void *baseaddr = memory->get();
		if (baseaddr == nullptr)
			return false;

		memory->setCodeSize(knf.codesize);
		memory->setDataSize(knf.datasize);

		BYTE *ptr = (BYTE *)baseaddr + OFFSET_CODE;
		void *code = &buffer[OFFSET_CODE];
		std::memcpy(ptr, code, knf.codesize);

		ptr = (BYTE *)baseaddr + OFFSET_DATA;
		void *data = &buffer[OFFSET_DATA];
		std::memcpy(ptr, data, knf.datasize);

		file.close();
		return true;
	}
	void KNVM::Emulate() {
		Memory code = memory->getCodePage();
		Memory stack = memory->getStackPage();
		code.setCodeSize(memory->getCodeSize());
		cpu.execute(code, stack);
	}
}