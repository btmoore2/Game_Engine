#include <assert.h>
#include "ShaderObject.h"
#include "File.h"

namespace EngineSpace
{
	ShaderObject::ShaderObject(ShaderObject::Name shaderName, const char* const pShaderBaseFileName, ShaderObject::Type shaderType)
	:myName(shaderName),myType(shaderType), pNext(nullptr), pPrev(nullptr)
	{
		assert(pShaderBaseFileName);
		this->privCreateShader(this->programObject, pShaderBaseFileName, shaderType);	
	}

	GLint ShaderObject::getLocation(const char* const pUniformName) const
	{
		assert(pUniformName);
		GLint loc = glGetUniformLocation(this->programObject, pUniformName);
		assert(loc != -1); //not found!
		
		return loc;
	}

	void ShaderObject::setActive() const
	{
		glUseProgram(this->programObject);
	}

	void ShaderObject::Dispatch(unsigned int groupsize_x, unsigned int groupsize_y, unsigned int groupsize_z)
	{
		if (this->myType == ShaderObject::Type::COMPUTE)
		{
			glDispatchCompute(groupsize_x, groupsize_y, groupsize_z);
		}
	}

	bool ShaderObject::privLoadCompile(GLuint& shaderObject, const char* const pShaderFileName, GLenum shader_type)
	{
		File::Handle fh;
		File::Error ferror;

		//Read file, setup buffer and close
		assert(pShaderFileName);
		
		ferror = File::Open(fh, pShaderFileName, File::Mode::READ);
		
		assert(ferror == File::Error::SUCCESS);
		
		ferror = File::Seek(fh, File::Location::END, 0);
		assert(ferror == File::Error::SUCCESS);

		DWORD numBytesInFile; 
		ferror = File::Tell(fh, numBytesInFile);
		assert(ferror == File::Error::SUCCESS);

		ferror = File::Seek(fh, File::Location::BEGIN, 0);
		assert(ferror == File::Error::SUCCESS);

		char* data = new char[numBytesInFile + 1];
		assert(data != 0);

		ferror = File::Read(fh, data, numBytesInFile);
		assert(ferror == File::Error::SUCCESS);

		//Add null termination char
		data[numBytesInFile] = 0;

		ferror = File::Close(fh);
		assert(ferror == File::Error::SUCCESS);

		shaderObject = glCreateShader(shader_type);
		assert(shaderObject != 0);

		glShaderSource(shaderObject, 1, &data, 0);
		delete[] data;

		glCompileShader(shaderObject);

		//Verify there were no errors

		GLint status = 0;
		glGetShaderiv(shaderObject, GL_COMPILE_STATUS, &status);

		if (!status)
		{
			//Errors, output data
			char buffer[4096];
			glGetShaderInfoLog(shaderObject, 4096, NULL, buffer);

			printf("Error(ShaderLoadCompile.cpp) %s: %s\n", pShaderFileName, buffer);

			glDeleteShader(shaderObject);

			assert(false);
			return false;
		}
		else
		{
			return true;
		}
	}

	bool ShaderObject::privCreateShader(GLuint& inProgramObject, const char* const pShaderBaseFileName, ShaderObject::Type shaderType)
	{
		bool status;
		char nameBuff[128];

		assert(pShaderBaseFileName);

		if (shaderType == ShaderObject::Type::GRAPHICS)
		{
			strcpy_s(nameBuff, 128, pShaderBaseFileName);
			strcat_s(nameBuff, 128, ".fs.glsl");

			GLuint fs;
			status = privLoadCompile(fs, nameBuff, GL_FRAGMENT_SHADER);
			assert(status == true);

			strcpy_s(nameBuff, 128, pShaderBaseFileName);
			strcat_s(nameBuff, 128, ".vs.glsl");

			GLuint vs;
			status = privLoadCompile(vs, nameBuff, GL_VERTEX_SHADER);
			assert(status == true);

			// Creates an empty program object
			inProgramObject = glCreateProgram();

			//Attaches a shader object to a program object
			glAttachShader(inProgramObject, vs);
			glAttachShader(inProgramObject, fs);

			// Links a program object
			glLinkProgram(inProgramObject);
		}
		else
		{
			strcpy_s(nameBuff, 128, pShaderBaseFileName);
			strcat_s(nameBuff, 128, ".cs.glsl");

			GLuint cs;
			status = privLoadCompile(cs, nameBuff, GL_COMPUTE_SHADER);
			assert(status == true);

			// Creates an empty program object
			inProgramObject = glCreateProgram();

			//Attaches a shader object to a program object
			glAttachShader(inProgramObject, cs);

			// Links a program object
			glLinkProgram(inProgramObject);

			int loc_0;
			loc_0 = glGetUniformLocation(inProgramObject, "tableDepth");

			int loc_1;
			loc_1 = glGetUniformLocation(inProgramObject, "boneCount");
		}
		return true;
	}
}