#pragma once

#include "types.h"
#include "Register.h"

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

		void add_except(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack);
		void del_except(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack);
		void mod_except(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack);
		void call_except(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack);

	public:
		void handle(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack);
	};
}