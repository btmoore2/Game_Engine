#include "AnimationClip.h"
#include <assert.h>
#include "ShaderManager.h"

namespace EngineSpace
{
	AnimationClip::AnimationClip(int boneCount, SSBO* pWorld)
		: pNext(nullptr),active(false),numBones(boneCount),numFrames(0),hierDepth(0),totalTime(AnimationTime::Length::ZERO), poHead(nullptr),poTail(nullptr),pBoneWorld(pWorld)
	{
		this->poResults = new SSBO();
		this->poHierarchy = new SSBO();

		BoneData* pData = new BoneData[(unsigned int)this->numBones];
		assert(pData);
		this->poResults->Set((unsigned int)this->numBones, sizeof(BoneData), pData);
		delete[] pData;
	}

	AnimationClip::~AnimationClip()
	{
		AnimationFrame* pDel = this->poHead;
		AnimationFrame* pDelNext;

		while (pDel != nullptr)
		{
			pDelNext = pDel->pNext;
			delete pDel;
			pDel = pDelNext;
		}

		delete this->poResults;
		delete this->poHierarchy;
	}

	void AnimationClip::addFrame(AnimationFrame* newFrame)
	{
		//Frames are always added first to last. 

		if (this->poHead == nullptr)
		{
			//While the frames should be coming in with null pointers, resetting them as a precaution.
			this->poHead = newFrame;
			this->poTail = newFrame;
			newFrame->pNext = nullptr;
			newFrame->pPrev = nullptr;
		}
		else
		{
			this->poTail->pNext = newFrame;
			newFrame->pPrev = this->poTail;
			newFrame->pNext = nullptr;
			this->poTail = newFrame;
		}

		this->numFrames++;
		this->totalTime = this->poTail->getTime();
	}

	void AnimationClip::animateBones(AnimationTime tCurr)
	{
		this->Update(tCurr);

		ShaderObject* pComputeShader = ShaderManager::Find(ShaderObject::Name::COMPUTE_WORLD);
		pComputeShader->setActive();

		this->poResults->Bind(0);
		this->poHierarchy->Bind(1);
		this->pBoneWorld->Bind(2);

		glUniform1i(pComputeShader->getLocation("tableDepth"), this->hierDepth);
		glUniform1i(pComputeShader->getLocation("boneCount"), this->numBones);

		pComputeShader->Dispatch((unsigned int)this->numBones / 20, 1, 1);

		//Wait for all stages to complete.
		glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);


	}

	void AnimationClip::Toggle()
	{
		this->active = !this->active;
	}

	void AnimationClip::Update(AnimationTime tCurr)
	{
		AnimationFrame* pTemp = this->poHead;

		//Find the current frame. 
		while (tCurr >= pTemp->getTime() && pTemp->pNext != nullptr)
		{
			pTemp = pTemp->pNext;
		}

		//pTemp is the current frame we are updating to. 
		ShaderObject* pComputeShader = ShaderManager::Find(ShaderObject::Name::COMPUTE_MIXER);

		float tWeight = (tCurr - pTemp->pPrev->getTime()) / (pTemp->getTime() - pTemp->pPrev->getTime());

		pComputeShader->setActive();
		pTemp->pPrev->getFrame()->Bind(0);
		pTemp->getFrame()->Bind(1);
		this->poResults->Bind(2);

		glUniform1f(pComputeShader->getLocation("ts"), tWeight);
		glUniform1i(pComputeShader->getLocation("boneCount"), numBones);

		pComputeShader->Dispatch((unsigned int)(numBones / 20), 1, 1);

		//Wait for all stages to complete
		glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

	}

	void AnimationClip::setHierarchy(int* pHierarchy, unsigned int count, int depth)
	{
		this->poHierarchy->Set(count, sizeof(int), pHierarchy);
		this->hierDepth = depth;
	}

	void AnimationClip::setBoneWorld(SSBO* pBones)
	{
		this->pBoneWorld = pBones;
	}

	AnimationTime AnimationClip::getTotalTime()
	{
		return this->totalTime;
	}

	int AnimationClip::getNumBones()
	{
		return this->numBones;
	}

	SSBO* AnimationClip::getResults()
	{
		return this->poResults;
	}

	SSBO* AnimationClip::getHierarchy()
	{
		return this->poHierarchy;
	}

	SSBO* AnimationClip::getWorld()
	{
		return this->pBoneWorld;
	}

	int AnimationClip::getDepth()
	{
		return this->hierDepth;
	}

}