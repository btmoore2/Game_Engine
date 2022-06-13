#include "Camera.h"
#include <assert.h>
#include "sb7.h"

namespace EngineSpace
{
	Camera::Camera()
		:pNext(nullptr), pPrev(nullptr), camType(Camera::Type::PERSPECTIVE_3D), nearDist(0.0f), farDist(0.0f), fovy(0.0f), aspectRatio(0.0f), nearHeight(0.0f), nearWidth(0.0f), farHeight(0.0f), farWidth(0.0f), xMin(0.0f), yMin(0.0f), zMin(0.0f), xMax(0.0f), yMax(0.0f), zMax(0.0f), viewportX(0), viewportY(0), viewportWidth(0), viewportHeight(0)
	{
		
			
	}

	Camera::Camera(Camera::Type type)
		: pNext(nullptr), pPrev(nullptr), camType(type), nearDist(0.0f), farDist(0.0f), fovy(0.0f), aspectRatio(0.0f), nearHeight(0.0f), nearWidth(0.0f), farHeight(0.0f), farWidth(0.0f), xMin(0.0f), yMin(0.0f), zMin(0.0f), xMax(0.0f), yMax(0.0f), zMax(0.0f), viewportX(0), viewportY(0), viewportWidth(0), viewportHeight(0)
	{

	}

	Camera::~Camera()
	{

	}

	void Camera::updateCamera()
	{
		// First find the near height/width, far height/width
		this->privCalcPlaneHeightWidth();

		// Find the frustum physical verts & collision normals
		this->privCalcFrustumVerts();
		this->privCalcFrustumCollisionNormals();

		// update the projection & view matrix
		this->privUpdateProjectionMatrix();
		this->privUpdateViewMatrix();
	}

	void Camera::setPerspective(const float Fovy, const float Aspect, const float NearDist, const float FarDist)
	{
		assert(this->camType == Camera::Type::PERSPECTIVE_3D);
		
		this->aspectRatio = Aspect;
		this->fovy = Fovy;
		this->nearDist = NearDist;
		this->farDist = FarDist;
	}

	void Camera::setOrthographic(const float newXMin, const float newXMax, const float newYMin, const float newYMax, const float newZMin, const float newZMax)
	{
		assert(this->camType == Camera::Type::ORTHOGRAPHIC_2D);

		this->xMin = newXMin;
		this->xMax = newXMax;
		this->yMin = newYMin;
		this->yMax = newYMax;
		this->zMin = newZMin;
		this->zMax = newZMax;

		this->farHeight = this->yMax = this->yMin;
		this->farWidth = this->xMax = this->xMin;
		this->nearWidth = this->yMax - this->yMin;
		this->nearDist = this->zMin;
		this->farDist = this->zMax;
	}

	void Camera::setViewport(const int inX, const int inY, const int width, const int height)
	{
		this->viewportX = inX;
		this->viewportY = inY;
		this->viewportWidth = width;
		this->viewportHeight = height;

		this->privSetViewState();
	};

	void Camera::setOrientAndPosition(const Vect& upVect, const Vect& lookPoint, const Vect& posPoint)
	{
		this->vLookAt = lookPoint;
		this->vDir = posPoint - lookPoint;
		this->vDir.norm();

		this->vRight = upVect.cross(this->vDir);
		this->vRight.norm();

		this->vUp = this->vDir.cross(this->vRight);
		this->vUp.norm();

		this->vPos = posPoint;
	}

	void Camera::setFieldOfView(const float Value)
	{
		this->aspectRatio = Value;
	};

	void Camera::setNearDist(const float Value)
	{
		this->nearDist = Value;
	}

	void Camera::quickSet(Vect& up, Vect& target, Vect& pos)
	{
		Vect upVect = up - pos;
		this->setOrientAndPosition(upVect, target, pos);
	}

	Matrix& Camera::getViewMatrix(void)
	{
		return this->viewMatrix;
	}

	Matrix& Camera::getProjMatrix(void)
	{
		return this->projMatrix;
	}

	Camera::Type Camera::getType()
	{
		return this->camType;
	}

	void Camera::getPos(Vect& outPos) const
	{
		outPos = this->vPos;
	}

