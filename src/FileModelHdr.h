#ifndef FILE_MODEL_HDR_H
#define FILE_MODEL_HDR_H

#include <Windows.h>

namespace EngineSpace
{
	struct FileModelHdr
	{
	public:
		static const unsigned int OBJECT_NAME_SIZE = 32;
		static const unsigned int TEXTURE_NAME_SIZE = 32;

		FileModelHdr();
		FileModelHdr(const FileModelHdr&) = delete;
		FileModelHdr& operator= (const FileModelHdr&) = delete;
		~FileModelHdr() = default;

		char objName[OBJECT_NAME_SIZE]; //Object name array
		
		int numVerts; //Number of vertices to read in
		DWORD vertBufferOffset; //Offset for start of vertex data
		
		int numTriList; //Number of triangles
		DWORD triListBufferOffset;  //Offset for start of triangle data

		char textName[TEXTURE_NAME_SIZE]; //Texture name arry
		DWORD textureBufferOffset; //Offset for start of texture data
		DWORD textureSize; //Size of texture

		DWORD sphereVertBufferOffset; //Offset for start of bounding sphere verticies
		DWORD sphereTriListBufferOffset; //Offset for start of bounding sphere triangles

		int numData; //Skeleton bones
		DWORD skeletonOffset; //Offset for start of bone data

		int depth; //Number of levels
		DWORD hierOffset; //Offset for hierarchy data

		int numAnim; //Number of animations
		DWORD animOffset; //Offset for animation data

		int numBone; //Number of bones
		DWORD weightOffset; //Offset for bone weight data
		DWORD indexOffset; //Offset for index data

		int numPose; //Number of poses
		DWORD poseOffset; //Offset for pose data
	};
}

#endif