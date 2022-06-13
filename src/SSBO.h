#ifndef SSBO_H
#define SSBO_H

#include "sb7.h"

namespace EngineSpace
{
	class SSBO
	{
	public: 
		enum class Access
		{
			READ_ONLY = GL_READ_ONLY,
			WRITE_ONLY = GL_WRITE_ONLY,
			READ_WRITE = GL_READ_WRITE
		};

		SSBO();
		SSBO(const SSBO&) = delete;
		const SSBO& operator= (const SSBO&) = delete;
		~SSBO();

		void Set(unsigned int newCount, unsigned int newSize, void* inData);
		unsigned int getLocation() const;
		void Bind(unsigned int bindLocation);
		void* Map(SSBO::Access accessType);
		void unMap();

	private:
		GLuint ssbo;
		unsigned int elementCount;
		unsigned int elementSize;
		void* poBuffer;

	};
}


#endif
