#include "AnimationManager.h"
#include <assert.h>
#include "SkinModel.h"
#include "ShaderManager.h"
#include "GraphicsObjectSkinColor.h"
#include "GraphicsObjectSkinTexture.h"
#include "File.h"
#include "FileModelHdr.h"

namespace EngineSpace
{


	AnimationManager::AnimationManager()
		:poControllerHead(nullptr), pauseAnimation(false)
	{

	}

	AnimationManager* AnimationManager::privGetInstance()
	{
		static AnimationManager singleManager;
		return &singleManager;
	}

	AnimationManager::~AnimationManager()
	{

	}

	GraphicsObject* AnimationManager::AddSSBOModel(const char* const pModelFileName, ModelName inName, ShaderObject::Name gShader, Vect Light, Vect Pos)
	{
		//Creates a skin model (color only) with bones that is ready to animate, but has no attached animation clips.
		AnimationManager* pMan = AnimationManager::privGetInstance();
		assert(pMan);

		SkinModel* pModel = (SkinModel*)ModelManager::Find(inName);

		if (pModel == nullptr)
		{
			pModel = new SkinModel(pModelFileName);
			ModelManager::Add(pModel,inName);
		}

		ShaderObject* pGraphicsShader = ShaderManager::Find(gShader);
		assert(pGraphicsShader);

		GraphicsObjectSkinColor* pColorGraphicsObject = new GraphicsObjectSkinColor(pModel, pGraphicsShader, Light, Pos);
		Matrix* pMatrix = new Matrix[pModel->numBones]();
		pColorGraphicsObject->getSSBO()->Set((unsigned int)pModel->numBones, sizeof(Matrix), pMatrix);
		delete[]pMatrix;

		return pColorGraphicsObject;
	}

	GraphicsObject* AnimationManager::AddSSBOModel(const char* const pModelFileName, ModelName inName, ShaderObject::Name gShader, Texture::Name tName, Vect Light, Vect Pos)
	{
		//Creates a skin model (texture only) with bones that is ready to animate, but has no attached animation clips.
		AnimationManager* pMan = AnimationManager::privGetInstance();
		assert(pMan);

		SkinModel* pModel = (SkinModel*)ModelManager::Find(inName);

		if (pModel == nullptr)
		{
			pModel = new SkinModel(pModelFileName, tName);
			ModelManager::Add(pModel, inName);
		}

		ShaderObject* pGraphicsShader = ShaderManager::Find(gShader);
		assert(pGraphicsShader);

		GraphicsObjectSkinTexture* pTexGraphicsObject = new GraphicsObjectSkinTexture(pModel, pGraphicsShader, tName, Light, Pos);
		Matrix* pMatrix = new Matrix[pModel->numBones]();
		pTexGraphicsObject->getSSBO()->Set((unsigned int)pModel->numBones, sizeof(Matrix), pMatrix);
		delete[]pMatrix;

		return pTexGraphicsObject;
	}

