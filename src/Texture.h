#ifndef TEXTURE_H
#define TEXTURE_H

#include "sb7.h"

namespace EngineSpace
{
	class Texture
	{
	public: 
		static const unsigned int ASSET_NAME_SIZE = 64;

		enum class Name
		{
			DEFAULT,
			WARBEAR1,
			WARBEAR2,
			ASTROBOY,
			BUGGY,
			FRIGATE,
			TRUCK,
			DUCKWEED,
			INVADERS,
			TEXT,
			TEDDY,
			NOT_INITALIZED
		};

		Texture();
		Texture(const Texture&) = delete;
		Texture& operator= (const Texture&) = delete;
		~Texture();

		void set(const char* const texName, Texture::Name name, GLuint _textureID, GLenum minFilter, GLenum magFilter, GLenum horizWrapMode, GLenum vertWrapMode, int inHeight, int inWidth);

		Texture::Name name;
		GLuint textureID;
		GLenum minFilter;
		GLenum magFilter;
		GLenum horizWrapMode;
		GLenum vertWrapMode;
		Texture* pNext;
		Texture* pPrev;
		int height;
		int width;
		int useCount;

	private:
		char assetName[Texture::ASSET_NAME_SIZE];
	};
}


#endif
