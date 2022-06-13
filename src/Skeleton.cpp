#include "Skeleton.h"
#include <assert.h>
#include "PyramidModel.h"
#include "ModelManager.h"
#include "ShaderManager.h"
#include "NullModel.h"
#include "GraphicsObjectNull.h"
#include "GameObjectStatic.h"
#include "GameObjectManager.h"
#include "GraphicsObjectWireframe.h"
#include "PCSTreeForwardIterator.h"



namespace EngineSpace
{
	Skeleton::Skeleton(BoneData* pBoneData, int boneCount, Skeleton::Data* newSkelData)
		:pFirstBone(nullptr), skelData(newSkelData), numBones(boneCount)
	{
		this->privSetHierarchy(pBoneData);
		assert(pFirstBone);
	}

	Skeleton::~Skeleton()
	{
		delete[] this->skelData;
	}

	GameObjectBone* Skeleton::getFirstBone()
	{
		assert(pFirstBone);
		return this->pFirstBone;
	}

	void Skeleton::privSetHierarchy(BoneData* pBoneData)
	{
		//Skeleton uses the pyramid model for each bone. 
		PyramidModel* pPyramidModel = (PyramidModel*)ModelManager::Find(ModelName::PYRAMID);

		if (pPyramidModel == nullptr)
		{
			pPyramidModel = new PyramidModel("pyramidModel.mdl");
			ModelManager::Add(pPyramidModel, ModelName::PYRAMID);
		}

		assert(pPyramidModel);

		//Load shaders for the skeleton
		ShaderObject* pShaderColor = new ShaderObject(ShaderObject::Name::COLOR_LIGHT, "colorRender", ShaderObject::Type::GRAPHICS);
		assert(pShaderColor);
		ShaderManager::Add(pShaderColor);

		ShaderObject* pShaderNull = new ShaderObject(ShaderObject::Name::NULL_SHADER, "nullRender", ShaderObject::Type::GRAPHICS);
		assert(pShaderNull);
		ShaderManager::Add(pShaderNull);

		//Create a null object as the root of the skeleton.
		NullModel* pModelNull = (NullModel*)ModelManager::Find(ModelName::NULLMODEL);
		GraphicsObjectNull* pNullObj = new GraphicsObjectNull(pModelNull, pShaderNull);
		GameObjectStatic* pSkelHead = new GameObjectStatic(pNullObj);
		pSkelHead->setName("RigidHead");
		GameObjectManager::Add(pSkelHead, GameObjectManager::getRoot());

		//First bone isn't rendered as its a just for model hierarchy.
		pNullObj = new GraphicsObjectNull(pPyramidModel, pShaderNull);
		GameObjectBone* pBoneObject = new GameObjectBone(pNullObj, pBoneData);
		pBoneObject->setIndex(this->skelData[0].index);
		pBoneObject->setName(this->skelData[0].name);
		GameObjectManager::Add(pBoneObject, pSkelHead);
		this->pFirstBone = pBoneObject;

		//Now add all bones to the list
		for (int i = 1; i < this->numBones; i++)
		{
			GraphicsObjectWireframe* pWireFrame = new GraphicsObjectWireframe(pPyramidModel, pShaderColor);
			pBoneObject = new GameObjectBone(pWireFrame, pBoneData);
			pBoneObject->setIndex(this->skelData[i].index);
			pBoneObject->setName(this->skelData[i].name);

			GameObjectBone* pParent = this->privFindBone(this->skelData[i].parentIndex);
			if (pParent)
			{
				//Objects are created with a draw true, default for bones will be to not be drawn.
				pBoneObject->toggleDraw();
				GameObjectManager::Add(pBoneObject, pParent);
			}
			else
			{
				//If no parent, it isn't part of the hierarchy. Delete.
				delete pBoneObject;
			}
		}	

	}

	GameObjectBone* Skeleton::privFindBone(int index)
	{
		GameObjectBone* pBoneObject = this->getFirstBone();

		if (pBoneObject->getIndex() == index)
		{
			return pBoneObject;
		}

		PCSTreeForwardIterator pIter(pBoneObject);
		PCSNode* pNode = pIter.First();

		while (pNode != nullptr)
		{
			assert(pNode);
			pBoneObject = (GameObjectBone*)pNode;
			if (pBoneObject->getIndex() == index)
			{
				return pBoneObject;
			}
			pNode = pIter.Next();
		}

		return nullptr;
	}
}