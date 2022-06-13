#ifndef ANIMATION_MANAGER_H
#define ANIMATION_MANAGER_H

#include "AnimationController.h"
#include "ModelManager.h"
#include "Texture.h"
#include "GraphicsObjectSkin.h"


namespace EngineSpace
{

	class AnimationManager
	{
	public:
		AnimationManager(const AnimationManager&) = delete;
		AnimationManager& operator= (const AnimationManager&) = delete;
		~AnimationManager();

		static GraphicsObject* AddAnimSSBOModel(const char* const pModelFileName, ModelName inName, ShaderObject::Name gShader, Vect Light, Vect Pos);
		static GraphicsObject* AddAnimSSBOModel(const char* const pModelFileName, ModelName inName, ShaderObject::Name gShader, Texture::Name tName, Vect Light, Vect Pos);
		static GraphicsObject* AddSSBOModel(const char* const pModelFileName, ModelName inName, ShaderObject::Name gShader, Vect Light, Vect Pos);
		static GraphicsObject* AddSSBOModel(const char* const pModelFileName, ModelName inName, ShaderObject::Name gShader, Texture::Name tName, Vect Light, Vect Pos);
		static void addAnimation(const char* const pModelFileName, GraphicsObjectSkin* addTo);
		static void Create();
		static void Destroy();
		static void Update();
		static void nextClip();
		static void Faster();
		static void Slower();
		static void Reverse();
		static void Pause();


	private:
		static AnimationManager* privGetInstance();
		AnimationManager();

		AnimationController* poControllerHead;
		bool pauseAnimation;

	};

}



#endif
