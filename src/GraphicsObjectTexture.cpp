#include "GraphicsObjectTexture.h"
#include "TextureManager.h"
#include <assert.h>
#include "CameraManager.h"

namespace EngineSpace
{ 
	GraphicsObjectTexture::GraphicsObjectTexture(const Model* const newModel, const ShaderObject* const newShader, Texture::Name newTex, const Vect& lightColor, const Vect& lightPos)
		:GraphicsObject(newModel, newShader), texName(newTex)
	{
		this->pLightColor = new Vect(lightColor);
		this->pLightPos = new Vect(lightPos);
		
		assert(pLightColor);
		assert(pLightPos);
		assert(this->pModel);
		assert(this->pShaderObj);
		assert(this->pWorld);

		TextureManager::useTex(this->texName);
	}

	GraphicsObjectTexture::~GraphicsObjectTexture()
	{
		delete this->pLightColor;
		delete this->pLightPos;
		TextureManager::disuseTex(this->texName);
	}

	void GraphicsObjectTexture::switchTex(Texture::Name newTex)
	{
		TextureManager::disuseTex(this->texName);
		TextureManager::useTex(newTex);

		this->texName = newTex;
	}

	void GraphicsObjectTexture::setState()
	{
		glActiveTexture(GL_TEXTURE0);

		// Bind the texture
		GLuint textureID = TextureManager::findID(this->texName);
		glBindTexture(GL_TEXTURE_2D, textureID);

		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CW);
	}

	void GraphicsObjectTexture::setDataGPU()
	{
		//Activate shader and VAO;
		this->pShaderObj->setActive();
		glBindVertexArray(this->getModel()->vao);
	}

	void GraphicsObjectTexture::Draw()
	{
		glDrawElements(GL_TRIANGLES, 3 * this->getModel()->numTris, GL_UNSIGNED_INT, 0);
	}

	void GraphicsObjectTexture::restoreState()
	{
		//Set to the defaults
		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CW);
	}





}