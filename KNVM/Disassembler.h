#pragma once

#include "CPU.h"
#include "Memory.h"
#include <string>
#include <sstream>
#include <iomanip>

namespace KNVM {
	class Disassembler {
	private:
		std::string mov(DispatchInfo *dpinfo);
		std::string push(DispatchInfo *dpinfo);
		std::string pop(DispatchInfo *dpinfo);
		std::string ret(DispatchInfo *dpinfo);

		std::string add(DispatchInfo *dpinfo);
		std::string sub(DispatchInfo *dpinfo);
		std::string mul(DispatchInfo *dpinfo);
		std::string div(DispatchInfo *dpinfo);

		std::string and(DispatchInfo *dpinfo);
		std::string or (DispatchInfo *dpinfo);
		std::string xor(DispatchInfo *dpinfo);

		std::string jmp(DispatchInfo *dpinfo);
		std::string je(DispatchInfo *dpinfo);
		std::string jne(DispatchInfo *dpinfo);
		std::string ja(DispatchInfo *dpinfo);
		std::string jb(DispatchInfo *dpinfo);
		std::string jl(DispatchInfo *dpinfo);
		std::string jle(DispatchInfo *dpinfo);
		std::string jz(DispatchInfo *dpinfo);

	private:
		FuncException fnExp;

		std::string add_except(DispatchInfo *dpinfo);
		std::string del_except(DispatchInfo *dpinfo);
		std::string call_except(DispatchInfo *dpinfo);

	private:
		std::string hex(DWORD value) {
			std::stringstream ss;
			ss << "0x" << std::setfill('0') << std::setw(8) << std::hex << value;
			return ss.str();
		}
		std::string join(std::string l) { return l + "\n"; }
		std::string join(std::string l, std::string r) { return l + ", " + r + "\n"; }

	private:
		CPU cpu;
		Memory &code;
	public:
		Disassembler(Memory &code) : code(code) { }
		std::string disassemble();
	};
}