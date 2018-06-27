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
			Memory mem(0x1000, PAGE_EXECUTE_READ, 4);
			this->cpu.execute(mem);
		}
		auto getReg() {
			return this->cpu.getRegister();
		}
	};
}