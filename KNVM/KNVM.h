#pragma once

#include <iostream>
#include <Windows.h>

#include "CPU.h"
#include "Register.h"

namespace KNVM {
	class KNVM {
	private:
		CPU cpu;

	public:
		void test() {
			Memory code(0x1000, PAGE_READWRITE, 4);
			Memory stack(0x1000, PAGE_READWRITE, 4);
			memset(code.get(), 0, 0x1000);
			memset(stack.get(), 0, 0x1000);
			this->cpu.execute(code, stack);
		}
		auto getReg() {
			return this->cpu.getRegister();
		}
	};
}