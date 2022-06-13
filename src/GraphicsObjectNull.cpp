#include "GraphicsObjectNull.h"

namespace EngineSpace
{
	GraphicsObjectNull::GraphicsObjectNull(const Model* const newModel, const ShaderObject* const newShader)
		:GraphicsObject(newModel, newShader)
	{

	}

	//Do nothing because this is a null object

	void GraphicsObjectNull::setState()
	{
		
	}

	void GraphicsObjectNull::setDataGPU()
	{

	}

	void GraphicsObjectNull::Draw()
	{

	}

	void GraphicsObjectNull::restoreState()
	{

	}

}