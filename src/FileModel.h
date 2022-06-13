#ifndef FILE_MODEL_H
#define FILE_MODEL_H

#include "Model.h"

namespace EngineSpace
{
	class FileModel : public Model
	{
	public:
		FileModel() = delete;
		FileModel(const FileModel&) = delete;
		FileModel& operator= (const FileModel&) = delete;
		virtual ~FileModel();

		FileModel(const char* const pModelFileName);

	private:
		virtual void privCreateVAO(const char* const pModelFileName) override;

	};
}


#endif