	GraphicsObject* AnimationManager::AddAnimSSBOModel(const char* const pModelFileName, ModelName inName, ShaderObject::Name gShader, Vect Light, Vect Pos)
	{
		AnimationManager* pMan = AnimationManager::privGetInstance();
		assert(pMan);

		//Create the model
		GraphicsObjectSkinColor* pColorGraphicsObject = (GraphicsObjectSkinColor *)AnimationManager::AddSSBOModel(pModelFileName, inName, gShader, Light, Pos);

		File::Handle fh;
		File::Error  ferror;
		FileModelHdr modelHdr;

		//Open and copy header data from the file
		ferror = File::Open(fh, pModelFileName, File::Mode::READ);
		assert(ferror == File::Error::SUCCESS);
		ferror = File::Read(fh, &modelHdr, sizeof(FileModelHdr));
		assert(ferror == File::Error::SUCCESS);

		//Copy the hierarhcy data
		int* Hierarchy = new int[(unsigned int)modelHdr.depth * modelHdr.numData];

		ferror = File::Seek(fh, File::Location::BEGIN, (int)modelHdr.hierOffset);
		assert(ferror == File::Error::SUCCESS);

		for (int i = 0; i < (modelHdr.depth * modelHdr.numData); i++)
		{
			ferror = File::Read(fh, &Hierarchy[i], sizeof(int));
			assert(ferror == File::Error::SUCCESS);
		}

		//Copy skeleton data. This will be owned by the skeleton object created by the controller, and will be deleted by it. 
		Skeleton::Data* newSkelData = new Skeleton::Data[(unsigned int)modelHdr.numData];

		ferror = File::Seek(fh, File::Location::BEGIN, (int)modelHdr.skeletonOffset);
		assert(ferror == File::Error::SUCCESS);

		for (int i = 0; i < modelHdr.numData; i++)
		{
			ferror = File::Read(fh, &newSkelData[i], sizeof(Skeleton::Data));
			assert(ferror == File::Error::SUCCESS);
		}

		ferror = File::Seek(fh, File::Location::BEGIN, (int)modelHdr.animOffset);
		assert(ferror == File::Error::SUCCESS);

		int frameCount = 0;
		float keyTime = 0.0f;
		AnimationClip* newClip;
		readBone inBone;

		for (int i = 0; i < modelHdr.numAnim; i++)
		{
			newClip = new AnimationClip(modelHdr.numData, pColorGraphicsObject->getSSBO());
			newClip->setHierarchy(Hierarchy, (unsigned int)modelHdr.depth * modelHdr.numData, modelHdr.depth);

			ferror = File::Read(fh, &frameCount, sizeof(int));
			assert(ferror == File::Error::SUCCESS);

			for (int j = 0; j < frameCount; j++)
			{
				ferror = File::Read(fh, &keyTime, sizeof(float));
				assert(ferror == File::Error::SUCCESS);

				AnimationFrame* newFrame = new AnimationFrame();
				newFrame->setTime(AnimationTime(AnimationTime::Length::MILLISECOND) * keyTime);

				BoneData* Frames = new BoneData[(unsigned int)modelHdr.numData];

				//Read in the bone data and convert to usable format
				for (int k = 0; k < modelHdr.numData; k++)
				{
					ferror = File::Read(fh, &inBone, sizeof(readBone));
					assert(ferror == File::Error::SUCCESS);

					Frames[k].T = Vect(inBone.T.x, inBone.T.y, inBone.T.z);
					Frames[k].R = Quat(Quat::Rot3::XYZ, inBone.Q.x, inBone.Q.y, inBone.Q.z);
					Frames[k].S = Vect(inBone.S.x, inBone.S.y, inBone.S.z);
				}

				newFrame->getFrame()->Set((unsigned int)modelHdr.numData, sizeof(BoneData), Frames);
				delete[] Frames;

				newClip->addFrame(newFrame);

			}

			if (pMan->poControllerHead == nullptr)
			{
				//First animated object, create head.
				AnimationTime delta = 0.1f * AnimationTime(AnimationTime::Length::FPS30);
				pMan->poControllerHead = new AnimationController(delta, newClip, modelHdr.numData, newSkelData);
				pColorGraphicsObject->setController(pMan->poControllerHead);

			}
			else if (pMan->poControllerHead->currStatus == Status::LOADING)
			{
				//This controller is already loading for this object. 
				pMan->poControllerHead->addClip(newClip);
			}
			else
			{
				//New animated object, so new controller needed. 
				AnimationTime delta = 0.1f * AnimationTime(AnimationTime::Length::FPS30);
				AnimationController* newController = new AnimationController(delta, newClip, modelHdr.numData, newSkelData);
				newController->pNext = pMan->poControllerHead;
				pMan->poControllerHead = newController;
				pColorGraphicsObject->setController(pMan->poControllerHead);
			}

		}

		delete[] Hierarchy;


		pMan->poControllerHead->currStatus = Status::FINISHED;
		pColorGraphicsObject->setSkeleton((GameObject*)pMan->poControllerHead->getSkeleton()->getParent());

		ferror = File::Close(fh);
		assert(ferror == File::Error::SUCCESS);

		return pColorGraphicsObject;

	}

