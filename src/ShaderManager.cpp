#include <assert.h>
#include "ShaderManager.h"

namespace EngineSpace
{
	ShaderManager::ShaderManager()
		:pActiveHead(nullptr)
	{

	}

	ShaderManager* ShaderManager::privGetInstance()
	{
		static ShaderManager SingleManager;
		return &SingleManager;
	}

	void ShaderManager::Create()
	{
		ShaderManager* pMan = ShaderManager::privGetInstance();
		assert(pMan);
	}

	void ShaderManager::Destroy()
	{
		ShaderManager* pMan = ShaderManager::privGetInstance();
		assert(pMan);

		ShaderObject* pLink = pMan->pActiveHead;

		while (pLink != nullptr)
		{
			ShaderObject* pTmp = pLink;
			pLink = pLink->pNext;
			pMan->privRemove(pTmp, pMan->pActiveHead);
			delete pTmp;
		}
	}

	ShaderObject* ShaderManager::Find(ShaderObject::Name Target)
	{
		ShaderManager* pMan = ShaderManager::privGetInstance();
		assert(pMan);

		ShaderObject* Finder = pMan->pActiveHead;

		while (Finder != nullptr)
		{
			if (Finder->myName == Target)
			{
				return Finder;
			}
			Finder = Finder->pNext;
		}

		return nullptr;
	}

	void ShaderManager::Add(ShaderObject* pShader)
	{
		ShaderManager* pMan = ShaderManager::privGetInstance();
		assert(pMan);

		pMan->privAddToFront(pShader, pMan->pActiveHead);
	}

	void ShaderManager::privAddToFront(ShaderObject* pNode, ShaderObject*& pHead)
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

	void ShaderManager::privRemove(ShaderObject* pNode, ShaderObject*& pHead)
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