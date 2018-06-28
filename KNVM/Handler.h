#pragma once

#include "types.h"
#include "Memory.h"
#include "Register.h"
#include "FuncException.h"

namespace KNVM {
	class Handler {
	private:
		void mov(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack);
		void push(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack);
		void pop(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack);
		void ret(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack);

		void add(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack);
		void sub(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack);
		void mul(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack);
		void div(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack);

		void and(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack);
		void or(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack);
		void xor(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack);

		void jmp(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack);
		void je(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack);
		void jne(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack);
		void ja(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack);
		void jb(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack);
		void jl(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack);
		void jle(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack);
		void jz(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack);

	private:
		FuncException fnExp;

		void add_except(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack);
		void del_except(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack);
		void call_except(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack);

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
		void handle(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack);
	};
}