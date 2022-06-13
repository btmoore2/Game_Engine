#ifndef SKELETON_H
#define SKELETON_H

#include "GameObjectBone.h"
#include "BoneData.h"

namespace EngineSpace
{
	class Skeleton
	{
	public:
		struct Data
		{
			char name[64];
			int index;
			int parentIndex;
		};

		Skeleton() = delete;
		Skeleton(const Skeleton&) = delete;
		Skeleton& operator= (const Skeleton&) = delete;
		~Skeleton();

		Skeleton(BoneData* pBoneData, int boneCount, Skeleton::Data* newSkelData);

		GameObjectBone* getFirstBone();

	private:
		void privSetHierarchy(BoneData* pBone);
		GameObjectBone* privFindBone(int index);

		GameObjectBone* pFirstBone;
		Skeleton::Data* skelData;
		int numBones;

	};
}


#endif
