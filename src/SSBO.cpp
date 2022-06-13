#include "SSBO.h"
#include <assert.h>

namespace EngineSpace
{
	SSBO::SSBO()
		:ssbo(), elementCount(0), elementSize(0), poBuffer(nullptr)
	{
		glGenBuffers(1, &ssbo);
	}

	SSBO::~SSBO()
	{
		this->poBuffer = nullptr;
		//release buffers
		glDeleteBuffers(1, &ssbo);
	}

	void SSBO::Set(unsigned int newCount, unsigned int newSize, void* pData)
	{
		this->elementCount = newCount;
		this->elementSize = newSize;
		this->poBuffer = pData;

		glBindBuffer(GL_SHADER_STORAGE_BUFFER, this->ssbo);
		glBufferData(GL_SHADER_STORAGE_BUFFER, (GLsizeiptr)(this->elementCount * this->elementSize), this->poBuffer, GL_STATIC_DRAW);
	}

	unsigned int SSBO::getLocation() const
	{
		return this->ssbo;
	}

	void SSBO::Bind(unsigned int bindLocation)
	{
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, bindLocation, this->ssbo);
	}

	void* SSBO::Map(SSBO::Access accessType)
	{
		void* result = glMapNamedBuffer(this->ssbo, GLenum(accessType));
		assert(result);

		return result;
	}

	void SSBO::unMap()
	{
		glUnmapNamedBuffer(this->ssbo);
	}
}