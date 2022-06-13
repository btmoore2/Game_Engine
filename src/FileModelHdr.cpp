#include "FileModelHdr.h"

namespace EngineSpace
{
	FileModelHdr::FileModelHdr()
	{
		//Clear data for this object since it is only for reading in.
		memset(this, 0x0, sizeof(FileModelHdr));
	}
}