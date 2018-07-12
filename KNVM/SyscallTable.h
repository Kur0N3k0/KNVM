#pragma once

#include <Windows.h>

namespace KNVM {
	// OS syscalls
	const BYTE INT_EXIT			= 0x00;
	const BYTE INT_READ			= 0x01;
	const BYTE INT_WRITE		= 0x02;
	const BYTE INT_FLUSH		= 0x03;
	const BYTE INT_SYSTEM		= 0x04;
	const BYTE INT_SOCKET		= 0x05;
	const BYTE INT_LISTEN		= 0x06;
	const BYTE INT_BIND			= 0x07;
	const BYTE INT_RECV			= 0x08;
	const BYTE INT_SEND			= 0x09;
	const BYTE INT_CLOSESOCK	= 0x0a;
	const BYTE INT_GETTHREAD	= 0x0b;
	const BYTE INT_SETTHREAD	= 0x0c;
	const BYTE INT_MODE			= 0x0d;
	const BYTE INT_PRIVILEGE	= 0x0e;
	const BYTE INT_TASKSWITCH	= 0x0f;
	const BYTE INT_USER1		= 0x10;
	const BYTE INT_USER2		= 0x11;
	const BYTE INT_USER3		= 0x12;
	const BYTE INT_USER4		= 0x13;
}