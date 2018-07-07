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

		/*
		flags
		7 6 5 4 3 2 1 0		7		6		   5		  4		  3		 2 1 0
		D N O C Z - - -		[Debug] [Negative] [Overflow] [Carry] [Zero] [reserved 0-2]
		*/
		RegisterList<> reg = { "eax", "ebx", "ecx", "edx", "esi", "edi", "esp", "ebp", "eip", "flags" };
		Handler handler;

	private:
		void genTestcase(Memory &code);

	public:
		void execute(Memory &code, Memory &stack);
		void execute(Memory &code, void *entrypoint, Memory &stack);
		DispatchInfo *dispatch(Register<> &pc, Memory &code);
		auto &getRegister() { return reg; }
	};
}