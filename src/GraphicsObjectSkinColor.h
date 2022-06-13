#ifndef GRAPHICS_OBJECT_SKIN_COLOR_H
#define GRAPHICS_OBJECT_SKIN_COLOR_H

#include "GraphicsObjectSkin.h"

namespace EngineSpace
{
	class GraphicsObjectSkinColor : public GraphicsObjectSkin
	{
	public:
		GraphicsObjectSkinColor() = delete;
		GraphicsObjectSkinColor(const GraphicsObjectSkinColor&) = delete;
		GraphicsObjectSkinColor& operator= (const GraphicsObjectSkinColor&) = delete;
		~GraphicsObjectSkinColor();

		GraphicsObjectSkinColor(const Model* const newModel, const ShaderObject* const newShader, Vect& LightColor, Vect& LightPos);

		virtual void setState() override;
		virtual void setDataGPU() override;
		virtual void Draw() override;
		virtual void restoreState() override;

		Vect* pLightColor;
		Vect* pLightPos;
	};

}

#endif
