#include <assert.h>
#include <stdlib.h>
#include "TextureManager.h"
#include "TGAFileHdr.h"
#include "File.h"

namespace EngineSpace
{
	TextureManager::TextureManager()
		:activeHead(nullptr)
	{

	}

	TextureManager* TextureManager::privGetInstance()
	{
		//Singleton lives here
		static TextureManager textMan;
		return &textMan;
	}

	void TextureManager::Destroy()
	{
		//Pull the singleton
		TextureManager* pMan = TextureManager::privGetInstance();
		assert(pMan);

		Texture* pLink = pMan->activeHead;
		Texture* pTemp = nullptr;
		while (pLink != nullptr)
		{
			pTemp = pLink;
			pLink = pLink->pNext;

			delete pTemp;
		}
	}

	void TextureManager::Add(const char* const fileName, const Texture::Name texName)
	{
		//Check if texture has already been loaded, if so no need to duplicate.
		Texture* pNode = TextureManager::Find(texName);

		if (pNode != nullptr)
		{
			//Texture already loaded.
			pNode->useCount++;
			return;
		}

		//Pull the singleton
		TextureManager* pMan = TextureManager::privGetInstance();
		assert(pMan);

		//Load texture and get ID
		int width;
		int height;
		GLuint texID;
		GLuint* pTexID = &texID;
		pMan->privLoadTexture(fileName, pTexID, width, height);

		pNode = new Texture();

		pNode->set(fileName,texName, texID, GL_LINEAR, GL_LINEAR, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, height, width);

		pMan->privAddToFront(pNode, pMan->activeHead);
	}

	void TextureManager::addBuffer(char* texBuff, const Texture::Name texName)
	{
		// Check if texture has already been loaded, if so no need to duplicate.
		Texture* pNode = TextureManager::Find(texName);

		if (pNode != nullptr)
		{
			//Texture already loaded.
			pNode->useCount++;
			return;
		}

		//Pull the singleton
		TextureManager* pMan = TextureManager::privGetInstance();
		assert(pMan);

		//Load texture and get ID
		int width;
		int height;
		GLuint texID;
		GLuint* pTexID = &texID;
		pMan->privLoadTexBuffer(texBuff, pTexID, width, height);

		pNode = new Texture();

		pNode->set(" ", texName, texID, GL_LINEAR, GL_LINEAR, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, height, width);

		pMan->privAddToFront(pNode, pMan->activeHead);
	}

	void TextureManager::Remove(const Texture::Name texName)
	{
		//Pull the singleton
		TextureManager* pMan = TextureManager::privGetInstance();
		assert(pMan);

		Texture* pNode = TextureManager::Find(texName);

		//Verify the node was found. If not, nothing to remove.
		if (pNode != nullptr)
		{
			//Check if anything is using, don't delete textures in use.
			if (pNode->useCount == 0)
			{
				//Nothing is using, okay to delete.
				glDeleteTextures(1, &pNode->textureID);

				if (pNode->pPrev != nullptr)
				{	//Node is middle or last, update backward->forward pointer.
					pNode->pPrev->pNext = pNode->pNext;
				}
				else
				{  //Node was head, update to new head. 
					pMan->activeHead = pNode->pNext;
				}

				if (pNode->pNext != nullptr)
				{	//First or middle, update the forward->backward pointer
					pNode->pNext->pPrev = pNode->pPrev;
				}

				delete pNode;
			}
		}
	}

	Texture* TextureManager::Find(const Texture::Name texName)
	{
		//Pull the singleton
		TextureManager* pMan = TextureManager::privGetInstance();
		assert(pMan);

		Texture* pNode = pMan->activeHead;
		
		while (pNode != nullptr)
		{
			if (pNode->name == texName)
			{
				return pNode;
			}

			pNode = pNode->pNext;

		}

		return nullptr;
	}

	GLuint TextureManager::findID(const Texture::Name texName)
	{
		Texture* pNode = TextureManager::Find(texName);
		
		if (pNode != nullptr)
		{
			//Found, return id. 
			return pNode->textureID;
		}
		
		return 0;
	}


	void TextureManager::useTex(Texture::Name texName)
	{
		Texture* pNode = TextureManager::Find(texName);
		if(pNode != nullptr)
		{
			//Found the texture, update use count.
			pNode->useCount++;
		}
	}

	void TextureManager::disuseTex(Texture::Name texName)
	{
		Texture* pNode = TextureManager::Find(texName);
		if (pNode != nullptr)
		{
			//Found the texture, update use count.
			pNode->useCount--;
		}
	}

	void TextureManager::deleteAllTextures()
	{
		//Pull the singleton
		TextureManager* pMan = TextureManager::privGetInstance();
		assert(pMan);

		Texture* pNode = pMan->activeHead;
		
		//Loop to remove all of the textures
		while (pNode != nullptr)
		{
			glDeleteTextures(1, &pNode->textureID);
			pNode = pNode->pNext;
		}
	}

	void TextureManager::privLoadTexture(const char* const fileName, GLuint*& texID, int& width, int& height)
	{
		//Get ID, Bind it and load texture
		glGenTextures(1, texID);
		glBindTexture(GL_TEXTURE_2D, *texID);

		this->privLoadTGATexture(fileName, GL_LINEAR, GL_LINEAR, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, width, height);
	}

	void TextureManager::privLoadTexBuffer(char* texBuff, GLuint*& texID, int& width, int& height)
	{
		glGenTextures(1, texID);
		glBindTexture(GL_TEXTURE_2D, *texID);

		this->privLoadTGABuffer(texBuff, GL_LINEAR, GL_LINEAR, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, width, height);
	}

