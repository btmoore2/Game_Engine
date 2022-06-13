#ifndef PCSNODE_H
#define PCSNODE_H

namespace EngineSpace
{
	class PCSNode
	{
	public:
		static const unsigned int NAME_SIZE = 32;
		static const unsigned int MAJOR_VERSION = 2;
		static const unsigned int MINOR_VERSION = 3;

		enum class Code
		{
			SUCCESS,
			FAIL_NULL_PTR,
			FAIL_RETURN_NOT_INITIALIZED,
			DWORD= 0x7fffffff
		};

		PCSNode();
		PCSNode(const PCSNode& inNode);
		PCSNode& operator= (const PCSNode& inNode);
		virtual ~PCSNode();

		PCSNode(PCSNode* const pInParent, PCSNode* const pInChild, PCSNode* const pInNextSibling, PCSNode* const pInPrevSibling, const char* const pInName);
		PCSNode(const char* const pInName);

		void setParent(PCSNode* const pIn);
		void setChild(PCSNode* const pIn);
		void setNextSibling(PCSNode* const pIn);
		void setPrevSibling(PCSNode* const pIn);
		void setForward(PCSNode* const pIn);
		void setReverse(PCSNode* const pIn);
		Code setName(const char* const pInName);


		PCSNode* getParent(void) const;
		PCSNode* getChild(void) const;
		PCSNode* getNextSibling(void) const;
		PCSNode* getPrevSibling(void) const;
		PCSNode* getForward(void) const;
		PCSNode* getReverse(void) const;
		int getNumSiblings() const;
		int getNumChildren() const;
		Code getName(char* const pOutBuffer, unsigned int sizeOutBuffer) const;

	private:
		PCSNode* pParent;
		PCSNode* pChild;
		PCSNode* pNextSibling;
		PCSNode* pPrevSibling;
		PCSNode* pForward;
		PCSNode* pReverse;

		char pName[PCSNode::NAME_SIZE];
	};
}


#endif