	GraphicsObject* AnimationManager::AddAnimSSBOModel(const char* const pModelFileName, ModelName inName, ShaderObject::Name gShader, Texture::Name tName, Vect Light, Vect Pos)
	{
		AnimationManager* pMan = AnimationManager::privGetInstance();
		assert(pMan);

		//Create the model
		GraphicsObjectSkinTexture* pTexGraphicsObject = (GraphicsObjectSkinTexture*)AnimationManager::AddSSBOModel(pModelFileName, inName, gShader, tName, Light, Pos);

		File::Handle fh;
		File::Error  ferror;
		FileModelHdr modelHdr;

		//Open and copy header data from the file
		ferror = File::Open(fh, pModelFileName, File::Mode::READ);
		assert(ferror == File::Error::SUCCESS);
		ferror = File::Read(fh, &modelHdr, sizeof(FileModelHdr));
		assert(ferror == File::Error::SUCCESS);

		//Copy the hierarhcy data
		int* Hierarchy = new int[(unsigned int)modelHdr.depth * modelHdr.numData];

		ferror = File::Seek(fh, File::Location::BEGIN, (int)modelHdr.hierOffset);
		assert(ferror == File::Error::SUCCESS);

		for (int i = 0; i < (modelHdr.depth * modelHdr.numData); i++)
		{
			ferror = File::Read(fh, &Hierarchy[i], sizeof(int));
			assert(ferror == File::Error::SUCCESS);
		}

		//Copy skeleton data. This will be owned by the skeleton object created by the controller, and will be deleted by it. 
		Skeleton::Data* newSkelData = new Skeleton::Data[(unsigned int)modelHdr.numData];

		ferror = File::Seek(fh, File::Location::BEGIN, (int)modelHdr.skeletonOffset);
		assert(ferror == File::Error::SUCCESS);

		for (int i = 0; i < modelHdr.numData; i++)
		{
			ferror = File::Read(fh, &newSkelData[i], sizeof(Skeleton::Data));
			assert(ferror == File::Error::SUCCESS);
		}

		ferror = File::Seek(fh, File::Location::BEGIN, (int)modelHdr.animOffset);
		assert(ferror == File::Error::SUCCESS);

		int frameCount = 0;
		float keyTime = 0.0f;
		AnimationClip* newClip;
		readBone inBone;

		for (int i = 0; i < modelHdr.numAnim; i++)
		{
			newClip = new AnimationClip(modelHdr.numData, pTexGraphicsObject->getSSBO());
			newClip->setHierarchy(Hierarchy, (unsigned int)modelHdr.depth * modelHdr.numData, modelHdr.depth);

			ferror = File::Read(fh, &frameCount, sizeof(int));
			assert(ferror == File::Error::SUCCESS);

			for (int j = 0; j < frameCount; j++)
			{
				ferror = File::Read(fh, &keyTime, sizeof(float));
				assert(ferror == File::Error::SUCCESS);

				AnimationFrame* newFrame = new AnimationFrame();
				newFrame->setTime(AnimationTime(AnimationTime::Length::MILLISECOND) * keyTime);

				BoneData* Frames = new BoneData[(unsigned int)modelHdr.numData];

				//Read in the bone data and convert to usable format
				for (int k = 0; k < modelHdr.numData; k++)
				{
					ferror = File::Read(fh, &inBone, sizeof(readBone));
					assert(ferror == File::Error::SUCCESS);

					Frames[k].T = Vect(inBone.T.x, inBone.T.y, inBone.T.z);
					Frames[k].R = Quat(Quat::Rot3::XYZ, inBone.Q.x, inBone.Q.y, inBone.Q.z);
					Frames[k].S = Vect(inBone.S.x, inBone.S.y, inBone.S.z);
				}

				newFrame->getFrame()->Set((unsigned int)modelHdr.numData, sizeof(BoneData), Frames);
				delete[] Frames;

				newClip->addFrame(newFrame);

			}

			if (pMan->poControllerHead == nullptr)
			{
				//First animated object, create head.
				AnimationTime delta = 0.1f * AnimationTime(AnimationTime::Length::FPS30);
				pMan->poControllerHead = new AnimationController(delta, newClip, modelHdr.numData, newSkelData);
				pTexGraphicsObject->setController(pMan->poControllerHead);

			}
			else if (pMan->poControllerHead->currStatus == Status::LOADING)
			{
				//This controller is already loading for this object. 
				pMan->poControllerHead->addClip(newClip);
			}
			else
			{
				//New animated object, so new controller needed. 
				AnimationTime delta = 0.1f * AnimationTime(AnimationTime::Length::FPS30);
				AnimationController* newController = new AnimationController(delta, newClip, modelHdr.numData, newSkelData);
				newController->pNext = pMan->poControllerHead;
				pMan->poControllerHead = newController;
				pTexGraphicsObject->setController(pMan->poControllerHead);
			}

		}

		delete[] Hierarchy;


		pMan->poControllerHead->currStatus = Status::FINISHED;
		pTexGraphicsObject->setSkeleton((GameObject*)pMan->poControllerHead->getSkeleton()->getParent());

		ferror = File::Close(fh);
		assert(ferror == File::Error::SUCCESS);

		return pTexGraphicsObject;
	}

