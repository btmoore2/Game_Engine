#ifndef SHADER_OBJECT_H
#define SHADER_OBJECT_H

#include "sb7.h"


namespace EngineSpace
{
	class ShaderObject
	{
	public:
		enum class Name
		{
			NULL_SHADER,
			COLOR_LIGHT,
			TEXTURE_SIMPLE,
			TEXTURE_POINT_LIGHT,
			SPRITE,
			CONST_COLOR,
			SKIN_TEXTURE,
			SKIN_COLOR,
			COMPUTE_MIXER,
			COMPUTE_WORLD
		};

		enum class Type
		{
			GRAPHICS,
			COMPUTE
		};

		ShaderObject() = delete;
		ShaderObject(const ShaderObject&) = delete;
		ShaderObject& operator= (const ShaderObject&) = delete;
		virtual ~ShaderObject() = default;

		ShaderObject(ShaderObject::Name shaderName, const char* const pShaderBaseFileName, ShaderObject::Type shaderType);

		void setActive() const;
		GLint getLocation(const char* const pUniformName) const;
		void Dispatch(unsigned int groupsize_x, unsigned int groupsize_y, unsigned int groupsize_z);

		Name myName;
		Type myType;
		GLuint programObject;

		ShaderObject* pNext;
		ShaderObject* pPrev;

	private:
		bool privCreateShader(GLuint& programObject, const char* const pShaderBaseFileName, ShaderObject::Type shaderType);
		bool privLoadCompile(GLuint& shaderObject, const char* const pShaderFileName, GLenum shader_type);





	};
}


#endif

