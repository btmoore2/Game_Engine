#include <assert.h>
#include "Game.h"
#include "MathEngine.h"
#include "InputManager.h"
#include "ModelManager.h"
#include "ShaderManager.h"
#include "GameObjectManager.h"
#include "AnimationManager.h"
#include "CameraManager.h"
#include "TextureManager.h"	

//Demo files
#include "GraphicsObjectSkinColor.h"
#include "GraphicsObjectSkinTexture.h"
#include "GameObjectStatic.h"
#include "PyramidModel.h"
#include "SkinModel.h"

namespace EngineSpace
{
	

	Game::Game(const char* const pWindowName, const int Width, const int Height)
		:Engine(pWindowName, Width, Height)
	{
		assert(pWindowName);
	}

	Game::~Game()
	{

	}

	void Game::Initialize()
	{

	}

	void Game::loadContent()
	{
		//Start the managers.
		InputManager::Create();
		ModelManager::Create();
		ShaderManager::Create();
		GameObjectManager::Create();
		AnimationManager::Create();
		CameraManager::Create();

		//3D Perspective Cams		
		Camera* pCam = new Camera();
		pCam->setViewport(0, 0, this->info.windowWidth, this->info.windowHeight);
		pCam->setPerspective(35.0f, float(pCam->getScreenWidth()) / float(pCam->getScreenHeight()), 1.0f, 20000.0f);
		pCam->setOrientAndPosition(Vect(0.0f, 0.0f, 1.0f), Vect(0.0f, 0.0f, 0.0f), Vect(2000.0f, 0.0f, 0.0f));
		pCam->updateCamera();
		CameraManager::Add(pCam);

		pCam = new Camera();
		pCam->setViewport(0, 0, this->info.windowWidth, this->info.windowHeight);
		pCam->setPerspective(50.0f, float(pCam->getScreenWidth()) / float(pCam->getScreenHeight()), 0.10f, 1000.0f);
		pCam->setOrientAndPosition(Vect(0.0f, 0.0f, 1.0f), Vect(0.0f, 0.0f, 0.0f), Vect(-3000.0f, 0.0f, 0.0f));
		pCam->updateCamera();
		CameraManager::Add(pCam);

		//2D Cam
		pCam = new Camera(Camera::Type::ORTHOGRAPHIC_2D);
		pCam->setViewport(0, 0, this->info.windowWidth, this->info.windowHeight);
		pCam->setOrthographic(float(-pCam->getScreenWidth()) / 2.0f, float(pCam->getScreenWidth()) / 2.0f, float(-pCam->getScreenHeight()) / 2.0f, float(pCam->getScreenHeight()) / 2.0f, 1.0f, 1000.0f);
		pCam->setOrientAndPosition(Vect(0.0f, 1.0f, 0.0f), Vect(0.0f, 0.0f, -1.0f), Vect(0.0f, 0.0f, 2.0f));
		CameraManager::Add(pCam);

		//Animated Objects
		Vect color(1.0f, 0.0f, 0.0f, 1.0f);
		Vect pos(0.0f, 0.0f, 0.0f, 1.0f);

		ShaderObject* pShaderObjectMixer = new ShaderObject(ShaderObject::Name::COMPUTE_MIXER, "computeMixer", ShaderObject::Type::COMPUTE);
		assert(pShaderObjectMixer);
		ShaderManager::Add(pShaderObjectMixer);

		ShaderObject* pShaderObjectWorld = new ShaderObject(ShaderObject::Name::COMPUTE_WORLD, "computeWorld", ShaderObject::Type::COMPUTE);
		assert(pShaderObjectWorld);
		ShaderManager::Add(pShaderObjectWorld);

		ShaderObject* pShaderObjectSkinColor = new ShaderObject(ShaderObject::Name::SKIN_COLOR, "skinSingleColor", ShaderObject::Type::GRAPHICS);
		assert(pShaderObjectSkinColor);
		ShaderManager::Add(pShaderObjectSkinColor);

		ShaderObject* pShaderObjectColor  = new ShaderObject(ShaderObject::Name::CONST_COLOR, "colorRender", ShaderObject::Type::GRAPHICS);
		assert(pShaderObjectColor);
		ShaderManager::Add(pShaderObjectColor);

		ShaderObject* pShaderObjectSkinTex = new ShaderObject(ShaderObject::Name::SKIN_TEXTURE, "skinTexture", ShaderObject::Type::GRAPHICS);
		assert(pShaderObjectSkinTex);
		ShaderManager::Add(pShaderObjectSkinTex);

		
		GraphicsObjectSkinColor* pRunner = (GraphicsObjectSkinColor*)AnimationManager::AddSSBOModel("DemoFiles/humanoidFull.mdl", ModelName::HUMAN, ShaderObject::Name::SKIN_COLOR, color, pos);
		AnimationManager::addAnimation("DemoFiles/Hero_Walk.mdl", pRunner);
		AnimationManager::addAnimation("DemoFiles/Human_Run.mdl", pRunner);
		pRunner->getSkeleton()->setName("Running Man");
		pRunner->getSkeleton()->setPos(Vect(-500, 500.0f, 0.0f));

		GameObject* pColorObject = new GameObjectStatic(pRunner);
		pColorObject->setName("Running Man");
		GameObjectManager::Add(pColorObject, GameObjectManager::getRoot());
		
		
		GraphicsObjectSkinColor* pMan = (GraphicsObjectSkinColor*)AnimationManager::AddAnimSSBOModel("DemoFiles/humanoidFull.mdl", ModelName::HUMAN, ShaderObject::Name::SKIN_COLOR, color, pos);
		pMan->getSkeleton()->setName("HumanSkel1");
		pMan->getSkeleton()->setPos(Vect(-500.0f, -500.0f, 0.0f));
		Matrix M = Matrix(Matrix::Rot1::Z, MATH_PI2);
		pMan->getSkeleton()->setModelRotate(&M);		

		GameObject* pColorObject2 = new GameObjectStatic(pMan);
		pColorObject2->setName("Human1");
		GameObjectManager::Add(pColorObject2, GameObjectManager::getRoot());	

		TextureManager::Add("DemoFiles/TeddyTexNORLE.tga", Texture::Name::TEDDY);

		color = Vect(1, 1, 1);
		GraphicsObjectSkinTexture* pBear = (GraphicsObjectSkinTexture*)AnimationManager::AddAnimSSBOModel("DemoFiles/teddy_TGA.mdl", ModelName::TEDDY, ShaderObject::Name::SKIN_TEXTURE, Texture::Name::TEDDY, color, pos);
		pBear->getSkeleton()->setName("Bear");
		pBear->getSkeleton()->setPos(Vect(-10000.0f, -100.0f, 0.0f));

		GameObject* pTexObject = new GameObjectStatic(pBear);
		pTexObject->setName("Bear");
		GameObjectManager::Add(pTexObject, GameObjectManager::getRoot());

	}