	void AnimationManager::addAnimation(const char* const pModelFileName, GraphicsObjectSkin* addTo)
	{
		AnimationManager* pMan = AnimationManager::privGetInstance();
		assert(pMan);

		File::Handle fh;
		File::Error  ferror;
		FileModelHdr modelHdr;

		//Open and copy header data from the file
		ferror = File::Open(fh, pModelFileName, File::Mode::READ);
		assert(ferror == File::Error::SUCCESS);
		ferror = File::Read(fh, &modelHdr, sizeof(FileModelHdr));
		assert(ferror == File::Error::SUCCESS);

		//Verify that the number of bones matches the model. 
		SkinModel* pModel = (SkinModel*)addTo->getModel();
		if (pModel->numBones != (unsigned int)modelHdr.numData)
		{
			//If not matching, close file, don't add the animation and exit
			ferror = File::Close(fh);
			assert(ferror == File::Error::SUCCESS);
			return;
		}

		//Copy the hierarchy data
		int* Hierarchy = new int[(unsigned int)modelHdr.depth * modelHdr.numData];
		
		ferror = File::Seek(fh, File::Location::BEGIN, (int)modelHdr.hierOffset);
		assert(ferror == File::Error::SUCCESS);

		for (int i = 0; i < (modelHdr.depth * modelHdr.numData); i++)
		{
			ferror = File::Read(fh, &Hierarchy[i], sizeof(int));
			assert(ferror == File::Error::SUCCESS);
		}
		

		int frameCount = 0;
		float keyTime = 0.0f;
		AnimationClip* newClip = nullptr;
		readBone inBone;
		
		for (int i = 0; i < modelHdr.numAnim; i++)
		{
			newClip = new AnimationClip(modelHdr.numData, addTo->getSSBO());
			newClip->setHierarchy(Hierarchy, (unsigned int)modelHdr.depth * modelHdr.numData, modelHdr.depth);

			ferror = File::Read(fh, &frameCount, sizeof(int));
			assert(ferror == File::Error::SUCCESS);

			for (int j = 0; j < frameCount; j++)
			{
				ferror = File::Read(fh, &keyTime, sizeof(float));
				assert(ferror == File::Error::SUCCESS);

				AnimationFrame* newFrame = new AnimationFrame();
				newFrame->setTime(AnimationTime(AnimationTime::Length::MILLISECOND) * keyTime);

				BoneData* Frames = new BoneData[(unsigned int)modelHdr.numData];

				//Read in the bone data and convert to usable format
				for (int k = 0; k < modelHdr.numData; k++)
				{
					ferror = File::Read(fh, &inBone, sizeof(readBone));
					assert(ferror == File::Error::SUCCESS);

					Frames[k].T = Vect(inBone.T.x, inBone.T.y, inBone.T.z);
					Frames[k].R = Quat(Quat::Rot3::XYZ, inBone.Q.x, inBone.Q.y, inBone.Q.z);
					Frames[k].S = Vect(inBone.S.x, inBone.S.y, inBone.S.z);
				}

				newFrame->getFrame()->Set((unsigned int)modelHdr.numData, sizeof(BoneData), Frames);
				delete[] Frames;

				newClip->addFrame(newFrame);

			}

			if (addTo->getController() == nullptr)
			{
				//This will be the first animation, so a new controller will be created. Pull skeleton data. 
				Skeleton::Data* newSkelData = new Skeleton::Data[(unsigned int)modelHdr.numData];

				ferror = File::Seek(fh, File::Location::BEGIN, (int)modelHdr.skeletonOffset);
				assert(ferror == File::Error::SUCCESS);

				for (int l = 0; l < modelHdr.numData; l++)
				{
					ferror = File::Read(fh, &newSkelData[l], sizeof(Skeleton::Data));
					assert(ferror == File::Error::SUCCESS);
				}

				if (pMan->poControllerHead == nullptr)
				{
					//First animated object, create as head
					AnimationTime delta = 0.1f * AnimationTime(AnimationTime::Length::FPS30);
					pMan->poControllerHead = new AnimationController(delta, newClip, modelHdr.numData, newSkelData);
					addTo->setController(pMan->poControllerHead);
					addTo->setSkeleton((GameObject*)pMan->poControllerHead->getSkeleton()->getParent());

				}
				else
				{
					AnimationTime delta = 0.1f * AnimationTime(AnimationTime::Length::FPS30);
					AnimationController* newController = new AnimationController(delta, newClip, modelHdr.numData, newSkelData);
					newController->pNext = pMan->poControllerHead;
					pMan->poControllerHead = newController;
					addTo->setController(pMan->poControllerHead);
					addTo->setSkeleton((GameObject*)pMan->poControllerHead->getSkeleton()->getParent());
				}

				pMan->poControllerHead->currStatus = Status::FINISHED;
			}
			else
			{
				//Adding to pre-existing controller
				addTo->getController()->addClip(newClip);
			}

		}
		
		
		delete[] Hierarchy;

		ferror = File::Close(fh);
		assert(ferror == File::Error::SUCCESS);
	}

