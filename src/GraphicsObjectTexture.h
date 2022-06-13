#ifndef GRAPHICS_OBJECT_TEXTURE
#define GRAPHICS_OBJECT_TEXTURE

#include "GraphicsObject.h"
#include "Texture.h"

namespace EngineSpace
{
	class GraphicsObjectTexture : public GraphicsObject
	{
	public:
		GraphicsObjectTexture() = delete;
		GraphicsObjectTexture(const GraphicsObjectTexture&) = delete;
		GraphicsObjectTexture& operator= (const GraphicsObjectTexture&) = delete;
		virtual ~GraphicsObjectTexture();

		GraphicsObjectTexture(const Model* const newModel, const ShaderObject* const newShader, Texture::Name newTex, const Vect& lightColor = Vect(1,1,1,1), const Vect& lightPos = Vect(0,0,0,1));

		virtual void setState() override;
		virtual void setDataGPU() override;
		virtual void Draw() override;
		virtual void restoreState() override;

		void switchTex(Texture::Name newTex);

		Texture::Name texName;
		Vect* pLightColor;
		Vect* pLightPos;
	};
}


#endif
