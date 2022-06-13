#ifndef GRAPHICS_OBJECT_SKIN_TEXTURE_H
#define GRAPHICS_OBJECT_SKIN_TEXTURE_H

#include "GraphicsObjectSkin.h"
#include "Texture.h"

namespace EngineSpace
{
	class GraphicsObjectSkinTexture : public GraphicsObjectSkin
	{
	public:
		GraphicsObjectSkinTexture() = delete;
		GraphicsObjectSkinTexture(const GraphicsObjectSkinTexture&) = delete;
		GraphicsObjectSkinTexture& operator= (const GraphicsObjectSkinTexture&) = delete;
		virtual ~GraphicsObjectSkinTexture();

		GraphicsObjectSkinTexture(const Model* const newModel, const ShaderObject* const newShader, Texture::Name newTex, Vect& newLightColor, Vect& newLightPos);

		virtual void setState() override;
		virtual void setDataGPU() override;
		virtual void Draw() override;
		virtual void restoreState() override;

		Texture::Name texName;
		Vect* pLightColor;
		Vect* pLightPos;
	};
}


#endif
