#include <iostream>
#include "KNVM.h"

using namespace std;

int main() {
	//KNVM::KNVM knvm;

	//knvm.test();

	KNVM::Memory code(100, PAGE_READWRITE, 4);
	KNVM::Disassembler disassembler(code);

	memset(code.get(), 0, 100);
	cout << disassembler.disassemble() << endl;

	int i;
	cin >> i;

	return 0;
}