	void  Camera::getDir(Vect& outDir) const
	{
		outDir = this->vDir;
	}

	void  Camera::getUp(Vect& outUp) const
	{
		outUp = this->vUp;
	}

	void Camera::getLookAt(Vect& outLookAt) const
	{
		outLookAt = this->vLookAt;
	}

	void Camera::getRight(Vect& outRight) const
	{
		outRight = this->vRight;
	}

	int Camera::getScreenWidth() const
	{
		return this->viewportWidth;
	}

	int Camera::getScreenHeight() const
	{
		return this->viewportHeight;
	}

	void Camera::getFieldOfView(float& Value) const
	{
		Value = this->aspectRatio;
	};

	void Camera::getNearDist(float& Value) const
	{
		Value = this->nearDist;
	}

	void Camera::getNearTopLeft(Vect& vOut) const
	{
		vOut = this->nearTopLeft;
	}
	void Camera::getNearTopRight(Vect& vOut) const
	{
		vOut = this->nearTopRight;
	}
	void Camera::getNearBottomLeft(Vect& vOut)const
	{
		vOut = this->nearBottomLeft;
	}
	void Camera::getNearBottomRight(Vect& vOut) const
	{
		vOut = this->nearBottomRight;
	}
	void Camera::getFarTopLeft(Vect& vOut) const
	{
		vOut = this->farTopLeft;
	}
	void Camera::getFarTopRight(Vect& vOut) const
	{
		vOut = this->farTopRight;
	}
	void Camera::getFarBottomLeft(Vect& vOut) const
	{
		vOut = this->farBottomLeft;
	}
	void Camera::getFarBottomRight(Vect& vOut)const
	{
		vOut = this->farBottomRight;
	}

	void Camera::quickGet(Vect& up, Vect& target, Vect& pos, Vect& uNorm, Vect& fNorm, Vect& rNorm)
	{
		this->getPos(pos);
		this->getLookAt(target);
		this->getUp(uNorm);
		up = pos + uNorm;

		fNorm = target - pos;
		fNorm.norm();

		this->getRight(rNorm);
	}

	void Camera::privSetViewState()
	{
		glViewport(this->viewportX, this->viewportY, this->viewportWidth, this->viewportHeight);
	}

	void Camera::privCalcPlaneHeightWidth()
	{
		this->nearHeight = 2.0f * tanf((this->fovy * MATH_PI / 180.0f) * .5f) * this->nearDist;
		this->nearWidth = this->nearHeight * this->aspectRatio;

		this->farHeight = 2.0f * tanf((this->fovy * MATH_PI / 180.0f) * .5f) * this->farDist;
		this->farWidth = this->farHeight * this->aspectRatio;
	}

	void Camera::privCalcFrustumVerts()
	{
		this->nearTopLeft = this->vPos - this->vDir * this->nearDist + this->vUp * this->nearHeight * 0.5f - this->vRight * this->nearWidth * 0.5f;
		this->nearTopRight = this->vPos - this->vDir * this->nearDist + this->vUp * this->nearHeight * 0.5f + this->vRight * this->nearWidth * 0.5f;
		this->nearBottomLeft = this->vPos - this->vDir * this->nearDist - this->vUp * this->nearHeight * 0.5f - this->vRight * this->nearWidth * 0.5f;
		this->nearBottomRight = this->vPos - this->vDir * this->nearDist - this->vUp * this->nearHeight * 0.5f + this->vRight * this->nearWidth * 0.5f;
		this->farTopLeft = this->vPos - this->vDir * this->farDist + this->vUp * this->farHeight * 0.5f - this->vRight * this->farWidth * 0.5f;
		this->farTopRight = this->vPos - this->vDir * this->farDist + this->vUp * this->farHeight * 0.5f + this->vRight * this->farWidth * 0.5f;
		this->farBottomLeft = this->vPos - this->vDir * this->farDist - this->vUp * this->farHeight * 0.5f - this->vRight * this->farWidth * 0.5f;
		this->farBottomRight = this->vPos - this->vDir * this->farDist - this->vUp * this->farHeight * 0.5f + this->vRight * this->farWidth * 0.5f;
	}