	void TextureManager::privLoadTGATexture(const char* fileName, GLint minFilter, GLint magFilter, GLint horizWrapMode, GLint vertWrapMode, int& nWidth, int& nHeight)
	{
		GLbyte* pBits;
		int nComponents;
		GLenum eFormat;

		// Read the texture bits
		pBits = gltReadTGABits(fileName, &nWidth, &nHeight, &nComponents, &eFormat);
		if (pBits == nullptr)
		{
			assert(false);
		}


		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, horizWrapMode);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, vertWrapMode);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexImage2D(GL_TEXTURE_2D, 0, nComponents, nWidth, nHeight, 0, eFormat, GL_UNSIGNED_BYTE, pBits);

		free(pBits);

		if (minFilter == GL_LINEAR_MIPMAP_LINEAR || minFilter == GL_LINEAR_MIPMAP_NEAREST || minFilter == GL_NEAREST_MIPMAP_LINEAR || minFilter == GL_NEAREST_MIPMAP_NEAREST)
		{
			glGenerateMipmap(GL_TEXTURE_2D);
		}
	}


	GLbyte* TextureManager::gltReadTGABits(const char* fileName, GLint* iWidth, GLint* iHeight, GLint* iComponents, GLenum* eFormat)
	{
		TGAHEADER tgaHeader;		// TGA file header
		unsigned long lImageSize;		// Size in bytes of image
		short sDepth;			// Pixel depth;
		GLbyte* pBits = nullptr;          // Pointer to bits

		// Default/Failed values
		*iWidth = 0;
		*iHeight = 0;
		*eFormat = GL_RGB;
		*iComponents = GL_RGB;


		File::Handle fh;
		File::Error ferror;

		ferror = File::Open(fh, fileName, File::Mode::READ);
		assert(ferror == File::Error::SUCCESS);

		ferror = File::Read(fh, &tgaHeader, sizeof(TGAHEADER));
		assert(ferror == File::Error::SUCCESS);

		// Get width, height, and depth of texture
		*iWidth = tgaHeader.width;
		*iHeight = tgaHeader.height;
		sDepth = tgaHeader.bits / 8;

		// We only want 8, 24, or 32 bit targa's.
		if (tgaHeader.bits != 8 && tgaHeader.bits != 24 && tgaHeader.bits != 32)
		{
			assert(false);
		}
		
		// Calculate size of image buffer
		lImageSize = (unsigned int)tgaHeader.width * (unsigned int)tgaHeader.height * (unsigned int)sDepth;

		// Allocate memory and check for success
		pBits = (GLbyte*)malloc(lImageSize * sizeof(GLbyte));
		assert(pBits);
		
		ferror = File::Read(fh, pBits, lImageSize);
		assert(ferror == File::Error::SUCCESS);

		// Set OpenGL format
		switch (sDepth)
		{

		case 3:     // Most likely case
			*eFormat = GL_BGR;
			*iComponents = GL_RGB;
			break;
		case 4:
			*eFormat = GL_BGRA;
			*iComponents = GL_RGBA;
			break;
		case 1:
			assert(false);
			break;
		}

		// Done with File
		File::Close(fh);

		// Return pointer to image data
		return pBits;
	}

	void TextureManager::privLoadTGABuffer(char* texBuff, GLint minFilter, GLint magFilter, GLint horizWrapMode, GLint vertWrapMode, int& nWidth, int& nHeight)
	{
		GLbyte* pBits = nullptr;

		//Set default values. 
		int nComponents = GL_RGB;
		GLenum eFormat = GL_RGB;
		nWidth = 0;
		nHeight = 0;

		TGAHEADER tgaHeader;		// TGA file header
		short sDepth;			

		memcpy(&tgaHeader, &texBuff, sizeof(TGAHEADER));
		// Get width, height, and depth of texture
		nWidth = tgaHeader.width;
		nHeight = tgaHeader.height;
		sDepth = tgaHeader.bits / 8;

		// We only want 8, 24, or 32 bit targa's.
		if (tgaHeader.bits != 8 && tgaHeader.bits != 24 && tgaHeader.bits != 32)
		{
			assert(false);
		}

		//Set to the portion of the buffer with the data. 
		pBits = (GLbyte*)(texBuff + sizeof(TGAHEADER));
		assert(pBits);

		// Set OpenGL format
		switch (sDepth)
		{

		case 3:     // Most likely case
			eFormat = GL_BGR;
			nComponents = GL_RGB;
			break;
		case 4:
			eFormat = GL_BGRA;
			nComponents = GL_RGBA;
			break;
		case 1:
			assert(false);
			break;
		}

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, horizWrapMode);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, vertWrapMode);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexImage2D(GL_TEXTURE_2D, 0, nComponents, nWidth, nHeight, 0, eFormat, GL_UNSIGNED_BYTE, pBits);

		if (minFilter == GL_LINEAR_MIPMAP_LINEAR || minFilter == GL_LINEAR_MIPMAP_NEAREST || minFilter == GL_NEAREST_MIPMAP_LINEAR || minFilter == GL_NEAREST_MIPMAP_NEAREST)
		{
			glGenerateMipmap(GL_TEXTURE_2D);
		}
	}

	

	void TextureManager::privAddToFront(Texture* node, Texture*& head)
	{
		assert(node);

		if (head == nullptr)
		{
			head = node;
			node->pNext = nullptr;
			node->pPrev = nullptr;
		}
		else
		{
			node->pNext = head;
			head->pPrev = node;
			head = node;
		}
	}

}