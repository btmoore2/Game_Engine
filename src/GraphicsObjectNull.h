#ifndef GRAPHICS_OBJECT_NULL_H
#define GRAPHICS_OBJECT_NULL_H

#include "GraphicsObject.h"

namespace EngineSpace
{
	class GraphicsObjectNull : public GraphicsObject
	{
	public:
		GraphicsObjectNull() = delete;
		GraphicsObjectNull(const GraphicsObjectNull&) = delete;
		GraphicsObjectNull& operator= (const GraphicsObjectNull&) = delete;

		GraphicsObjectNull(const Model* const newModel, const ShaderObject* const newShader);

		virtual void setState() override;
		virtual void setDataGPU() override;
		virtual void Draw() override;
		virtual void restoreState() override;
	};
}


#endif
