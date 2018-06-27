#include "types.h"
#include "Handler.h"

namespace KNVM {
	void Handler::handle(DispatchInfo *dpinfo) {
		if (dpinfo == nullptr)
			return;
		
		switch (dpinfo->opcode) {
			
		}
	}
}