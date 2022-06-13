#ifndef PCSTREE_REVERSE_ITERATOR_H
#define PCSTREE_REVERSE_ITERATOR_H

#include "PCSTreeIterator.h"

namespace EngineSpace
{
	class PCSTreeReverseIterator : public PCSTreeIterator
	{
	public:
		PCSTreeReverseIterator() = delete;
		PCSTreeReverseIterator(const PCSTreeReverseIterator&) = delete;
		PCSTreeReverseIterator& operator= (const PCSTreeReverseIterator&) = delete;
		virtual ~PCSTreeReverseIterator() = default;

		PCSTreeReverseIterator(PCSNode* rootNode);

		virtual PCSNode* First() override;
		virtual PCSNode* Next() override;
		virtual bool isDone() override;
		virtual PCSNode* currentItem() override;

	private:
		PCSNode* root;
		PCSNode* current;
	};
}

#endif