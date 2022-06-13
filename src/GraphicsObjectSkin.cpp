#include "GraphicsObjectSkin.h"

namespace EngineSpace
{
	
	GraphicsObjectSkin::GraphicsObjectSkin(const Model* const newModel, const ShaderObject* const newShader)
		:GraphicsObject(newModel,newShader),pSkelHead(nullptr),pController(nullptr)
	{
		this->pBoneWorldSSBO = new SSBO();
	}

	GraphicsObjectSkin::~GraphicsObjectSkin()
	{
		delete this->pBoneWorldSSBO;
	}

	void GraphicsObjectSkin::setSkeleton(GameObject* newSkel)
	{
		this->pSkelHead = newSkel;
	}

	void GraphicsObjectSkin::setController(AnimationController* newController)
	{
		this->pController = newController;
	}

	GameObject* GraphicsObjectSkin::getSkeleton()
	{
		return this->pSkelHead;
	}

	SSBO* GraphicsObjectSkin::getSSBO()
	{
		return this->pBoneWorldSSBO;
	}

	AnimationController* GraphicsObjectSkin::getController()
	{
		return this->pController;
	}

}