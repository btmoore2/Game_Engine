#ifndef BONE_DATA_H
#define BONE_DATA_H

#include "MathEngine.h"

namespace EngineSpace
{
	struct BoneData : public Align16
	{
		Vect T; //Translation 
		Quat R; //Rotation
		Vect S; //Scaling
	};

	//Used to transfer data from the model converter to the engine. 
	struct boneVect
	{
		float x;
		float y;
		float z;
	};

	struct readBone
	{
		boneVect T;
		boneVect Q;
		boneVect S;
	};
}


#endif