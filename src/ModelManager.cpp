#include <assert.h>
#include "ModelManager.h"

namespace EngineSpace
{
	ModelManager::ModelManager()
		:pActiveHead(nullptr)
	{

	}

	ModelManager* ModelManager::privGetInstance()
	{
		static ModelManager SingleManager;
		return &SingleManager;
	}

	void ModelManager::Create()
	{
		ModelManager* pModelManager = ModelManager::privGetInstance();
		assert(pModelManager);
	}

	void ModelManager::Destroy()
	{
		ModelManager* pMan = ModelManager::privGetInstance();
		assert(pMan);

		ModelNode* pLink = pMan->pActiveHead;

		while (pLink != nullptr)
		{
			ModelNode* pTemp = pLink;
			pLink = pLink->pNext;
			pMan->privRemove(pTemp, pMan->pActiveHead);
			delete pTemp->poModel;
			delete pTemp;
		}
	}

	void ModelManager::Add(Model* pModel, ModelName name)
	{
		ModelManager* pMan = ModelManager::privGetInstance();
		assert(pMan);
		assert(pModel);

		ModelNode* newNode = new ModelNode{ pModel, name, nullptr, nullptr };

		pMan->privAddToFront(newNode, pMan->pActiveHead);

	}

	Model* ModelManager::Find(ModelName searchName)
	{
		ModelManager* pMan = ModelManager::privGetInstance();
		assert(pMan);

		ModelNode* Search = pMan->pActiveHead;

		while (Search != nullptr)
		{
			if (Search->oName == searchName)
			{
				return Search->poModel;
			}

			Search = Search->pNext;
		}

		return nullptr;
	}

	void ModelManager::privAddToFront(ModelNode* pNode, ModelNode*& pHead)
	{
		assert(pNode);

		if (pHead == nullptr)
		{
			pHead = pNode;
		}
		else
		{
			pNode->pNext = pHead;
			pHead->pPrev = pNode;
			pHead = pNode;
		}
	}
	
	void ModelManager::privRemove(ModelNode* pNode, ModelNode*& pHead)
	{
		assert(pNode);

		if (pNode->pPrev != nullptr)
		{
			//Middle/Last node.
			pNode->pPrev->pNext = pNode->pNext;
		}
		else
		{
			//Was head node. Fine if next is null as previous updates next
			pHead = pNode->pNext;
		}

		if (pNode->pNext != nullptr)
		{
			//Was head or middle.
			pNode->pNext->pPrev = pNode->pPrev;
		}

	}
}