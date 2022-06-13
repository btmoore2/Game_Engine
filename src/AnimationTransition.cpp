#include "AnimationTransition.h"
#include <assert.h>
#include "ShaderManager.h"


namespace EngineSpace
{
	AnimationTransition::AnimationTransition(AnimationClip* pCurrent, AnimationClip* pNext)
		:pCurrClip(pCurrent),pNextClip(pNext),t(0),tWeight(0),numBones(pCurrent->getNumBones()),complete(false)
	{
		BoneData* pBone = new BoneData[(unsigned int)this->numBones];
		this->poResult = new SSBO();
		this->poResult->Set((unsigned int) this->numBones, sizeof(BoneData), pBone);
		delete[] pBone;
	}

	AnimationTransition::~AnimationTransition()
	{
		delete this->poResult;
	}

	bool AnimationTransition::isDone()
	{
		return this->complete;
	}

	void AnimationTransition::Update(AnimationTime currTime)
	{
		//First update the transition weight.
		this->privTransitionTime();

		//Update both clips
		this->pCurrClip->Update(currTime);
		this->pNextClip->Update(currTime);

		//Mix the current clip with the next clip according to the transition weight. 
		ShaderObject* pComputeShader = ShaderManager::Find(ShaderObject::Name::COMPUTE_MIXER);

		pComputeShader->setActive();
		this->pCurrClip->getResults()->Bind(0);
		this->pNextClip->getResults()->Bind(1);
		this->poResult->Bind(2);

		glUniform1f(pComputeShader->getLocation("ts"), this->tWeight);
		glUniform1i(pComputeShader->getLocation("boneCount"),this->numBones);

		pComputeShader->Dispatch((unsigned int)(numBones / 20), 1, 1);

		//Wait for everything to complete
		glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

		//Update the world
		pComputeShader = ShaderManager::Find(ShaderObject::Name::COMPUTE_WORLD);
		pComputeShader->setActive();

		this->poResult->Bind(0);
		this->pCurrClip->getHierarchy()->Bind(1);
		this->pCurrClip->getWorld()->Bind(2);

		glUniform1i(pComputeShader->getLocation("tableDepth"), this->pCurrClip->getDepth());
		glUniform1i(pComputeShader->getLocation("boneCount"), this->numBones);

		pComputeShader->Dispatch((unsigned int)this->numBones / 20, 1, 1);

		// Block Waiting for stages to complete
		glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

		if (this->tWeight == 1)
		{
			this->complete = true;
		}

	}

	void AnimationTransition::privTransitionTime()
	{
		//Gradual transition using 0.03 for each tick. 
		float t_delta = 0.03f;
		float b = 1.0f;
		float a = 0.0f;

		this->t += t_delta;

		if (t > 1.0f)
		{
			t = 1.0f;
		}

		this->tWeight = a + t * (b - a);
	}
}