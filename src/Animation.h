#ifndef ANIMATION_H
#define ANIMATION_H

#include "AnimationClip.h"
#include "Skeleton.h"

namespace EngineSpace
{
	class Animation
	{
	public:
		Animation() = delete;
		Animation(const Animation&) = delete;
		Animation& operator= (const Animation&) = delete;
		~Animation();

		Animation(AnimationClip* newClip, int boneCount, Skeleton::Data* skelData);

		void Animate(AnimationTime tCurr);
		void poseSkeleton(AnimationTime tCurr);
		
		void setClip(AnimationClip* inClip);
		AnimationTime getMaxTime();
		GameObject* getSkeleton();

	private:
		void privSetBonePose(GameObjectBone* pNode);
		
		AnimationClip* pClip;
		BoneData* poBoneResult;
		Skeleton* poSkeleton;
		int numBones;
	};
}


#endif