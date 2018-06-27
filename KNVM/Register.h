#pragma once

#include <vector>
#include <string>
#include <array>
#include <cstdint>

#include <Windows.h>

namespace KNVM {
	template<typename nType=std::string, DWORD default_val=0>
	class Register {
	private:
		nType name;
		DWORD val;
	public:
		Register(nType name, DWORD val=default_val) : name(name), val(val) { }
		DWORD get() const { return val; }
		void set(DWORD val) { this->val = val; }
		bool operator==(nType name) { return this->name == name; }
		bool operator!=(nType name) { return this->name != name; }
		bool operator==(DWORD val) { return this->val == val; }
		bool operator!=(DWORD val) { return this->val != val; }
		bool operator==(Register<nType, default_val> &reg) { return this->val == reg.val; }
		bool operator!=(Register<nType, default_val> &reg) { return this->val != reg.val; }
		bool operator>=(DWORD val) { return this->val >= val; }
		bool operator<=(DWORD val) { return this->val <= val; }
		bool operator>=(Register<nType, default_val> & val) { return this->val >= val.val; }
		bool operator<=(Register<nType, default_val> & val) { return this->val <= val.val; }
		bool operator>(DWORD val) { return this->val > val; }
		bool operator<(DWORD val) { return this->val > val; }
		bool operator>(Register<nType, default_val> & val) { return this->val > val.val; }
		bool operator<(Register<nType, default_val> & val) { return this->val < val.val; }

		DWORD operator++() { return this->val++; }
		DWORD operator++(int) { return ++this->val; }
		DWORD operator--() { return this->val--; }
		DWORD operator--(int) { return --this->val; }

		DWORD operator+(DWORD val) { return this->val + val; }
		DWORD operator-(DWORD val) { return this->val - val; }
		DWORD operator*(DWORD val) { return this->val * val; }
		DWORD operator/(DWORD val) { return this->val / val; }
		DWORD operator&(DWORD val) { return this->val & val; }
		DWORD operator|(DWORD val) { return this->val | val; }
		DWORD operator^(DWORD val) { return this->val ^ val; }
		DWORD operator+(Register<nType, default_val> & val) { return this->val + val.val; }
		DWORD operator-(Register<nType, default_val> & val) { return this->val - val.val; }
		DWORD operator*(Register<nType, default_val> & val) { return this->val * val.val; }
		DWORD operator/(Register<nType, default_val> & val) { return this->val / val.val; }
		DWORD operator&(Register<nType, default_val> & val) { return this->val & val.val; }
		DWORD operator|(Register<nType, default_val> & val) { return this->val | val.val; }
		DWORD operator^(Register<nType, default_val> & val) { return this->val ^ val.val; }
		
		Register<nType, default_val> &operator+=(DWORD val) { this->val += val; return *this; }
		Register<nType, default_val> &operator-=(DWORD val) { this->val -= val; return *this; }
		Register<nType, default_val> &operator*=(DWORD val) { this->val *= val; return *this; }
		Register<nType, default_val> &operator/=(DWORD val) { this->val /= val; return *this; }
		Register<nType, default_val> &operator&=(DWORD val) { this->val &= val; return *this; }
		Register<nType, default_val> &operator|=(DWORD val) { this->val |= val; return *this; }
		Register<nType, default_val> &operator^=(DWORD val) { this->val ^= val; return *this; }
		
		Register<nType, default_val> &operator+=(Register<nType, default_val> & val) { this->val += val.val; return *this; }
		Register<nType, default_val> &operator-=(Register<nType, default_val> & val) { this->val -= val.val; return *this; }
		Register<nType, default_val> &operator*=(Register<nType, default_val> & val) { this->val *= val.val; return *this; }
		Register<nType, default_val> &operator/=(Register<nType, default_val> & val) { this->val /= val.val; return *this; }
		Register<nType, default_val> &operator&=(Register<nType, default_val> & val) { this->val &= val.val; return *this; }
		Register<nType, default_val> &operator|=(Register<nType, default_val> & val) { this->val |= val.val; return *this; }
		Register<nType, default_val> &operator^=(Register<nType, default_val> & val) { this->val ^= val.val; return *this; }

		Register<nType, default_val> &operator=(DWORD val) { this->val = val; return *this; }
		Register<nType, default_val> &operator=(Register<nType, default_val> &r) { this->val = r.val; return *this; }
		DWORD *operator*() { return (DWORD *)this->val; }
	};

	template<typename nType = std::string, uint32_t default_val = 0>
	class RegisterList {
	private:
		std::vector< Register<nType, default_val> > reg;
	
	public:
		RegisterList(std::initializer_list<nType> l) { reg.insert(reg.end(), l.begin(), l.end()); }
		Register<nType, default_val> &operator[](std::string regname) {
			auto result = std::find(reg.begin(), reg.end(), regname);
			if (result == reg.end())
				throw "Register::Unknown";
			return *result;
		}
		Register<nType, default_val> &operator[](BYTE regop) {
			DWORD idx = 0;
			if (regop != 0) {
				idx = 1;
				while (!(regop & 0b00000001)) {
					regop >>= 1;
					idx++;
				}
			}
			return reg[idx];
		}
	};
}