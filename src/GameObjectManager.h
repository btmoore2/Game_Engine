#ifndef GAME_OBJECT_MANAGER_H
#define GAME_OBJECT_MANAGER_H

#include "GameObject.h"
#include "PCSTree.h"
#include "Texture.h"
#include "ModelManager.h"

namespace EngineSpace
{
	class GameObjectManager
	{
	public:
		GameObjectManager(const GameObjectManager&) = delete;
		GameObjectManager& operator= (const GameObjectManager&) = delete;
		~GameObjectManager();

		static void Create();
		static void Add(GameObject* newObject, GameObject* pParent);
		static GameObject* TextureLoadFromFile(const char* pModelFileName, const ShaderObject* const pShaderObj, ModelName modName, Texture::Name texName = Texture::Name::DEFAULT);

		static void Draw();
		static void Update();
		static void Remove(GameObject* Remove);
		static void toggleBounding();
		static void toggle2D();
		static PCSTree* getPCSTree();
		static GameObject* getRoot();		
		static void Destroy();

	private:
		GameObjectManager();
		static GameObjectManager* privGetInstance();

		PCSTree* pRootTree;
	};
}

#endif