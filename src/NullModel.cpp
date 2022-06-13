#include "NullModel.h"


namespace EngineSpace
{
	NullModel::NullModel()
		:Model()
	{

	}

	void NullModel::privCreateVAO(const char* const modelFileName)
	{
		SB7_UNUSED_VAR(modelFileName);
	}
}