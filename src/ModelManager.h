#ifndef MODEL_MANAGER_H
#define MODEL_MANAGER_H

#include "Model.h"

namespace EngineSpace
{
	enum class ModelName
	{
		NULLMODEL,
		WARBEAR,
		FRIGATE,
		BUGGY,
		PYRAMID,
		TEDDY,
		HUMAN,
		OTHER
	};

	struct ModelNode
	{
		Model* poModel;
		ModelName oName;
		ModelNode* pNext;
		ModelNode* pPrev;
	};

	class ModelManager
	{
	public:
		static void Add(Model* pModel, ModelName name);
		static Model* Find(ModelName name);
		static void Create();
		static void Destroy();

	private:
		static ModelManager* privGetInstance();
		ModelManager();

		void privAddToFront(ModelNode* pNode, ModelNode*& pHead);
		void privRemove(ModelNode* pNode, ModelNode*& pHead);

		ModelNode* pActiveHead;
	};
}


#endif