	void Game::Update(float)
	{
		Keyboard* pKey = InputManager::getKeyboard();
		static char prevKey;
		static int count = 100;

		if (pKey->getKeyState(Keyboard::KEY_N))
		{
			if (prevKey != 'n')
			{
				// Change Camera
				CameraManager::Switch(Camera::Type::PERSPECTIVE_3D);
				prevKey = 'n';
			}
		}

		else if (pKey->getKeyState(Keyboard::KEY_1))
		{
			if (prevKey != '1')
			{

				AnimationManager::nextClip();
				prevKey = '1';
			}
		}

		//Camera Movement!
		if (pKey->getKeyState(Keyboard::KEY_S))
		{

			Vect pos;
			Vect tar;
			Vect up;
			Vect upNorm;
			Vect forwardNorm;
			Vect rightNorm;

			CameraManager::getCurrCam(Camera::Type::PERSPECTIVE_3D)->quickGet(up, tar, pos, upNorm, forwardNorm, rightNorm);

			pos -= forwardNorm * 0.5f;

			CameraManager::getCurrCam(Camera::Type::PERSPECTIVE_3D)->quickSet(up, tar, pos);
		}

		else if (pKey->getKeyState(Keyboard::KEY_W))
		{
			Vect pos;
			Vect tar;
			Vect up;
			Vect upNorm;
			Vect forwardNorm;
			Vect rightNorm;

			CameraManager::getCurrCam(Camera::Type::PERSPECTIVE_3D)->quickGet(up, tar, pos, upNorm, forwardNorm, rightNorm);

			pos += forwardNorm * 0.5f;

			CameraManager::getCurrCam(Camera::Type::PERSPECTIVE_3D)->quickSet(up, tar, pos);
		}


		if (pKey->getKeyState(Keyboard::KEY_A))
		{

			Vect pos;
			Vect tar;
			Vect up;
			Vect upNorm;
			Vect forwardNorm;
			Vect rightNorm;

			CameraManager::getCurrCam(Camera::Type::PERSPECTIVE_3D)->quickGet(up, tar, pos, upNorm, forwardNorm, rightNorm);

			tar -= rightNorm * 0.8f;

			CameraManager::getCurrCam(Camera::Type::PERSPECTIVE_3D)->quickSet(up, tar, pos);
		}
		else if (pKey->getKeyState(Keyboard::KEY_D))
		{
			Vect pos;
			Vect tar;
			Vect up;
			Vect upNorm;
			Vect forwardNorm;
			Vect rightNorm;

			CameraManager::getCurrCam(Camera::Type::PERSPECTIVE_3D)->quickGet(up, tar, pos, upNorm, forwardNorm, rightNorm);

			tar += rightNorm * 0.8f;

			CameraManager::getCurrCam(Camera::Type::PERSPECTIVE_3D)->quickSet(up, tar, pos);
		}

		else
		{
			if (count == 0)
			{
				prevKey = ' ';
				count = 100;
			}
			else
			{
				count--;
			}

		}

		CameraManager::getCurrCam(Camera::Type::PERSPECTIVE_3D)->updateCamera();
		CameraManager::getCurrCam(Camera::Type::ORTHOGRAPHIC_2D)->updateCamera();

		AnimationManager::Update();

		// Update the game objects
		GameObjectManager::Update();
	}

	void Game::Draw()
	{
		GameObjectManager::Draw();
	}

	void Game::unloadContent()
	{
		//InputManager::Destroy();
		GameObjectManager::Destroy();
		ModelManager::Destroy();
		ShaderManager::Destroy();
		CameraManager::Destroy();
		TextureManager::Destroy();
		AnimationManager::Destroy();

	}

	void Game::clearBufferFunc()
	{
		const GLfloat grey[] = { 0.7f, 0.7f, 0.8f, 1.0f };
		const GLfloat one = 1.0f;

		glViewport(0, 0, info.windowWidth, info.windowHeight);
		glClearBufferfv(GL_COLOR, 0, grey);
		glClearBufferfv(GL_DEPTH, 0, &one);
	}

}