#pragma once

#include "types.h"
#include "Memory.h"
#include "Register.h"
#include "FuncException.h"
#include "SyscallTable.h"

#include <Windows.h>

namespace KNVM {
	class Handler {
	private:
		DWORD mov(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack);
		DWORD push(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack);
		DWORD pop(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack);
		DWORD ret(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack);

		DWORD add(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack);
		DWORD sub(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack);
		DWORD mul(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack);
		DWORD div(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack);

		DWORD and(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack);
		DWORD or(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack);
		DWORD xor(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack);

		DWORD jmp(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack);
		DWORD je(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack);
		DWORD jne(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack);
		DWORD ja(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack);
		DWORD jb(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack);
		DWORD jl(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack);
		DWORD jle(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack);
		DWORD jz(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack);

		DWORD cmp(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack);
		DWORD test(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack);

		DWORD exit(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack);

	private:
		FuncException fnExp;

		DWORD add_except(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack);
		DWORD del_except(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack);
		DWORD call_except(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack);

	private:
		DWORD syscall(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack);

	private: 
		/*
		flags
		7 6 5 4 3 2 1 0		7		6		   5		  4		  3		 2 1 0
		D N O C Z - - -		[Debug] [Negative] [Overflow] [Carry] [Zero] [reserved 0-2]
		*/
		inline void setZF(RegisterList<> &reg);
		inline bool getZF(RegisterList<> &reg);
		inline void setCF(RegisterList<> &reg);
		inline bool getCF(RegisterList<> &reg);
		inline void setOF(RegisterList<> &reg);
		inline bool getOF(RegisterList<> &reg);
		inline void setNF(RegisterList<> &reg);
		inline bool getNF(RegisterList<> &reg);
		inline void setDF(RegisterList<> &reg);
		inline bool getDF(RegisterList<> &reg);

	public:
		DWORD handle(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack);
	};
}