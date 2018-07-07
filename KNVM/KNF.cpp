#include "KNF.h"

namespace KNVM {
	bool KNFBuilder::build(const char *path) {
		std::ofstream file(path, std::ios::out | std::ios::binary);
		
		std::memcpy(header.signature, ".KNF", 4);
		file.write((char *)&header, sizeof(header));
		file.seekp(OFFSET_CODE);

		file.write((char *)code.get(), code.getCodeSize());
		file.seekp(OFFSET_DATA);

		file.write((char *)data.get(), data.getDataSize());

		file.close();

		return true;
	}
}