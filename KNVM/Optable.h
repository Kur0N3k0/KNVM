#pragma once

#include <Windows.h>

namespace KNVM {
	/*
	Opcode Type Table
	*/
	const BYTE OP_TYPE_IMM = 0;
	const BYTE OP_TYPE_REG = 1;
	const BYTE OP_TYPE_REG2 = 2;
	const BYTE OP_TYPE_IMM2 = 3;

	/*
	Opcode Table
	*/
	const BYTE OP_PUSH = 0x00;
	const BYTE OP_POP = 0x01;
	const BYTE OP_MOV = 0x02;
	const BYTE OP_RET = 0x03;
	
	const BYTE OP_ADD = 0x04;
	const BYTE OP_SUB = 0x05;
	const BYTE OP_MUL = 0x06;
	const BYTE OP_DIV = 0x07;

	const BYTE OP_AND = 0x08;
	const BYTE OP_OR = 0x09;
	const BYTE OP_XOR = 0x0a;

	const BYTE OP_JMP = 0x0b;
	const BYTE OP_JE = 0x0c;
	const BYTE OP_JNE = 0x0d;
	const BYTE OP_JA = 0x0e;
	const BYTE OP_JB = 0x0f;
	const BYTE OP_JL = 0x10;
	const BYTE OP_JLE = 0x11;
	const BYTE OP_JZ = 0x12;

	const BYTE OP_CMP = 0x13;
	const BYTE OP_TEST = 0x14;

	const BYTE OP_EXIT = 0x3f; // 0b00111111

	const BYTE OP_ADD_EXCEPT = 0xde;
	const BYTE OP_DEL_EXCEPT = 0xad;
	//const BYTE OP_MOD_EXCEPT = 0xbe;
	const BYTE OP_EXCEPT_CALL = 0xef;
}