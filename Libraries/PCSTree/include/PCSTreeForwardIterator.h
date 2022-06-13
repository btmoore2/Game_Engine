#ifndef PCSTREE_FORWARD_ITERATOR_H
#define PCSTREE_FORWARD_ITERATOR_H

#include "PCSTreeIterator.h"

namespace EngineSpace
{
	class PCSTreeForwardIterator : public PCSTreeIterator
	{
	public:
		PCSTreeForwardIterator() = delete;
		PCSTreeForwardIterator(const PCSTreeForwardIterator&) = delete;
		PCSTreeForwardIterator& operator= (const PCSTreeForwardIterator&) = delete;
		virtual ~PCSTreeForwardIterator();

		PCSTreeForwardIterator(PCSNode* rootNode);

		virtual PCSNode* First() override;
		virtual PCSNode* Next() override;
		virtual bool isDone() override;
		virtual PCSNode* currentItem() override;

	private:
		PCSNode* root;
		PCSNode* current;
		PCSNode* end;
	};
}

#endif