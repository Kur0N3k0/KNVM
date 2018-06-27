#pragma once

#include "types.h"

namespace KNVM {
	class Handler {
	private:
		void mov();
		void push();
		void pop();
		void ret();

		void add();
		void sub();
		void mul();
		void div();

		void and();
		void or ();
		void xor();

	public:
		void handle(DispatchInfo *dpinfo);
	};
}