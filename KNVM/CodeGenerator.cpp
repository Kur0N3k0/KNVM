#include "CodeGenerator.h"

namespace KNVM {
	void CodeGenerator::Prefix(BYTE *opcode, DWORD size) {
		switch (opcode[0]) {
		case 0x26: 
			break;

		}
	}
	void CodeGenerator::ImmediateGrp(BYTE *opcode, DWORD size) {

	}
	void CodeGenerator::Grp(BYTE *opcode, DWORD size, BYTE id) {

	}
	void CodeGenerator::ShiftGrp(BYTE *opcode, DWORD size) {

	}
	void CodeGenerator::UnaryGrp(BYTE *opcode, DWORD size) {

	}
	void CodeGenerator::Default(BYTE *opcode, DWORD size) {
		x86Inst::ModRM modrm;
		std::string regmap[] = { "eax", "ecx", "edx", "ebx", "esp", "ebp", "esi", "edi" };
		std::string rm00map[] = { "eax", "ecx", "edx", "ebx", "", "", "esi", "edi" };
		std::string rm01map[] = { "eax", "ecx", "edx", "ebx", "", "ebp", "esi", "edi" };
		std::string rm10map[] = { "eax", "ecx", "edx", "ebx", "", "ebp", "esi", "edi" };
		std::string rm11map[] = { "eax", "ecx", "edx", "ebx", "esp", "ebp", "esi", "edi" };

		switch (opcode[0]) {
		case 0x00:
			memcpy(&modrm, &opcode[1], sizeof(modrm));
			
			switch (modrm.mod) {
			case 0b00:
				if (rm00map[modrm.rm] != "") {
					int i;
					for (i = modrm.rm + 1; i < 8 || rm00map[i] == ""; i++);
					std::string &creg = rm00map[i % 8];
					vAsm.push_back(Asm(OP_PUSH, creg));
					vAsm.push_back(Asm(OP_MOV, creg, regmap[modrm.reg]));
					vAsm.push_back(Asm(OP_AND, creg, 0x000000ff));
					vAsm.push_back(Asm(OP_ADD, "[" + rm00map[modrm.rm] + "]", creg));
					vAsm.push_back(Asm(OP_POP, creg));
				}
				else {
					// [--][--], disp32

				}
				break;
			case 0b01:
				if (rm01map[modrm.rm] != "") {
					int i;
					for (i = modrm.rm + 1; i < 8 || rm01map[i] == ""; i++);
					std::string &creg = rm01map[i % 8];
					vAsm.push_back(Asm(OP_PUSH, creg));
					vAsm.push_back(Asm(OP_MOV, creg, rm01map[modrm.reg]));
					vAsm.push_back(Asm(OP_AND, creg, 0x000000ff));
					vAsm.push_back(Asm(OP_ADD, creg, opcode[2]));
					vAsm.push_back(Asm(OP_ADD, "[" + rm01map[modrm.reg] + "]", creg));
					vAsm.push_back(Asm(OP_POP, creg));
				}
				else {
					// [--][--]


				}
				break;
			case 0b10:
				if (rm10map[modrm.rm] != "") {
					int i;
					for (i = modrm.rm + 1; i < 8 || rm10map[i] == ""; i++);
					std::string &creg = rm10map[i % 8];
					vAsm.push_back(Asm(OP_PUSH, creg));
					vAsm.push_back(Asm(OP_MOV, creg, rm10map[modrm.reg]));
					vAsm.push_back(Asm(OP_AND, creg, 0x000000ff));
					vAsm.push_back(Asm(OP_ADD, creg, *(DWORD *)&opcode[2]));
					vAsm.push_back(Asm(OP_ADD, "[" + rm10map[modrm.reg] + "]", creg));
					vAsm.push_back(Asm(OP_POP, creg));
				}
				else {
					// [--][--]

				}
				break;
			case 0b11:
			{
				int i, k;
				for (i = 0; i < 8; i++) {
					if (rm11map[i] == "esp") continue;
					if (rm11map[i] == rm11map[modrm.rm]) continue;
					if (rm11map[i] == rm11map[modrm.reg]) continue;
					break;
				}
				for (k = 0; k < 8; k++) {
					if (rm11map[k] == "esp") continue;
					if (rm11map[k] == rm11map[modrm.rm]) continue;
					if (rm11map[k] == rm11map[modrm.reg]) continue;
					if (rm11map[k] == rm11map[i]) continue;
					break;
				}

				std::string &creg = rm11map[i];
				std::string &creg2 = rm11map[k];

				vAsm.push_back(Asm(OP_PUSH, creg));
				vAsm.push_back(Asm(OP_PUSH, creg2));

				vAsm.push_back(Asm(OP_MOV, creg, rm11map[modrm.rm]));
				vAsm.push_back(Asm(OP_OR, creg, 0xffffff00));
				vAsm.push_back(Asm(OP_MOV, creg2, rm11map[modrm.reg]));
				vAsm.push_back(Asm(OP_AND, creg2, 0x000000ff));
				vAsm.push_back(Asm(OP_ADD, creg, creg2));
				vAsm.push_back(Asm(OP_AND, creg, 0x000000ff));
				vAsm.push_back(Asm(OP_AND, rm11map[modrm.rm], 0xffffff00));
				vAsm.push_back(Asm(OP_OR, rm11map[modrm.rm], creg));

				vAsm.push_back(Asm(OP_POP, creg2));
				vAsm.push_back(Asm(OP_POP, creg));
			}
				break;
			}
			break;

		case 0x01:
			memcpy(&modrm, &opcode[1], sizeof(modrm));

			switch (modrm.mod) {
			case 0b00:
				if (rm00map[modrm.rm] != "") {

				}
				else {

				}
				break;
			case 0b01:
				if (rm01map[modrm.rm] != "") {

				}
				else {

				}
				break;
			case 0b10:
				if (rm10map[modrm.rm] != "") {

				}
				else {

				}
				break;
			case 0b11:

				break;
			}
			break;
		case 0x02:
		case 0x03:
		case 0x04:
		case 0x05:
			break;
		}
	}

