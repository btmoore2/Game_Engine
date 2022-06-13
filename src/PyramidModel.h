#ifndef PYRAMID_MODEL_H
#define PYRAMID_MODEL_H

#include "Model.h"

namespace EngineSpace
{
	class PyramidModel : public Model
	{
	public:
		PyramidModel() = delete;
		PyramidModel(const PyramidModel&) = delete;
		PyramidModel& operator= (const PyramidModel&) = delete;
		~PyramidModel();

		PyramidModel(const char* const pModelFileName);


	private:
		void privCreateVAO(const char* const pModelFileName) override;
	};
}


#endif
