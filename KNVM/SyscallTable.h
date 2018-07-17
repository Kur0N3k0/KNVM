#pragma once

#include "Memory.h"
#include "Register.h"
#include "PipeLine.h"
#include "types.h"

#include <cstdio>
#include <Windows.h>

#pragma comment(lib, "ws2_32.lib")

namespace KNVM {
#pragma pack(push, 1)
	typedef struct Syscall {
		using Callback = void(*)(DispatchInfo *, RegisterList<> &, Memory &);
		BYTE interrupt;
		Callback callback;
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

	namespace KNVM_SYSCALL {
		void syscall_exit(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack);
		void syscall_read(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack);
		void syscall_write(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack);
		void syscall_flush(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack);
		void syscall_system(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack);
		void syscall_socket(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack);
		void syscall_listen(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack);
		void syscall_bind(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack);
		void syscall_recv(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack);
		void syscall_send(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack);
		void syscall_closesock(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack);
		void syscall_getthread(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack);
		void syscall_setthread(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack);
		void syscall_mode(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack);
		void syscall_privilege(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack);
		void syscall_taskswitch(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack);
	}

	static Syscall SyscallTable[0xff] = {
		{ INT_EXIT, KNVM_SYSCALL::syscall_exit },
		{ INT_READ, KNVM_SYSCALL::syscall_read },
		{ INT_WRITE, KNVM_SYSCALL::syscall_write },
		{ INT_FLUSH, KNVM_SYSCALL::syscall_flush },
		{ INT_SYSTEM, KNVM_SYSCALL::syscall_system },
		{ INT_LISTEN, KNVM_SYSCALL::syscall_listen },
		{ INT_BIND, KNVM_SYSCALL::syscall_bind },
		{ INT_RECV, KNVM_SYSCALL::syscall_recv },
		{ INT_SEND, KNVM_SYSCALL::syscall_send },
		{ INT_CLOSESOCK, KNVM_SYSCALL::syscall_closesock },
		{ INT_GETTHREAD, KNVM_SYSCALL::syscall_getthread },
		{ INT_SETTHREAD, KNVM_SYSCALL::syscall_setthread },
		{ INT_MODE, KNVM_SYSCALL::syscall_mode },
		{ INT_PRIVILEGE, KNVM_SYSCALL::syscall_privilege },
		{ INT_TASKSWITCH, KNVM_SYSCALL::syscall_taskswitch },
		{ INT_USER1, KNVM_SYSCALL::syscall_exit },
		{ INT_USER2, KNVM_SYSCALL::syscall_exit },
		{ INT_USER3, KNVM_SYSCALL::syscall_exit },
		{ INT_USER4, KNVM_SYSCALL::syscall_exit }
	};
#pragma pack(pop)
}