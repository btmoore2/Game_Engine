#ifndef PCSTREE_H
#define PCSTREE_H

#include "PCSNode.h"

namespace EngineSpace
{
	class PCSTree
	{
	public: 
		struct Info
		{
			int currNumNodes;
			int maxNumNodes;
			int currNumLevels;
			int maxNumLevels;
		};

		PCSTree();
		PCSTree(const PCSTree& in) = delete;
		PCSTree& operator = (const PCSTree& in) = delete;
		~PCSTree();

		PCSNode* GetRoot() const;
		void GetInfo(Info& info);

		void Insert(PCSNode* const pInNode, PCSNode* const pParent);
		void Remove(PCSNode* const pInNode);

	private:
		Info	mInfo;
		PCSNode* pRoot;

	};
}

#endif