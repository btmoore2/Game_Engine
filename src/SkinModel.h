#ifndef SKIN_MODEL_H
#define SKIN_MODEL_H

#include "Model.h"
#include "SSBO.h"
#include "MathEngine.h"
#include "TextureManager.h"

namespace EngineSpace
{
	class SkinModel : public Model
	{
	public:
		SkinModel() = delete;
		SkinModel(const SkinModel&) = delete;
		SkinModel& operator= (const SkinModel&) = delete;
		virtual ~SkinModel();

		SkinModel(const char* const pModelFileName);
		SkinModel(const char* const pModelFileName, Texture::Name texName);

		unsigned int numBones;
		SSBO* poInversePoseSSBO;
		Matrix* poBone;

	private:
		virtual void privCreateVAO(const char* const pModelFileName) override;
		void privCreateTexture(const char* const pModelFileName, Texture::Name texName);

		// Data
		GLuint vboboneweight;   // Bone_weight
		GLuint vboboneindex;   // Bone_index

	};
}


#endif
