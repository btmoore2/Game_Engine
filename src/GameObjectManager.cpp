#include <assert.h>
#include "GameObjectManager.h"
#include "NullModel.h"
#include "ShaderManager.h"
#include "GraphicsObjectNull.h"
#include "GameObjectStatic.h"
#include "PCSTreeForwardIterator.h"
#include "FileModel.h"
#include "GraphicsObjectTexture.h"


namespace EngineSpace
{
	GameObjectManager::GameObjectManager()
	{
		//Create the tree
		this->pRootTree = new PCSTree();
		assert(this->pRootTree);

		//Create and set the root as a null object.
		NullModel* pModel = new NullModel();
		assert(pModel);
		ModelManager::Add(pModel, ModelName::NULLMODEL);

		ShaderObject* pShader = new ShaderObject(ShaderObject::Name::NULL_SHADER, "nullRender", ShaderObject::Type::GRAPHICS);
		assert(pShader);
		ShaderManager::Add(pShader);

		GraphicsObjectNull* pGraphicsObject = new GraphicsObjectNull(pModel, pShader);
		GameObjectStatic* pGameRoot = new GameObjectStatic(pGraphicsObject);
		pGameRoot->setName("GameObject_Root");

		this->pRootTree->Insert(pGameRoot, this->pRootTree->GetRoot());

	}

	GameObjectManager::~GameObjectManager()
	{
		delete this->pRootTree;
	}

	GameObjectManager* GameObjectManager::privGetInstance()
	{
		static GameObjectManager singleManager;
		return &singleManager;
	}

	void GameObjectManager::Create()
	{
		GameObjectManager* pMan = GameObjectManager::privGetInstance();
		assert(pMan);
	}

	void GameObjectManager::Add(GameObject* newObject, GameObject* pParent)
	{
		assert(newObject);
		assert(pParent);

		GameObjectManager* pMan = GameObjectManager::privGetInstance();
		assert(pMan);

		pMan->pRootTree->Insert(newObject, pParent);
	}

	GameObject* GameObjectManager::TextureLoadFromFile(const char* pModelFileName, const ShaderObject* const pShaderObj, ModelName modName, Texture::Name texName)
	{
		FileModel* pModel = new FileModel(pModelFileName);
		ModelManager::Add(pModel, modName);
		GraphicsObject* pGraphicsObj = new GraphicsObjectTexture(pModel, pShaderObj, texName);
		GameObject* pGameObj = new GameObjectStatic(pGraphicsObj);
		pGameObj->setPos(Vect(0.0f, 0.0f, 0.0f));
		//pGameObj->AddBoundingSphere(pModelFileName);

		GameObjectManager::Add(pGameObj, GameObjectManager::getRoot());
		return pGameObj;
	}

	void GameObjectManager::Draw()
	{
		GameObjectManager* pMan = GameObjectManager::privGetInstance();
		assert(pMan);

		PCSNode* pRoot = pMan->pRootTree->GetRoot();
		assert(pRoot);

		PCSTreeForwardIterator pForIter(pRoot);
		PCSNode* pNode = pForIter.First();

		GameObject* pGameObject;

		//while (!pForIter.isDone())
		while (pNode != nullptr)
		{
			assert(pNode);

			pGameObject = (GameObject*)pNode;
			pGameObject->Draw();

			pNode = pForIter.Next();
		}
	}

	void GameObjectManager::Update()
	{
		GameObjectManager* pMan = GameObjectManager::privGetInstance();
		assert(pMan);

		PCSNode* pRoot = pMan->pRootTree->GetRoot();
		assert(pRoot);

		PCSTreeForwardIterator pForIter(pRoot);
		PCSNode* pNode = pForIter.First();

		GameObject* pGameObject;

		//while (!pForIter.isDone())
		while (pNode != nullptr)
		{
			assert(pNode);

			pGameObject = (GameObject*)pNode;
			pGameObject->Update();

			pNode = pForIter.Next();
		}
	}

	void GameObjectManager::Remove(GameObject* Remove)
	{
		GameObjectManager* pMan = GameObjectManager::privGetInstance();
		assert(pMan);

		pMan->pRootTree->Remove(Remove);

		delete Remove;
	}

	void GameObjectManager::toggleBounding()
	{
		GameObjectManager* pMan = GameObjectManager::privGetInstance();
		assert(pMan);

		PCSNode* pRoot = pMan->pRootTree->GetRoot();
		assert(pRoot);

		PCSTreeForwardIterator pForIter(pRoot);
		PCSNode* pNode = pForIter.First();

		GameObject* pGameObject;

		while (!pForIter.isDone())
		{
			assert(pNode);

			pGameObject = (GameObject*)pNode;
			pGameObject->toggleSphere();

			pNode = pForIter.Next();
		}
	}	

	void GameObjectManager::toggle2D()
	{
		GameObjectManager* pMan = GameObjectManager::privGetInstance();
		assert(pMan);

		PCSNode* pRoot = pMan->pRootTree->GetRoot();
		assert(pRoot);

		PCSTreeForwardIterator pForIter(pRoot);
		PCSNode* pNode = pForIter.First();

		GameObject* pGameObject = 0;

		while (!pForIter.isDone())
		{
			assert(pNode);
			pGameObject = (GameObject*)pNode;
			if (pGameObject->Type == GOType::TEXT)
			{
				pGameObject->toggleDraw();
			}

			pNode = pForIter.Next();
		}
	}

	PCSTree* GameObjectManager::getPCSTree()
	{
		GameObjectManager* pMan = GameObjectManager::privGetInstance();
		assert(pMan);

		return pMan->pRootTree;
	}

	GameObject* GameObjectManager::getRoot()
	{
		GameObjectManager* pMan = GameObjectManager::privGetInstance();
		assert(pMan);

		return (GameObject*)pMan->pRootTree->GetRoot();
	}

	void GameObjectManager::Destroy()
	{
		GameObjectManager* pMan = GameObjectManager::privGetInstance();
		assert(pMan);

		//Clear all objects
		PCSTree* pTree = pMan->pRootTree;
		PCSNode* pNode = nullptr;

		PCSTreeForwardIterator pForIter(pTree->GetRoot());
		pNode = pForIter.First();
		PCSNode* pTmp = nullptr;
		while (!pForIter.isDone())
		{
			pTmp = pForIter.currentItem();

			pNode = pForIter.Next();
			delete pTmp;
		}

		delete pNode;
	}

}