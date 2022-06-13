#include "GraphicsObjectSkinColor.h"
#include <assert.h>
#include "CameraManager.h"
#include "SkinModel.h"

namespace EngineSpace
{
	GraphicsObjectSkinColor::GraphicsObjectSkinColor(const Model* const newModel, const ShaderObject* const newShader, Vect& LightColor, Vect& LightPos)
		:GraphicsObjectSkin(newModel, newShader)
	{
		this->pLightColor = new Vect(LightColor);
		this->pLightPos = new Vect(LightPos);
		assert(pLightColor);
		assert(pLightPos);
	}

	GraphicsObjectSkinColor::~GraphicsObjectSkinColor()
	{
		delete this->pLightColor;
		delete this->pLightPos;
	}

	void GraphicsObjectSkinColor::setState()
	{
		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CCW);
	}

	void GraphicsObjectSkinColor::setDataGPU()
	{
		//Activate shader and VAO;
		this->pShaderObj->setActive();

		SkinModel* pSkinModel = (SkinModel*)this->getModel();
		glBindVertexArray(pSkinModel->vao);


		Matrix view = CameraManager::getCurrCam(Camera::Type::PERSPECTIVE_3D)->getViewMatrix();
		Matrix proj = CameraManager::getCurrCam(Camera::Type::PERSPECTIVE_3D)->getProjMatrix();

		pSkinModel->poInversePoseSSBO->Bind(0);
		this->pBoneWorldSSBO->Bind(1);

		glUniformMatrix4fv(this->pShaderObj->getLocation("projMatrix"), 1, GL_FALSE, (float*)&proj);
		glUniformMatrix4fv(this->pShaderObj->getLocation("viewMatrix"), 1, GL_FALSE, (float*)&view);
		glUniformMatrix4fv(this->pShaderObj->getLocation("localMatrix"), 1, GL_FALSE, (float*)pSkelHead->getWorld());

		Vect light = *this->pLightPos;
		Vect color = *this->pLightColor;
		glUniform3fv(this->pShaderObj->getLocation("vLightPos"), 1, (float*)&light);
		glUniform4fv(this->pShaderObj->getLocation("vColor"), 1, (float*)&color);

		//glUniform3fv(this->pShaderObj->getLocation("vLightPos"), 1, (float*)this->pLightPos);
		//glUniform4fv(this->pShaderObj->getLocation("vColor"), 1, (float*)this->pLightColor);
	}

	void GraphicsObjectSkinColor::Draw()
	{
		glDrawElements(GL_TRIANGLES, 3 * this->getModel()->numTris, GL_UNSIGNED_INT, 0);
	}

	void GraphicsObjectSkinColor::restoreState()
	{
		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CW);
	}

}
