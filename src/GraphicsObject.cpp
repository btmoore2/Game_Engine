#include <assert.h>
#include "GraphicsObject.h"

namespace EngineSpace
{
	GraphicsObject::GraphicsObject(const Model* const newModel, const ShaderObject* const newShader)
		: pModel(newModel), pShaderObj(newShader)
	{
		this->pWorld = new Matrix(Matrix::Special::Identity);
		assert(this->pWorld);
		assert(newModel);
		assert(newShader);
	}

	GraphicsObject::~GraphicsObject()
	{
		delete this->pWorld;
	}

	void GraphicsObject::Render()
	{
		this->setState();
		this->setDataGPU();
		this->Draw();
		this->restoreState();
	}

	const Model* GraphicsObject::getModel() const
	{
		return this->pModel;
	}

	Matrix& GraphicsObject::getWorld()
	{
		return *this->pWorld;
	}

	void GraphicsObject::setWorld(Matrix& newWorld)
	{
		*this->pWorld = newWorld;
	}
}