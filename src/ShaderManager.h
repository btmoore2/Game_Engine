#ifndef SHADER_MANAGER_H
#define SHADER_MANAGER_H

#include "ShaderObject.h"

namespace EngineSpace
{
	class ShaderManager
	{
	public: 
		static void Add(ShaderObject* pShader);
		static void Create();
		static void Destroy();
		static ShaderObject* Find(ShaderObject::Name Target);

	private:
		static ShaderManager* privGetInstance();
		ShaderManager();

		void privAddToFront(ShaderObject* pNode, ShaderObject*& pHead);
		void privRemove(ShaderObject* pNode, ShaderObject*& pHead);

		ShaderObject* pActiveHead;
	};
}


#endif

