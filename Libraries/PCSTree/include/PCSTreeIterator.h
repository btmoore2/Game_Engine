#ifndef PCSTREE_ITERATOR_H
#define PCSTREE_ITERATOR_H

#include "PCSNode.h"

namespace EngineSpace
{
	class PCSTreeIterator
	{
	public:
		virtual PCSNode* First() = 0;
		virtual PCSNode* Next() = 0;
		virtual bool isDone() = 0;
		virtual PCSNode* currentItem() = 0;

		virtual ~PCSTreeIterator() = default;
	};
}

#endif