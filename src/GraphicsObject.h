#ifndef GRAPHICS_OBJECT_H
#define GRAPHICS_OBJECT_H

#include "MathEngine.h"
#include "ShaderObject.h"
#include "Model.h"

namespace EngineSpace
{
	class GraphicsObject
	{
	public:
		enum class GOType
		{
			WIREFRAME,
			LITTEXTURE,
			FLATTEXTURE,
			CONSTCOLOR,
			COLORPOS,
			NULLTYPE
		};

		GraphicsObject() = delete;
		GraphicsObject(const GraphicsObject&) = delete;
		GraphicsObject& operator= (const GraphicsObject&) = delete;
		virtual ~GraphicsObject();

		GraphicsObject(const Model* const newModel, const ShaderObject* const newShader);

		void Render();
		void setWorld(Matrix& newWorld);

		virtual void setState() = 0;
		virtual void setDataGPU() = 0;
		virtual void Draw() = 0;
		virtual void restoreState() = 0;

		const Model* getModel() const;
		Matrix& getWorld();

	protected:
		const Model* pModel;
		Matrix* pWorld;
		const ShaderObject* pShaderObj;


	};
}

#endif
