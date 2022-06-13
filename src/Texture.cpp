#include "Texture.h"

namespace EngineSpace
{
	Texture::Texture()
		:name(Texture::Name::NOT_INITALIZED), textureID(0), minFilter(GL_LINEAR), magFilter(GL_LINEAR), horizWrapMode(GL_CLAMP_TO_EDGE), vertWrapMode(GL_CLAMP_TO_EDGE), pNext(nullptr), pPrev(nullptr), height(-1), width(-1), useCount(0)
	{
		memset(this->assetName, 0, Texture::ASSET_NAME_SIZE);
	}

	Texture::~Texture()
	{

	}

	void Texture::set(const char* const _assetName, Texture::Name _name, GLuint _TextureID, GLenum _minFilter, GLenum _magFilter, GLenum _horizWrapMode, GLenum _vertWrapMode, int inHeight, int inWidth)
	{
		memset(this->assetName, 0x0, Texture::ASSET_NAME_SIZE);
		memcpy(this->assetName, _assetName, Texture::ASSET_NAME_SIZE - 1);
		this->name = _name;
		this->magFilter = _magFilter;
		this->minFilter = _minFilter;
		this->horizWrapMode = _horizWrapMode;
		this->vertWrapMode = _vertWrapMode;
		this->textureID = _TextureID;
		this->height = inHeight;
		this->width = inWidth;
	}
}