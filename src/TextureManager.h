#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#include "Texture.h"

namespace EngineSpace
{
	class TextureManager
	{
	public:
		TextureManager(const TextureManager&) = delete;
		TextureManager& operator= (const TextureManager&) = delete;
		~TextureManager() = default;

		static void Destroy();
		static void Add(const char* const fileName, const Texture::Name texName);
		static void addBuffer(char* texBuff, const Texture::Name texName);
		static void deleteAllTextures();
		static void Remove(Texture::Name texName);
		static void useTex(Texture::Name texName);
		static void disuseTex(Texture::Name texName);
		static GLuint findID(const Texture::Name texName);
		static Texture* Find(const Texture::Name texName);

	private:
		TextureManager();
		static TextureManager* privGetInstance();

		void privLoadTexture(const char* const fileName, GLuint*& texID, int& width, int& height);
		void privLoadTexBuffer(char* texBuff, GLuint*& texID, int& width, int& height);
		void privLoadTGATexture(const char* fileName, GLint minFilter, GLint magFilter, GLint horizWrapMode, GLint vertWrapMode, int& nWidth, int& nHeight);
		void privLoadTGABuffer(char* texBuff, GLint minFilter, GLint magFilter, GLint horizWrapMode, GLint vertWrapMode, int& nWidth, int& nHeight);
		void privAddToFront(Texture* node, Texture*& head);
		GLbyte* gltReadTGABits(const char* fileName, GLint* iWidth, GLint* iHeight, GLint* iComponents, GLenum* eFormat);

		Texture* activeHead;
	};
}

#endif
 