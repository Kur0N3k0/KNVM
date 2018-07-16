#pragma once

#include <iostream>
#include <fstream>
#include <Windows.h>

#include "types.h"
#include "Optable.h"

#include "Asm.h"
#include "Memory.h"
#include "PipeLine.h"
#include "CPU.h"
#include "Disassembler.h"
#include "KNF.h"

#include "UI.h"

namespace KNVM {
	class KNVM {
	private:
		CPU cpu;
		Memory *memory = nullptr;
		PipeLine pipe_in, pipe_out;
		void *entrypoint;

	public:
		KNVM() {
			memory = new Memory(MAX_MEMORY_SIZE, PAGE_READWRITE, sizeof(size_t));
			void *addr = memory->get();
			std::memset(addr, 0xcc, MAX_MEMORY_SIZE);
		}
		~KNVM() {
			if (memory)
				delete memory;
			memory = nullptr;
		}

		bool ParseBinary(KNF &knf, Memory &code, Memory &data);
		bool ParseBinary(const char *path);
		void Emulate();

		void test() {
			Memory code(0x1000, PAGE_READWRITE, 4);
			Memory stack(0x1000, PAGE_READWRITE, 4);
			memset(code.get(), 0, 0x1000);
			memset(stack.get(), 0, 0x1000);
			this->cpu.execute(code, stack);
		}
		void test2(Memory &code){
			Memory stack(0x1000, PAGE_READWRITE, 4);
			this->cpu.execute(code, stack);
		}
	};
}