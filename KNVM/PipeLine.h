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
		IO kstdin;
		IO kstdout;

	public:
		PipeLine(bool in = true, bool out = true) {
			if (in)
				kstdin.open();
			if (out)
				kstdout.open();
		}
		void write(char *ptr, size_t size) { kstdout.write(ptr, size); }
		void read(char *ptr, size_t size) { kstdin.read(ptr, size); }
		void fflush(IO &io) {
			io.close();
			io.open();
		}
	};
}