#pragma once

#include <iostream>
#include <Windows.h>

#include "types.h"
#include "Optable.h"

#include "Asm.h"
#include "Memory.h"
#include "CPU.h"
#include "Disassembler.h"

namespace KNVM {
	class KNVM {
	private:
		CPU cpu;

	public:
		void test() {
			Memory code(0x1000, PAGE_READWRITE, 4);
			Memory stack(0x1000, PAGE_READWRITE, 4);
			memset(code.get(), 0, 0x1000);
			memset(code.get(), 0, 0x1000);
			this->cpu.execute(code, stack);
		}
		auto getReg() {
			return this->cpu.getRegister();
		}
	};
}