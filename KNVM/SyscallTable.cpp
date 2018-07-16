#include "SyscallTable.h"

namespace KNVM {
	namespace KNVM_SYSCALL {
		/*
		* @argument (void)
		* @return $eax
		*/
		void _Private syscall_exit(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack) {
			throw "Program Exit";
		}

		/*
		* @argument (char *, uint32_t)
		* @return size
		*/
		void _Private syscall_read(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack) {
			
		}

		/*
		* @argument (char *, uint32_t)
		* @return size
		*/
		void _Private syscall_write(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack) {

		}

		/*
		* @argument (stdio)
		* @return size
		*/
		void _Private syscall_flush(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack) {

		}

		/*
		* @argument (char *)
		* @return void
		*/
		void _Private syscall_system(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack) {

		}

		/*
		* @argument (uint32_t AF_INET/PF_INET, uint32_t socktype)
		* @return socket
		*/
		void _Private syscall_socket(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack) {

		}

		/*
		* @argument (socket, uint32_t cnt)
		* @return bool
		*/
		void _Private syscall_listen(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack) {

		}

		/*
		* @argument (socket, uint32_t cnt)
		* @return bool
		*/
		void _Private syscall_bind(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack) {

		}

		/*
		* @argument (socket, char *, uint32_t cnt)
		* @return size
		*/
		void _Private syscall_recv(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack) {

		}

		/*
		* @argument (socket, char *, uint32_t size)
		* @return size
		*/
		void _Private syscall_send(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack) {

		}

		/*
		* @argument (socket)
		* @return void
		*/
		void _Private syscall_closesock(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack) {

		}

		/*
		* @argument (void)
		* @return threadinfo
		*/
		void _Private syscall_getthread(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack) {

		}

		/*
		* @argument (threadinfo)
		* @return void
		*/
		void _Private syscall_setthread(DispatchInfo *dpinfo, RegisterList<> &reg, Memory &stack) {

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