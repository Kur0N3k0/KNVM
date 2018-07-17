#include "SyscallTable.h"

namespace KNVM {
	namespace KNVM_SYSCALL {
		/*
		* @argument (void)
		* @return $eax
		*/
		void _Private syscall_exit(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack) {
			reg["eax"].set(reg["ebx"].get());
			throw "Program Exit(Interrupt)";
		}

		/*
		* @argument (pipe, char *, uint32_t)
		* @return size
		*/
		void _Private syscall_read(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack) {
			DWORD type = reg["ebx"].get();
			char *ptr = (char *)reg["ecx"].get();
			DWORD size = reg["edx"].get();

			if (type == 0)
				kstdin.read(ptr, size);
			else if (type == 1)
				kstdout.read(ptr, size);

			reg["eax"].set(size);
		}

		/*
		* @argument (pipe, char *, uint32_t)
		* @return size
		*/
		void _Private syscall_write(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack) {
			DWORD type = reg["ebx"].get();
			char *ptr = (char *)reg["ecx"].get();
			DWORD size = reg["edx"].get();

			if (type == 0)
				kstdin.write(ptr, size);
			else if (type == 1)
				kstdout.write(ptr, size);

			reg["eax"].set(size);
		}

		/*
		* @argument (pipe)
		* @return void
		*/
		void _Private syscall_flush(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack) {
			DWORD type = reg["ebx"].get();
			char *ptr = (char *)reg["ecx"].get();
			DWORD size = reg["edx"].get();

			if (type == 0)
				kstdin.fflush();
			else
				kstdout.fflush();
		}

		/*
		* @argument (char *)
		* @return void
		*/
		void _Private syscall_system(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack) {
			char *ptr = (char *)reg["ebx"].get();
			
			FILE *file = _popen(ptr, "rt");
			if (file == NULL)
				return;

			char buf[1024] = { 0, };
			while (fgets(buf, 1023, file))
				kstdout.write(buf, strlen(buf));

			if (feof(file)) fclose(file);
		}

		/*
		* @argument (uint32_t AF_INET/PF_INET, uint32_t socktype)
		* @return socket
		*/
		void _Private syscall_socket(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack) {
			DWORD af = reg["ebx"].get();
			DWORD type = reg["ecx"].get();
			WSADATA wsa;
			if (WSAStartup(MAKEWORD(2, 2), &wsa) != NO_ERROR) 
				reg["eax"].set(NO_ERROR);
			else
				reg["eax"].set((DWORD)socket(af, type, 0));
		}

		/*
		* @argument (socket, uint32_t backlog)
		* @return bool
		*/
		void _Private syscall_listen(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack) {
			SOCKET sock = (SOCKET)reg["ebx"].get();
			DWORD backlog = reg["ecx"].get();
			if (listen(sock, backlog) != SOCKET_ERROR)
				reg["eax"].set(true);
			else
				reg["eax"].set(false);
		}

		/*
		* @argument (socket, uint32_t cnt)
		* @return bool
		*/
		void _Private syscall_bind(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack) {
			SOCKET sock = (SOCKET)reg["ebx"].get();
			SOCKADDR_IN *addr = (SOCKADDR_IN *)reg["ecx"].get();
			if (bind(sock, (SOCKADDR *)addr, sizeof(SOCKADDR_IN)) != SOCKET_ERROR)
				reg["eax"].set(true);
			else
				reg["eax"].set(false);
		}

		/*
		* @argument (socket, char *, uint32_t cnt)
		* @return size
		*/
		void _Private syscall_recv(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack) {
			SOCKET sock = (SOCKET)reg["ebx"].get();
			char *ptr = (char *)reg["ecx"].get();
			DWORD size = reg["edx"].get();

			reg["eax"].set(recv(sock, ptr, size, 0));
		}

		/*
		* @argument (socket, char *, uint32_t size)
		* @return size
		*/
		void _Private syscall_send(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack) {
			SOCKET sock = (SOCKET)reg["ebx"].get();
			char *ptr = (char *)reg["ecx"].get();
			DWORD size = reg["edx"].get();

			reg["eax"].set(send(sock, ptr, size, 0));
		}

		/*
		* @argument (socket)
		* @return void
		*/
		void _Private syscall_closesock(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack) {
			SOCKET sock = (SOCKET)reg["ebx"].get();
			closesocket(sock);
			WSACleanup();
		}

		/*
		* @argument (threadid)
		* @return threadinfo
		*/
		void _Private syscall_getthread(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack) {
			DWORD tid = reg["ebx"].get();
			ThreadInfoBlock *ptr = &tib;
			while (ptr) {
				if (ptr->tid == tid)
					break;
				ptr = ptr->next;
			}
			reg["eax"].set((DWORD)ptr);
		}

		/*
		* @argument (threadinfo)
		* @return void
		*/
		void _Private syscall_setthread(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack) {
			ThreadInfoBlock *target = (ThreadInfoBlock *)reg["ebx"].get();
			ThreadInfoBlock *ptr = &tib;
			while (ptr) {
				if (ptr->tid == target->tid)
					break;
				ptr = ptr->next;
			}
			if(ptr != NULL)
				memcpy(ptr, target, sizeof(ThreadInfoBlock));
		}

		/*
		* @argument (uint32_t mode)
		* @return prev_mode
		*/
		void _Private syscall_mode(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack) {

		}

		/*
		* @argument (uint32_t priv)
		* @return prev_priv
		*/
		void _Private syscall_privilege(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack) {

		}

		/*
		* @argument (uint32_t threadid)
		* @return socket
		*/
		void _Private syscall_taskswitch(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack) {

		}
	}
}