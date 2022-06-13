#include "PyramidModel.h"
#include "File.h"
#include "FileModelHdr.h"
#include <assert.h>

namespace EngineSpace
{

#define WRITE_DATA_TO_FILE 1
	//This allows a new pyramid file to be created when needed. If already created, it can be changed to 
	//0 and skip the re-creation.

#if WRITE_DATA_TO_FILE

	Model::Tri_index pyramidTriList[] =
	{
		{0,1,2},
		{2,3,0},
		{4,5,6},
		{5,7,6},
		{8,9,10},
		{9,11,10},
		{12,13,14},
		{13,15,14},
		{16,17,18},
		{18,19,16},
		{20,21,22},
		{22,23,20}
	};

	Model::Vert_xyzuvn pyramidVertList[] =
	{
		{1, -0.25f, 0.0f, -0.25f, 0.0f, 0.0f, -0.6f, 0.6f, -0.6f},
		{2, -0.25f, -0.0f, -0.25f, 0.0f, 1.0f, -0.6f, -0.6f, -0.6f},
		{3, 0.25f, -0.0f, -0.25f, 1.0f, 1.0f,  0.6f, -0.6f, -0.6f},
		{4, 0.0f, 1.0f, -0.0f, 1.0f, 0.0f, 0.6f, 0.6f, -0.6f},
		{5, 0.25f, -0.0f, -0.25f, 0.0f, 1.0f, 0.6f, -0.6f, -0.6f},
		{6, 0.25f, -0.0f, 0.25f, 1.0f, 1.0f, 0.6f, -0.6f, 0.6f},
		{7, 0.0f, 1.0f, -0.0f, 0.0f, 0.0f, 0.6f, 0.6f, -0.6f},
		{8, 0.25f, 0.0f, 0.25f, 1.0f, 0.0f, 0.6f, 0.6f, 0.6f},
		{9, 0.25f, -0.0f, 0.25f, 1.0f, 1.0f, 0.6f, -0.6f, 0.6f}, 
		{10, -0.25f, -0.0f, 0.25f, 0.0f, 1.0f, -0.6f, -0.6f, 0.6f}, 
		{11, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.6f, 0.6f, 0.6f}, 
		{12, -0.0f, 1.0f, 0.0f, 0.0f, 0.0f, -0.6f, 0.6f, 0.6f},  
		{13, -0.25f, -0.0f, 0.25f, 0.0f, 0.0f, -0.6f, -0.6f, 0.6f}, 
		{14, -0.25f, -0.0f, -0.25f, 0.0f, 1.0f, -0.6f, -0.6f, -0.6f}, 
		{15, -0.0f, 1.0f, 0.0f, 1.0f, 0.0f, -0.6f, 0.6f, 0.6f},
		{16, -0.0f, 1.0f, -0.0f, 1.0f, 1.0f, -0.6f, 0.6f, -0.6f},
		{17, -0.25f, -0.0f, 0.25f, 0.0f, 0.0f, -0.6f, -0.6f, 0.6f},
		{18, 0.25f, -0.0f, 0.25f, 1.0f, 0.0f, 0.6f, -0.6f, 0.6f},
		{19, 0.25f, -0.0f, -0.25f, 1.0f, 1.0f, 0.6f, -0.6f, -0.6f},
		{20, -0.25f, -0.0f, -0.25f, 0.0f, 1.0f, -0.6f, -0.6f, -0.6f}, 
		{21, -0.0f, 1.0f, -0.0f, 0.0f, 1.0f, -0.6f, 0.6f, -0.6f}, 
		{22, 0.0f, 1.0f, -0.0f, 1.0f, 1.0f, 0.6f, 0.6f, -0.6f}, 
		{23, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.6f, 0.6f, 0.6f}, 
		{24, -0.0f, 1.0f, 0.0f, 0.0f, 0.0f, -0.6f, 0.6f, 0.6f}
	};

#endif

