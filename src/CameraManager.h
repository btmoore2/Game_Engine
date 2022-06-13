#ifndef CAMERA_MANAGER_H
#define CAMERA_MANAGER_H

#include "Camera.h"

namespace EngineSpace
{
	class CameraManager
	{
	public: 
		CameraManager(const CameraManager&) = delete;
		CameraManager& operator= (const CameraManager&) = delete;
		~CameraManager() = default;

		static void Add(Camera* pCam);
		static void Create();
		static void Destroy();
		static Camera* getCurrCam(Camera::Type type);
		static void Switch(Camera::Type type);

	private:
		static CameraManager* privGetInstance();
		CameraManager();

		void privAddToFront(Camera* node, Camera*& head);
		void privRemove(Camera* node, Camera*& head);

		//Seperate lists for 2D/3D cameras

		Camera* active3D;
		Camera* active2D;
		Camera* head3D;
		Camera* head2D;
	};
}


#endif
