#include "SkinModel.h"
#include "FileModelHdr.h"
#include "File.h"
#include <assert.h>

namespace EngineSpace
{
	struct fBone_Weight
	{
		float fA;
		float fB;
		float fC;
		float fD;
	};

	struct Bone_Index
	{
		int iA;
		int iB;
		int iC;
		int iD;
	};

	struct PoseInvBoneEntry
	{
	public:
		static const unsigned int StringSize = 128;


		struct Mat
		{
			float m0;
			float m1;
			float m2;
			float m3;
			float m4;
			float m5;
			float m6;
			float m7;
			float m8;
			float m9;
			float m10;
			float m11;
			float m12;
			float m13;
			float m14;
			float m15;
		};

		int iBoneIndex;
		char poBoneName[StringSize];
		Mat m;
	};

	void SkinModel::privCreateVAO(const char* const pModelFileName)
	{
		File::Handle fh;
		File::Error  ferror;
		FileModelHdr modelHdr;

		//Open and copy header data from the file
		ferror = File::Open(fh, pModelFileName, File::Mode::READ);
		assert(ferror == File::Error::SUCCESS);
		ferror = File::Read(fh, &modelHdr, sizeof(FileModelHdr));
		assert(ferror == File::Error::SUCCESS);

		//Update the model and allocate the space for the buffers using the data
		this->numVerts = modelHdr.numVerts;
		this->numTris = modelHdr.numTriList;
		this->numBones = (unsigned int)modelHdr.numPose;

		Vert_xyzuvn* pVertData = new Vert_xyzuvn[(unsigned int)modelHdr.numVerts];
		Tri_index* pTriList = new Tri_index[(unsigned int)modelHdr.numTriList];
		Bone_Index* pBoneIndex = new Bone_Index[(unsigned int)modelHdr.numBone];
		fBone_Weight* pBoneWeight = new fBone_Weight[(unsigned int)modelHdr.numBone];
		PoseInvBoneEntry* pInvBone = new PoseInvBoneEntry[(unsigned int)modelHdr.numPose];
		Matrix* pInversePose = new Matrix[this->numBones];
		this->poBone = new Matrix[this->numBones];

		//Copy vertex data
		ferror = File::Seek(fh, File::Location::BEGIN, (int)modelHdr.vertBufferOffset);
		assert(ferror == File::Error::SUCCESS);
		ferror = File::Read(fh, pVertData, sizeof(Vert_xyzuvn) * this->numVerts);
		assert(ferror == File::Error::SUCCESS);

		//Copy triangle data
		ferror = File::Seek(fh, File::Location::BEGIN, (int)modelHdr.triListBufferOffset);
		assert(ferror == File::Error::SUCCESS);
		ferror = File::Read(fh, pTriList, sizeof(Tri_index) * this->numTris);
		assert(ferror == File::Error::SUCCESS);

		//Copy bone index
		ferror = File::Seek(fh, File::Location::BEGIN, (int)modelHdr.indexOffset);
		assert(ferror == File::Error::SUCCESS);
		ferror = File::Read(fh, pBoneIndex, sizeof(Bone_Index) * modelHdr.numBone);
		assert(ferror == File::Error::SUCCESS);

		//Copy bone weight
		ferror = File::Seek(fh, File::Location::BEGIN, (int)modelHdr.weightOffset);
		assert(ferror == File::Error::SUCCESS);
		ferror = File::Read(fh, pBoneWeight, sizeof(fBone_Weight) * modelHdr.numBone);
		assert(ferror == File::Error::SUCCESS);

		//Copy inverse pose
		ferror = File::Seek(fh, File::Location::BEGIN, (int)modelHdr.poseOffset);
		assert(ferror == File::Error::SUCCESS);
		ferror = File::Read(fh, pInvBone, sizeof(PoseInvBoneEntry) * modelHdr.numPose);
		assert(ferror == File::Error::SUCCESS);

		Matrix matIdentity(Matrix::Special::Identity);
		for (int i = 0; i < modelHdr.numPose; i++)
		{
			pInversePose[i] = *(Matrix*)&pInvBone[i].m;
			this->poBone[i] = matIdentity;
		}

		this->poInversePoseSSBO->Set((unsigned int)modelHdr.numPose, sizeof(Matrix), pInversePose);

		//Done with the file, close
		ferror = File::Close(fh);
		assert(ferror == File::Error::SUCCESS);

		//Send the data to the GPU

		//Create VAO
		glGenVertexArrays(1, &this->vao);
		assert(this->vao != 0);
		glBindVertexArray(this->vao);

		//Create VBOs
		glGenBuffers(1, &this->vbo_0);
		assert(this->vbo_0 != 0);
		glGenBuffers(1, &this->vbo_1);
		assert(this->vbo_1 != 0);
		glGenBuffers(1, &this->vboboneweight);
		assert(this->vboboneweight != 0);
		glGenBuffers(1, &this->vboboneindex);
		assert(this->vboboneindex != 0);

		//Load vertex data to 1st buffer
		glBindBuffer(GL_ARRAY_BUFFER, this->vbo_0);
		glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)(sizeof(Vert_xyzuvn) * this->numVerts), pVertData, GL_STATIC_DRAW);

		//Vert data in location: 0
		void* offsetVert = (void*)((unsigned int)&pVertData[0].x - (unsigned int)pVertData);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vert_xyzuvn), offsetVert);
		glEnableVertexAttribArray(0);

		//Texture data in location: 1
		void* offsetTex = (void*)((unsigned int)&pVertData[0].u - (unsigned int)pVertData);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vert_xyzuvn), offsetTex);
		glEnableVertexAttribArray(1);

		//Normal data in location 2
		void* offsetNorm = (void*)((unsigned int)&pVertData[0].nx - (unsigned int)pVertData);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vert_xyzuvn), offsetNorm);
		glEnableVertexAttribArray(2);


		//Load bone weight to 1st buffer
		glBindBuffer(GL_ARRAY_BUFFER, this->vboboneweight);
		glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)(sizeof(fBone_Weight) * modelHdr.numBone), pBoneWeight, GL_STATIC_DRAW);

		//Bone weight data in location 3
		offsetVert = (void*)((unsigned int)&pBoneWeight[0].fA - (unsigned int)pBoneWeight);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(fBone_Weight), offsetVert);
		glEnableVertexAttribArray(3);

		//Load bone index to 1st buffer
		glBindBuffer(GL_ARRAY_BUFFER, this->vboboneindex);
		glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)(sizeof(Bone_Index) * modelHdr.numBone), pBoneIndex, GL_STATIC_DRAW);

		//Bone index data in location 4
		offsetVert = (void*)((unsigned int)&pBoneIndex[0].iA - (unsigned int)pBoneIndex);
		glVertexAttribIPointer(4, 4, GL_UNSIGNED_INT, sizeof(Bone_Index), offsetVert);
		glEnableVertexAttribArray(4);

		//Load triangle data to 2nd buffer
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_1);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)(sizeof(Tri_index) * this->numTris), pTriList, GL_STATIC_DRAW);

		//Clean up
		delete[] pVertData;
		delete[] pTriList;
		delete[] pBoneIndex;
		delete[] pBoneWeight;
		delete[] pInvBone;
		delete[] pInversePose;
	}

	void SkinModel::privCreateTexture(const char* const pModelFileName, Texture::Name texName)
	{
		File::Handle fh;
		File::Error  ferror;
		FileModelHdr modelHdr;

		//Open and copy header data from the file
		ferror = File::Open(fh, pModelFileName, File::Mode::READ);
		assert(ferror == File::Error::SUCCESS);
		ferror = File::Read(fh, &modelHdr, sizeof(FileModelHdr));
		assert(ferror == File::Error::SUCCESS);

		//Only read in if there is actually texture data in the .mdl file
		if (modelHdr.textureSize > 0)
		{
			char* texRead = new char[modelHdr.textureSize];
			ferror = File::Seek(fh, File::Location::BEGIN, (int)modelHdr.textureBufferOffset);
			assert(ferror == File::Error::SUCCESS);
			ferror = File::Read(fh, texRead, modelHdr.textureSize);
			assert(ferror == File::Error::SUCCESS);

			TextureManager::addBuffer(texRead, texName);

			//Done with the texture buffer, delete it
			delete[] texRead;
		}

		ferror = File::Close(fh);
		assert(ferror == File::Error::SUCCESS);
	}

	SkinModel::SkinModel(const char* const pModelFileName)
		: Model()
	{
		assert(pModelFileName);
		this->poInversePoseSSBO = new SSBO();
		this->privCreateVAO(pModelFileName);
	}

	SkinModel::SkinModel(const char* const pModelFileName, Texture::Name texName)
		:Model()
	{
		assert(pModelFileName);
		this->poInversePoseSSBO = new SSBO();
		this->privCreateVAO(pModelFileName);
		this->privCreateTexture(pModelFileName, texName);
	}

	SkinModel::~SkinModel()
	{
		delete[] this->poBone;
		delete this->poInversePoseSSBO;
	}

}