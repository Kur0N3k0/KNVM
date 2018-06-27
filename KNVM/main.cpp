#include <iostream>
#include "KNVM.h"

using namespace std;

int main() {
	KNVM::KNVM knvm;

	auto reg = knvm.getReg();
	cout << reg["eax"].get() << endl;
	reg["eax"].set(0x100);
	cout << reg["eax"].get() << endl;

	auto r2 = knvm.getReg();
	cout << reg["eax"].get() << endl;

	knvm.test();

	int i;
	cin >> i;

	return 0;
}