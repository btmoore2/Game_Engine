#include "GraphicsObjectSkinTexture.h"
#include <assert.h>
#include "CameraManager.h"
#include "SkinModel.h"
#include "TextureManager.h"

namespace EngineSpace
{
	GraphicsObjectSkinTexture::GraphicsObjectSkinTexture(const Model* const newModel, const ShaderObject* const newShader, Texture::Name newTex, Vect& newLightColor, Vect& newLightPos)
		:GraphicsObjectSkin(newModel, newShader), texName(newTex)
	{
		this->pLightColor = new Vect(newLightColor);
		this->pLightPos = new Vect(newLightPos);
		assert(pLightColor);
		assert(pLightPos);

		TextureManager::useTex(this->texName);
	}

	GraphicsObjectSkinTexture::~GraphicsObjectSkinTexture()
	{
		delete this->pLightColor;
		delete this->pLightPos;	
	}

	void GraphicsObjectSkinTexture::setState()
	{
		glActiveTexture(GL_TEXTURE0);

		// Bind the texture
		GLuint textureID = TextureManager::findID(this->texName);
		glBindTexture(GL_TEXTURE_2D, textureID);

		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CCW);
	}

	void GraphicsObjectSkinTexture::setDataGPU()
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
		glUniform1i(this->pShaderObj->getLocation("texObject"), 0);

		glUniform3fv(this->pShaderObj->getLocation("vLightPos"), 1, (float*)this->pLightPos);
		glUniform4fv(this->pShaderObj->getLocation("vColor"), 1, (float*)this->pLightColor);
	}

	void GraphicsObjectSkinTexture::Draw()
	{
		glDrawElements(GL_TRIANGLES, 3 * this->getModel()->numTris, GL_UNSIGNED_INT, 0);
	}

	void GraphicsObjectSkinTexture::restoreState()
	{
		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CW);
	}

}
