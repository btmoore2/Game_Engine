#ifndef CAMERA_H
#define CAMERA_H

#include "MathEngine.h"

namespace EngineSpace
{
	class Camera : public Align16
	{
	public:

		enum class Type
		{
			PERSPECTIVE_3D,
			ORTHOGRAPHIC_2D
		};

		Camera();
		Camera(const Camera&) = delete;
		Camera& operator= (const Camera&) = delete;
		~Camera();

		Camera(Camera::Type type);

		void updateCamera();

		void setPerspective(const float FieldofView_Degs, const float AspectRatio, const float nearDist, const float farDist);
		void setOrthographic(const float xMin, const float xMax, const float yMin, const float yMax, const float zMin, const float zMax);
		void setViewport(const int inX, const int inY, const int width, const int height);
		void setOrientAndPosition(const Vect& upVect, const Vect& lookPoint, const Vect& posPoint);
		void setFieldOfView(const float Value);
		void setNearDist(const float Value);
		void quickSet(Vect& up, Vect& target, Vect& pos);

		Matrix& getViewMatrix();
		Matrix& getProjMatrix();
		Camera::Type getType();
		void getPos(Vect& outPos) const;
		void getDir(Vect& outDir) const;
		void getUp(Vect& outUp) const;
		void getLookAt(Vect& outLookAt) const;
		void getRight(Vect& outRight) const;
		int getScreenWidth() const;
		int getScreenHeight() const;
		void getFieldOfView(float& Value) const;
		void getNearDist(float& Value) const;
		void getNearTopLeft(Vect& vOut) const;
		void getNearTopRight(Vect& vOut) const;
		void getNearBottomLeft(Vect& vOut) const;
		void getNearBottomRight(Vect& vOut) const;
		void getFarTopLeft(Vect& vOut) const;
		void getFarTopRight(Vect& vOut) const;
		void getFarBottomLeft(Vect& vOut) const;
		void getFarBottomRight(Vect& vOut) const;
		void quickGet(Vect& up, Vect& target, Vect& pos, Vect& uNorm, Vect& fNorm, Vect& rNorm);

		Camera* pNext;
		Camera* pPrev;

	private: 
		void privSetViewState();
		void privCalcPlaneHeightWidth();
		void privCalcFrustumVerts();
		void privCalcFrustumCollisionNormals();
		void privUpdateProjectionMatrix();
		void privUpdateViewMatrix();

		Matrix projMatrix;
		Matrix viewMatrix;

		Vect vUp;
		Vect vDir;
		Vect vRight;  
		Vect vPos;

		Vect vLookAt;

		// world space coords for viewing frustum
		Vect nearTopLeft;
		Vect nearTopRight;
		Vect nearBottomLeft;
		Vect nearBottomRight;
		Vect farTopLeft;
		Vect farTopRight;
		Vect farBottomLeft;
		Vect farBottomRight;

		// Normals of the frustum
		Vect frontNorm;
		Vect backNorm;
		Vect rightNorm;
		Vect leftNorm;
		Vect topNorm;
		Vect bottomNorm;

		Camera::Type camType;

		// Frustum inputs
		float nearDist;
		float farDist;
		float fovy;
		float aspectRatio;

		// Screen size in world space
		float nearHeight;
		float nearWidth;
		float farHeight;
		float farWidth;

		//Ortho Data
		float xMin;
		float yMin;
		float zMin;
		float xMax;
		float yMax;
		float zMax;

		// viewports
		int	viewportX;
		int	viewportY;
		int	viewportWidth;
		int	viewportHeight;

	};
}


#endif
