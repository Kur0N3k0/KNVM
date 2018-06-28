#pragma once

#include <unordered_map>

#include <Windows.h>

namespace KNVM {
	class FuncException {
		using FuncMap = std::unordered_map<void *, void *>;
	private:
		std::unordered_map<void *, void *> func;
	public:
		void add_except(void *faddr) { func[faddr] = faddr; }
		void del_except(void *faddr) { func.erase(faddr); }
		bool is_func(void *faddr) { return func.find(faddr) != func.end(); }
	};
}