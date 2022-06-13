#ifndef GRAPHICS_OBJECT_SKIN_H
#define GRAPHICS_OBJECT_SKIN_H

#include "GraphicsObject.h"
#include "GameObject.h"
#include "SSBO.h"
#include "AnimationController.h"

namespace EngineSpace
{
	class GraphicsObjectSkin : public GraphicsObject
	{
	public: 
		GraphicsObjectSkin() = delete;
		GraphicsObjectSkin(const GraphicsObjectSkin&) = delete;
		GraphicsObjectSkin& operator= (const GraphicsObjectSkin&) = delete;
		virtual ~GraphicsObjectSkin();

		GraphicsObjectSkin(const Model* const newModel, const ShaderObject* const newShader);

		void setSkeleton(GameObject* newSkel);
		void setController(AnimationController* newController);
		GameObject* getSkeleton();
		SSBO* getSSBO();
		AnimationController* getController();		

	protected:
		GameObject* pSkelHead;
		SSBO* pBoneWorldSSBO;
		AnimationController* pController;
	};
}


#endif
