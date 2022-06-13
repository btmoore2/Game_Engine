#ifndef ANIMATION_CLIP_H
#define ANIMATION_CLIP_H

#include "BoneData.h"
#include "AnimationFrame.h"
#include "GameObject.h"

namespace EngineSpace
{
	class AnimationClip
	{
	public:
		AnimationClip() = delete;
		AnimationClip(const AnimationClip&) = delete;
		AnimationClip& operator= (const AnimationClip&) = delete;
		~AnimationClip();

		AnimationClip(int boneCount, SSBO* pWorld);

		void addFrame(AnimationFrame* newFrame);
		void animateBones(AnimationTime tCurr);
		void Toggle();
		void Update(AnimationTime tCurr);

		void setHierarchy(int* pHierarchy, unsigned int count, int depth);
		void setBoneWorld(SSBO* pBones);
		AnimationTime getTotalTime();
		int getNumBones();
		SSBO* getResults();
		SSBO* getHierarchy();
		SSBO* getWorld();
		int getDepth();		

		AnimationClip* pNext;

	private:
		bool active;
		int numBones;
		int numFrames;
		int hierDepth;
		AnimationTime totalTime;
		AnimationFrame* poHead;
		AnimationFrame* poTail;
		SSBO* poResults;
		SSBO* poHierarchy;
		SSBO* pBoneWorld;
	};
}

#endif
