#pragma once

#include "CPU.h"
#include "Memory.h"
#include "types.h"

#include <string>
#include <sstream>
#include <iomanip>

namespace KNVM {
	class Disassembler {
	private:
		DisassembleInfo mov(DispatchInfo *dpinfo);
		DisassembleInfo push(DispatchInfo *dpinfo);
		DisassembleInfo pop(DispatchInfo *dpinfo);
		DisassembleInfo ret(DispatchInfo *dpinfo);

		DisassembleInfo add(DispatchInfo *dpinfo);
		DisassembleInfo sub(DispatchInfo *dpinfo);
		DisassembleInfo mul(DispatchInfo *dpinfo);
		DisassembleInfo div(DispatchInfo *dpinfo);

		DisassembleInfo and (DispatchInfo *dpinfo);
		DisassembleInfo or (DispatchInfo *dpinfo);
		DisassembleInfo xor (DispatchInfo *dpinfo);

		DisassembleInfo jmp(DispatchInfo *dpinfo);
		DisassembleInfo je(DispatchInfo *dpinfo);
		DisassembleInfo jne(DispatchInfo *dpinfo);
		DisassembleInfo ja(DispatchInfo *dpinfo);
		DisassembleInfo jb(DispatchInfo *dpinfo);
		DisassembleInfo jl(DispatchInfo *dpinfo);
		DisassembleInfo jle(DispatchInfo *dpinfo);
		DisassembleInfo jz(DispatchInfo *dpinfo);

		DisassembleInfo cmp(DispatchInfo *dpinfo);
		DisassembleInfo test(DispatchInfo *dpinfo);

		DisassembleInfo exit(DispatchInfo *dpinfo);

	private:
		FuncException fnExp;

		DisassembleInfo add_except(DispatchInfo *dpinfo);
		DisassembleInfo del_except(DispatchInfo *dpinfo);
		DisassembleInfo call_except(DispatchInfo *dpinfo);

	private:
		DisassembleInfo syscall(DispatchInfo *dpinfo);

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