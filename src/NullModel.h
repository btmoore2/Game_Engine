#ifndef NULL_MODEL_H
#define NULL_MODEL_H

#include "Model.h"
namespace EngineSpace
{
	class NullModel : public Model
	{
	public:
		NullModel();
		NullModel(const NullModel&) = delete;
		NullModel& operator= (const NullModel&) = delete;

	private:
		virtual void privCreateVAO(const char* const modelFileName) override;


	};

}

#endif