	DWORD CodeGenerator::getVMCodeSize() {
		DWORD result = 0;
		for (auto iter = vAsm.begin(); iter != vAsm.end(); iter++)
			result += iter->codesize;
		return result;
	}
	void CodeGenerator::getModRM(x86Inst::ModRM &modrm) {
		
	}

	CodeGenerator::CodeGenerator() {
		std::ifstream jInst("x86_64.json");
		if (!jInst.is_open()) {
			std::cout << __FUNCTION__ << " : " << "not open x86 instruction json file" << std::endl;
			getchar();
			exit(-1);
		}

		Json::CharReaderBuilder builder;
		builder["collectComments"] = false;

		Json::Value root;
		std::string err;
		if (Json::parseFromStream(builder, jInst, &root, &err) == false) {
			std::cout << __FUNCTION__ << " : " << err << std::endl;
			getchar();
			exit(-1);
		}
		
		auto instructions = root["instructions"];
		for (auto instIter = instructions.begin(); instIter != instructions.end(); instIter++) {
			if (instIter->isObject()) {
				std::cout << "object" << std::endl;
				std::cout << instIter.name() << std::endl;
				auto names = instIter->getMemberNames();
				for (auto iter = names.begin(); iter != names.end(); iter++)
					std::cout << *iter << std::endl;
			 }
			else if(instIter->isString()) {
				std::cout << "string" << std::endl;
			}
			//std::cout << instIter->asString() << std::endl;
		}
	}

	CodeGenerator::CodeGenerator(BYTE *bytecode, DWORD size) {
		BYTE *ptr = bytecode;

		while (ptr < bytecode + size) {
			switch (bytecode[0]) {
			// Prefix
			case 0x26:
			case 0x2e:
			case 0x3e:
			case 0x36:
			case 0x64:
			case 0x65:
			case 0x66:
			case 0x67:
				//x86Inst::Instruction &instruction
				Prefix(bytecode, size);
				ptr += inst.opsize;
				break;

			// Immediate Grp
			case 0x80:
			case 0x81:
			case 0x82:
			case 0x83:
				ImmediateGrp(bytecode, size);
				ptr += inst.opsize;
				break;

			// Grp 1A
			case 0x8f:
				Grp(bytecode, size, 0x1a);
				ptr += inst.opsize;
				break;

			// Shift Grp
			case 0xc0:
			case 0xc1:
			case 0xd0:
			case 0xd1:
			case 0xd2:
			case 0xd3:
				ShiftGrp(bytecode, size);
				ptr += inst.opsize;
				break;

			// Grp 11
			case 0xc6:
			case 0xc7:
				Grp(bytecode, size, 0x11);
				ptr += inst.opsize;
				break;

			// Unary Grp
			case 0xf6:
			case 0xf7:
				UnaryGrp(bytecode, size);
				ptr += inst.opsize;
				break;

			// Grp 4
			case 0xfe:
				Grp(bytecode, size, 0x04);
				ptr += inst.opsize;
				break;

			// Grp 5
			case 0xff:
				Grp(bytecode, size, 0x05);
				ptr += inst.opsize;
				break;

			default:
				Default(bytecode, size);
				ptr += inst.opsize;
				break;
			}
		}
	}
}