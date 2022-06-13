#include "CameraManager.h"
#include <assert.h>

namespace EngineSpace
{
	CameraManager::CameraManager()
		:active3D(nullptr), active2D(nullptr), head3D(nullptr), head2D(nullptr)
	{

	}

	CameraManager* CameraManager::privGetInstance()
	{
		static CameraManager camMan;
		return &camMan;
	}

	void CameraManager::Create()
	{
		CameraManager* pMan = CameraManager::privGetInstance();
		assert(pMan);
	}

	void CameraManager::Destroy()
	{
		CameraManager* pMan = CameraManager::privGetInstance();
		assert(pMan);

		Camera* pLink = pMan->head3D;

		while (pLink != nullptr)
		{
			Camera* pTmp = pLink;
			pLink = (Camera*)pLink->pNext;
			pMan->privRemove(pTmp, pMan->head3D);
			delete pTmp;
		}

		pLink = pMan->head2D;

		while (pLink != nullptr)
		{
			Camera* pTmp = pLink;
			pLink = (Camera*)pLink->pNext;
			pMan->privRemove(pTmp, pMan->head2D);
			delete pTmp;
		}
	}

	void CameraManager::Add(Camera* pCam)
	{
		CameraManager* pMan = CameraManager::privGetInstance();
		assert(pMan);
		assert(pCam);

		if (pCam->getType() == Camera::Type::PERSPECTIVE_3D)
		{
			pMan->privAddToFront(pCam, pMan->head3D);
			//Set as the active camera if this is the first. 
			if (pMan->active3D == nullptr)
			{
				pMan->active3D = pCam;
			}
		}
		else
		{
			pMan->privAddToFront(pCam, pMan->head2D);
			//Set as the active camera if this is the first. 
			if (pMan->active2D == nullptr)
			{
				pMan->active2D = pCam;
			}
		}
	}

	Camera* CameraManager::getCurrCam(Camera::Type type)
	{
		CameraManager* pMan = CameraManager::privGetInstance();
		assert(pMan);

		if (type == Camera::Type::PERSPECTIVE_3D)
		{
			return pMan->active3D;
		}
		else
		{
			return pMan->active2D;
		}
	}

	void CameraManager::Switch(Camera::Type type)
	{
		CameraManager* pMan = CameraManager::privGetInstance();
		assert(pMan);

		if (type == Camera::Type::PERSPECTIVE_3D)
		{
			pMan->active3D = pMan->active3D->pNext;

			if (pMan->active3D == nullptr)
			{
				pMan->active3D = pMan->head3D;
			}
		}

		else
		{
			pMan->active2D = pMan->active3D->pNext;

			if (pMan->active2D == nullptr)
			{
				pMan->active2D = pMan->head2D;
			}
		}
	}

	void CameraManager::privAddToFront(Camera* pNode, Camera*& pHead)
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

	void CameraManager::privRemove(Camera* pNode, Camera*& poHead)
	{
		assert(pNode);

		if (pNode->pPrev != nullptr)
		{	// middle or last node
			pNode->pPrev->pNext = pNode->pNext;
		}
		else
		{  // first
			poHead = (Camera*)pNode->pNext;
		}

		if (pNode->pNext != nullptr)
		{	// middle node
			pNode->pNext->pPrev = pNode->pPrev;
		}

	}

}

