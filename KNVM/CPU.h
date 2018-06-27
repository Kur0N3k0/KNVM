#pragma once

#include "Register.h"
#include "Memory.h"
#include "Optable.h"
#include "types.h"
#include "Handler.h"

#include <vector>
#include <algorithm>

#include <Windows.h>

namespace KNVM {
	class CPU {
	private:
		uint16_t flags = 0;
		RegisterList<> reg = { "eax", "ebx", "ecx", "edx", "esi", "edi", "esp", "ebp", "eip" };
		Handler handler;

	public:
		void execute(Memory &code);
		DispatchInfo *dispatch(Register<> &pc, Memory &code);
		auto &getRegister() { return reg; }
	};
}