	void PyramidModel::privCreateVAO(const char* const pModelFileName)
	{
		File::Handle fh;
		File::Error ferror;

		FileModelHdr modelHdr;

#if WRITE_DATA_TO_FILE

		//Fill the header 
		this->numVerts = 24;
		this->numTris = 12;

		strncpy_s(modelHdr.objName, FileModelHdr::OBJECT_NAME_SIZE, "Pyramid", _TRUNCATE);

		// vertex buffer
		modelHdr.numVerts = this->numVerts;
		modelHdr.vertBufferOffset = 0;

		// trilist index 
		modelHdr.numTriList = this->numTris;
		modelHdr.triListBufferOffset = 0;

		//Create the data file
		ferror = File::Open(fh, pModelFileName, File::Mode::READ_WRITE);
		assert(ferror == File::Error::SUCCESS);

		//Write the incomplete header
		ferror = File::Write(fh, &modelHdr, sizeof(FileModelHdr));
		assert(ferror == File::Error::SUCCESS);

		//Get the vert offset and write the data
		ferror = File::Tell(fh, modelHdr.vertBufferOffset);
		assert(ferror == File::Error::SUCCESS);
		ferror = File::Write(fh, pyramidVertList, sizeof(Vert_xyzuvn) * this->numVerts);
		assert(ferror == File::Error::SUCCESS);

		//Get the triangle offset and write the data
		ferror = File::Tell(fh, modelHdr.triListBufferOffset);
		assert(ferror == File::Error::SUCCESS);
		ferror = File::Write(fh, pyramidTriList, sizeof(Tri_index) * this->numTris);
		assert(ferror == File::Error::SUCCESS);

		// Finish the write
		ferror = File::Flush(fh);
		assert(ferror == File::Error::SUCCESS);

		// Rewind and overwrite with complete header and close the file.
		ferror = File::Seek(fh, File::Location::BEGIN, 0);
		assert(ferror == File::Error::SUCCESS);
		ferror = File::Write(fh, &modelHdr, sizeof(FileModelHdr));
		assert(ferror == File::Error::SUCCESS);
		ferror = File::Flush(fh);
		assert(ferror == File::Error::SUCCESS);
		ferror = File::Close(fh);
		assert(ferror == File::Error::SUCCESS);

#endif

		//If reading only, starts here
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
		ferror = File::Read(fh, pVertData, (sizeof(Vert_xyzuvn) * this->numVerts));
		assert(ferror == File::Error::SUCCESS);

		//Copy triangle data
		ferror = File::Seek(fh, File::Location::BEGIN, (int)modelHdr.triListBufferOffset);
		assert(ferror == File::Error::SUCCESS);
		ferror = File::Read(fh, pTriList, (sizeof(Tri_index) * this->numTris));
		assert(ferror == File::Error::SUCCESS);

		//Send the data to the GPU

		//Create VAO
		glGenVertexArrays(1, &this->vao);
		assert(this->vao != 0);
		glBindVertexArray(this->vao);

		// Create VBOs
		glGenBuffers(1, &this->vbo_0);
		assert(this->vbo_0 != 0);
		glGenBuffers(1, &this->vbo_1);
		assert(this->vbo_1 != 0);

		//Load vertex data to 1st buffer
		glBindBuffer(GL_ARRAY_BUFFER, this->vbo_0);
		glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)(sizeof(Vert_xyzuvn) * this->numVerts), pVertData, GL_STATIC_DRAW);

		//Vert data in location: 0  (for vertex shader)
		void* offsetVert = (void*)((unsigned int)&pVertData[0].x - (unsigned int)pVertData);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vert_xyzuvn), offsetVert);
		glEnableVertexAttribArray(0);

		//Texture data in location: 1  (for vertex shader)
		void* offsetTex = (void*)((unsigned int)&pVertData[0].u - (unsigned int)pVertData);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vert_xyzuvn), offsetTex);
		glEnableVertexAttribArray(1);

		//Normal data in location 2 (for vertex shader)
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

	PyramidModel::PyramidModel(const char* const pModelFileName)
		:Model()
	{
		assert(pModelFileName);
		this->privCreateVAO(pModelFileName);
	}

	PyramidModel::~PyramidModel()
	{

	}

}