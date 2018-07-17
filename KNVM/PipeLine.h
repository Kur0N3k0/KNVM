#pragma once

#include <iostream>

namespace KNVM {
	class IO {
	private:
		char *buffer = nullptr;
		bool enabled = true;
	public:
		~IO() {
			if (buffer)
				delete []buffer;
		}

		IO &open(size_t size = 65535) {
			if (buffer) delete[]buffer;
			buffer = new char[size];
			enabled = true;
			return *this;
		}
		void write(char *ptr, size_t size) {
			if (!enabled || buffer == nullptr)
				return;

			std::memcpy(buffer, ptr, size);
			buffer[size] = 0;
			std::cout << buffer;
		}
		int read(char *ptr, size_t size) {
			if (!enabled || buffer == nullptr)
				return -1;

			std::memcpy(buffer, ptr, size);
			buffer[size] = 0;
			std::cout << buffer;
			return size;
		}
		void close() {
			if (buffer)
				delete []buffer;
			buffer = nullptr;
			enabled = false;
		}
	};

	class PipeLine {
	private:
		IO io;

	public:
		PipeLine() { io.open(); }
		void write(char *ptr, size_t size) { io.write(ptr, size); }
		void read(char *ptr, size_t size) { io.read(ptr, size); }
		void fflush() {
			io.close();
			io.open();
		}
	};

	static PipeLine kstdout;
	static PipeLine kstdin;
}