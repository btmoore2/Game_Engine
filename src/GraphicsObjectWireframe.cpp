#include "GraphicsObjectWireframe.h"
#include <assert.h>
#include "CameraManager.h"

namespace EngineSpace
{
	GraphicsObjectWireframe::GraphicsObjectWireframe(const Model* const newModel, const ShaderObject* const newShader)
		: GraphicsObject(newModel, newShader)
	{
		assert(pModel);
		assert(pShaderObj);
		assert(pWorld);
	}

	GraphicsObjectWireframe::~GraphicsObjectWireframe()
	{

	}

	void GraphicsObjectWireframe::setState()
	{
		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CW);
	}

	void GraphicsObjectWireframe::setDataGPU()
	{
		this->pShaderObj->setActive();

		glBindVertexArray(this->getModel()->vao);

		// Set state
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDisable(GL_CULL_FACE);

		Matrix world = this->getWorld();
		Matrix view = CameraManager::getCurrCam(Camera::Type::PERSPECTIVE_3D)->getViewMatrix();
		Matrix proj = CameraManager::getCurrCam(Camera::Type::PERSPECTIVE_3D)->getProjMatrix();
		Matrix mvMatrix = world * view;

		glUniformMatrix4fv(this->pShaderObj->getLocation("projMatrix"), 1, GL_FALSE, (float*)&proj);
		glUniformMatrix4fv(this->pShaderObj->getLocation("viewMatrix"), 1, GL_FALSE, (float*)&view);
		glUniformMatrix4fv(this->pShaderObj->getLocation("worldMatrix"), 1, GL_FALSE, (float*)&world);
	}

	void GraphicsObjectWireframe::Draw()
	{
		//glDrawArrays(GL_TRIANGLES, 0, (3 * this->getModel()->numVerts));

		glDrawElements(GL_TRIANGLES, 3 * this->getModel()->numTris, GL_UNSIGNED_INT, 0);
	}

	void GraphicsObjectWireframe::restoreState()
	{
		// Restore state
		glEnable(GL_CULL_FACE);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}