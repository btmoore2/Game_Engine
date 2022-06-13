#include "FileModel.h"
#include "FileModelHdr.h"
#include "File.h"
#include <assert.h>

namespace EngineSpace
{
	FileModel::FileModel(const char* const pModelFileName)
		: Model()
	{
		assert(pModelFileName);
		this->privCreateVAO(pModelFileName);
	}

	FileModel::~FileModel()
	{

	}

	void FileModel::privCreateVAO(const char* const pModelFileName)
	{
		File::Handle fh;
		File::Error ferror;

		FileModelHdr modelHdr;

		//Open and copy header data from the file
		ferror = File::Open(fh, pModelFileName, File::Mode::READ);
		assert(ferror == File::Error::SUCCESS);
		ferror = File::Read(fh, &modelHdr, sizeof(FileModelHdr));
		assert(ferror == File::Error::SUCCESS);

		//Update the model and allocate the space for the buffers using the data
		this->numVerts = modelHdr.numVerts;
		this->numTris = modelHdr.numTriList;

		Vert_xyzuvn* pVertData = new Vert_xyzuvn[(unsigned int)this->numVerts];
		Tri_index* pTriList = new Tri_index[(unsigned int)this->numTris];

		//Copy vertex data
		ferror = File::Seek(fh, File::Location::BEGIN, (int)modelHdr.vertBufferOffset);
		assert(ferror == File::Error::SUCCESS);
		ferror = File::Read(fh, &pVertData, (sizeof(Vert_xyzuvn) * this->numVerts));
		assert(ferror == File::Error::SUCCESS);

		//Copy triangle data
		ferror = File::Seek(fh, File::Location::BEGIN, (int)modelHdr.triListBufferOffset);
		assert(ferror == File::Error::SUCCESS);
		ferror = File::Read(fh, pTriList, (sizeof(Tri_index) * this->numTris));
		assert(ferror == File::Error::SUCCESS);

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

		//Load vertex data to 1st buffer
		glBindBuffer(GL_ARRAY_BUFFER, this->vbo_0);
		glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)(sizeof(Vert_xyzuvn) * this->numVerts), pVertData, GL_STATIC_DRAW);

		//Vert data in location 0
		void* offsetVert = (void*)((unsigned int)&pVertData[0].x - (unsigned int)pVertData);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vert_xyzuvn), offsetVert);
		glEnableVertexAttribArray(0);

		//Texture data in location 1
		void* offsetTex = (void*)((unsigned int)&pVertData[0].u - (unsigned int)pVertData);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vert_xyzuvn), offsetTex);
		glEnableVertexAttribArray(1);

		//Normal data in location 2
		void* offsetNorm = (void*)((unsigned int)&pVertData[0].nx - (unsigned int)pVertData);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vert_xyzuvn), offsetNorm);
		glEnableVertexAttribArray(2);

		//Done with the vert buffer, delete
		delete[] pVertData;

		//Load triangle data to 2nd buffer
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_1);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)(sizeof(Tri_index) * this->numTris), pTriList, GL_STATIC_DRAW);

		//Done, delete triangle buffer
		delete[] pTriList;

	}
}