	void AnimationManager::Create()
	{
		AnimationManager* pMan = AnimationManager::privGetInstance();
		assert(pMan);
	}

	void AnimationManager::Destroy()
	{
		AnimationManager* pMan = AnimationManager::privGetInstance();
		assert(pMan);

		AnimationController* tempDel = pMan->poControllerHead;
		AnimationController* tempNext;

		while (tempDel != nullptr)
		{
			tempNext = tempDel->pNext;
			delete tempDel;
			tempDel = tempNext;
		}
	}

	void AnimationManager::Update()
	{
		AnimationManager* pMan = AnimationManager::privGetInstance();
		assert(pMan);

		if (!pMan->pauseAnimation)
		{
			AnimationController* Current = pMan->poControllerHead;

			while (Current != nullptr)
			{
				Current->Update();
				Current = Current->pNext;
			}
		}
	}

	void AnimationManager::nextClip()
	{
		AnimationManager* pMan = AnimationManager::privGetInstance();
		assert(pMan);

		if (!pMan->pauseAnimation)
		{
			AnimationController* Current = pMan->poControllerHead;

			while (Current != nullptr)
			{
				Current->nextClip();
				Current = Current->pNext;
			}
		}
	}

	void AnimationManager::Faster()
	{
		AnimationManager* pMan = AnimationManager::privGetInstance();
		assert(pMan);
		
		AnimationController* Current = pMan->poControllerHead;

		while (Current != nullptr)
		{
			Current->Faster();
			Current = Current->pNext;
		}
		
	}

	void AnimationManager::Slower()
	{
		AnimationManager* pMan = AnimationManager::privGetInstance();
		assert(pMan);

		AnimationController* Current = pMan->poControllerHead;

		while (Current != nullptr)
		{
			Current->Slower();
			Current = Current->pNext;
		}
	}

	void AnimationManager::Reverse()
	{
		AnimationManager* pMan = AnimationManager::privGetInstance();
		assert(pMan);

		AnimationController* Current = pMan->poControllerHead;

		while (Current != nullptr)
		{
			Current->Reverse();
			Current = Current->pNext;
		}
	}

	void AnimationManager::Pause()
	{
		AnimationManager* pMan = AnimationManager::privGetInstance();
		assert(pMan);

		pMan->pauseAnimation = !pMan->pauseAnimation;

	}
}