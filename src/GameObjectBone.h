#ifndef GAME_OBJECT_BONE_H
#define GAME_OBJECT_BONE_H

#include "GameObject.h"
#include "BoneData.h"

namespace EngineSpace
{
	class GameObjectBone : public GameObject
	{
	public:
		GameObjectBone() = delete;
		GameObjectBone(const GameObjectBone&) = delete;
		GameObjectBone& operator= (const GameObjectBone&) = delete;
		virtual ~GameObjectBone();

		GameObjectBone(GraphicsObject* newGraphicsObject, BoneData* newBone);

		virtual void Update();

		Matrix getBoneOrientation();
		void setBoneOrientation(const Matrix& temp);
		void setIndex(int val);
		int getIndex();

	private:
		void privUpdate();
		
		Matrix* pBoneOrientation;
		BoneData* pBoneData;
		int boneArrayIndex;


	};
}


#endif
