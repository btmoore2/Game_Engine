#ifndef MODEL_H
#define MODEL_H

#include "sb7.h"

namespace EngineSpace
{
	class Model
	{
	public:
		struct Vert_xyzuvn
		{
			int index;
			float x;
			float y;
			float z;
			float u;
			float v;
			float nx;
			float ny;
			float nz;
		};

		struct Tri_index
		{
			unsigned int v0;
			unsigned int v1;
			unsigned int v2;
		};

		Model();
		Model(const Model&) = delete;
		Model& operator= (const Model&) = delete;
		virtual ~Model();

		int numVerts;
		int numTris;

		GLuint vao;
		GLuint vbo_0; //Vertex Data - xyzuvn format
		GLuint vbo_1; //Triangle list

	protected:
		virtual void privCreateVAO(const char* const pModelFileName) = 0;
	};
}

#endif