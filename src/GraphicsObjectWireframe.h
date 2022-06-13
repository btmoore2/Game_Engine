#ifndef GRAPHICS_OBJECT_WIREFRAME_H
#define GRAPHICS_OBJECT_WIREFRAME_H

#include "GraphicsObject.h"

namespace EngineSpace
{
	class GraphicsObjectWireframe : public GraphicsObject
	{
	public:
		GraphicsObjectWireframe() = delete;
		GraphicsObjectWireframe(const GraphicsObjectWireframe&) = delete;
		GraphicsObjectWireframe& operator= (const GraphicsObjectWireframe&) = delete;
		virtual ~GraphicsObjectWireframe();

		GraphicsObjectWireframe(const Model* const newModel, const ShaderObject* const newShader);

		virtual void setState() override;
		virtual void setDataGPU() override;
		virtual void Draw() override;
		virtual void restoreState() override;
	};
}


#endif