	void Camera::privCalcFrustumCollisionNormals(void)
	{
		// Normals of the frustum around nearTopLeft
		Vect A = this->nearBottomLeft - this->nearTopLeft;
		Vect B = this->nearTopRight - this->nearTopLeft;
		Vect C = this->farTopLeft - this->nearTopLeft;

		this->frontNorm = A.cross(B);
		this->frontNorm.norm();

		this->leftNorm = C.cross(A);
		this->leftNorm.norm();

		this->topNorm = B.cross(C);
		this->topNorm.norm();

		// Normals of the frustum around farBottomRight
		A = this->farBottomLeft - this->farBottomRight;
		B = this->farTopRight - this->farBottomRight;
		C = this->nearBottomRight - this->farBottomRight;

		this->backNorm = A.cross(B);
		this->backNorm.norm();

		this->rightNorm = B.cross(C);
		this->rightNorm.norm();

		this->bottomNorm = C.cross(A);
		this->bottomNorm.norm();
	};

	void Camera::privUpdateProjectionMatrix()
	{
		if (this->camType == Camera::Type::PERSPECTIVE_3D)
		{
			this->projMatrix[m0] = 2.0f * this->nearDist / this->nearWidth;
			this->projMatrix[m1] = 0.0f;
			this->projMatrix[m2] = 0.0f;
			this->projMatrix[m3] = 0.0f;

			this->projMatrix[m4] = 0.0f;
			this->projMatrix[m5] = 2.0f * this->nearDist / this->nearHeight;
			this->projMatrix[m6] = 0.0f;
			this->projMatrix[m7] = 0.0f;

			this->projMatrix[m8] = 0.0f;
			this->projMatrix[m9] = 0.0f;
			this->projMatrix[m10] = (this->farDist + this->nearDist) / (this->nearDist - this->farDist);
			this->projMatrix[m11] = -1.0f;

			this->projMatrix[m12] = 0.0f;
			this->projMatrix[m13] = 0.0f;
			this->projMatrix[m14] = (2.0f * this->farDist * this->nearDist) / (this->nearDist - this->farDist);
			this->projMatrix[m15] = 0.0f;
		}
		else
		{
			this->projMatrix[m0] = 2.0f / (xMax - xMin);
			this->projMatrix[m1] = 0.0f;
			this->projMatrix[m2] = 0.0f;
			this->projMatrix[m3] = 0.0f;

			this->projMatrix[m4] = 0.0f;
			this->projMatrix[m5] = 2.0f / (yMax - yMin);
			this->projMatrix[m6] = 0.0f;
			this->projMatrix[m7] = 0.0f;

			this->projMatrix[m8] = 0.0f;
			this->projMatrix[m9] = 0.0f;
			this->projMatrix[m10] = -2.0f / (zMax - zMin);
			this->projMatrix[m11] = 0.0f;

			this->projMatrix[m12] = 0;
			this->projMatrix[m13] = 0;
			this->projMatrix[m14] = -(zMax + zMin) / (zMax - zMin);
			this->projMatrix[m15] = 1.0f;
		}
	};

	void Camera::privUpdateViewMatrix()
	{
		//Assumes perpendicular vects
		this->viewMatrix[m0] = this->vRight[x];
		this->viewMatrix[m1] = this->vUp[x];
		this->viewMatrix[m2] = this->vDir[x];
		this->viewMatrix[m3] = 0.0f;
		this->viewMatrix[m4] = this->vRight[y];
		this->viewMatrix[m5] = this->vUp[y];
		this->viewMatrix[m6] = this->vDir[y];
		this->viewMatrix[m7] = 0.0f;
		this->viewMatrix[m8] = this->vRight[z];
		this->viewMatrix[m9] = this->vUp[z];
		this->viewMatrix[m10] = this->vDir[z];
		this->viewMatrix[m11] = 0.0f;
		this->viewMatrix[m12] = -vPos.dot(vRight);
		this->viewMatrix[m13] = -vPos.dot(vUp);
		this->viewMatrix[m14] = -vPos.dot(vDir);
		this->viewMatrix[m15] = 1.0f;
	};




}

