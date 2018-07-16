#pragma once

#include "Memory.h"
#include "Register.h"
#include "types.h"

#include <Windows.h>

namespace KNVM {
#pragma pack(push, 1)
	typedef struct Syscall {
		const BYTE interrupt;
		void(*callback)(DispatchInfo *, RegisterList<> &, Memory &);
	} Syscall;

	// OS syscalls
	typedef enum {
		INT_EXIT = 0x00,
		INT_READ = 0x01,
		INT_WRITE = 0x02,
		INT_FLUSH = 0x03,
		INT_SYSTEM = 0x04,
		INT_SOCKET = 0x05,
		INT_LISTEN = 0x06,
		INT_BIND = 0x07,
		INT_RECV = 0x08,
		INT_SEND = 0x09,
		INT_CLOSESOCK = 0x0a,
		INT_GETTHREAD = 0x0b,
		INT_SETTHREAD = 0x0c,
		INT_MODE = 0x0d,
		INT_PRIVILEGE = 0x0e,
		INT_TASKSWITCH = 0x0f,
		INT_USER1 = 0x10,
		INT_USER2 = 0x11,
		INT_USER3 = 0x12,
		INT_USER4 = 0x13
	} SyscallIndex;

	Syscall SyscallTable[] = {
		{ INT_EXIT, NULL },
		{ INT_READ, NULL },
		{ INT_WRITE, NULL },
		{ INT_FLUSH, NULL },
		{ INT_SYSTEM, NULL },
		{ INT_LISTEN, NULL },
		{ INT_BIND, NULL },
		{ INT_RECV, NULL },
		{ INT_SEND, NULL },
		{ INT_CLOSESOCK, NULL },
		{ INT_GETTHREAD, NULL },
		{ INT_SETTHREAD, NULL },
		{ INT_MODE, NULL },
		{ INT_PRIVILEGE, NULL },
		{ INT_TASKSWITCH, NULL },
		{ INT_USER1, NULL },
		{ INT_USER2, NULL },
		{ INT_USER3, NULL },
		{ INT_USER4, NULL }
	};

#pragma pack(pop)
}