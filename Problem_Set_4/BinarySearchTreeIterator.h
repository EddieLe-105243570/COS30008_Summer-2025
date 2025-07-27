// COS30008, Problem Set 4, Problem 3, 2022
#pragma once
#include "BinarySearchTree.h"
#include <stack>

template <typename T>
class BinarySearchTreeIterator
{
private:
	using BSTree = BinarySearchTree<T>;
	using BNode = BinaryTreeNode<T>;
	using BTreeNode = BNode *; // FIX 1: Should be pointer type
	using BTNStack = std::stack<BTreeNode>;

	const BSTree &fBSTree; // binary search tree
	BTNStack fStack;	   // DFS traversal stack

	void pushLeft(BTreeNode aNode);

public:
	using Iterator = BinarySearchTreeIterator<T>;

	BinarySearchTreeIterator(const BSTree &aBSTree);
	const T &operator*() const; // FIX 2: Should be operator* not operator()
	Iterator &operator++();
	Iterator operator++(int);
	bool operator==(const Iterator &aOtherIter) const;
	bool operator!=(const Iterator &aOtherIter) const;
	Iterator begin() const;
	Iterator end() const;
};

// Helper method: Push all left nodes onto stack
template <typename T>
void BinarySearchTreeIterator<T>::pushLeft(BTreeNode aNode)
{
	while (!aNode->empty())
	{
		fStack.push(aNode);
		aNode = aNode->left;
	}
}

// Constructor - initialize iterator to begin position
template <typename T>
BinarySearchTreeIterator<T>::BinarySearchTreeIterator(const BSTree &aBSTree)
	: fBSTree(aBSTree)
{
	// Start traversal from root, pushing all left nodes
	if (!fBSTree.fRoot->empty())
	{
		pushLeft(fBSTree.fRoot);
	}
}

// Dereference operator - return current key
template <typename T>
const T &BinarySearchTreeIterator<T>::operator*() const
{
	if (fStack.empty())
	{
		throw std::runtime_error("Iterator out of bounds");
	}
	return fStack.top()->key;
}

// Pre-increment operator
template <typename T>
typename BinarySearchTreeIterator<T>::Iterator &BinarySearchTreeIterator<T>::operator++()
{
	if (!fStack.empty())
	{
		BTreeNode current = fStack.top();
		fStack.pop();
		// If current node has right subtree, push its leftmost path
		if (!current->right->empty())
		{
			pushLeft(current->right);
		}
	}
	return *this; // FIX 3: Should be *this not this
}

// Post-increment operator
template <typename T>
typename BinarySearchTreeIterator<T>::Iterator BinarySearchTreeIterator<T>::operator++(int)
{
	Iterator temp(*this); // FIX 4: Should be *this not this
	++(*this);			  // FIX 5: Should be ++(*this) not ++(this)
	return temp;
}

// Equality comparison
template <typename T>
bool BinarySearchTreeIterator<T>::operator==(const Iterator &aOtherIter) const
{
	// First check if both iterators reference the same tree
	if (&fBSTree != &aOtherIter.fBSTree)
	{
		return false; // Different trees, cannot be equal
	}

	// Check stack sizes
	if (fStack.size() != aOtherIter.fStack.size())
	{
		return false;
	}

	// If both stacks are empty, both are at end
	if (fStack.empty())
	{
		return true; // Both are end iterators
	}

	// Compare actual node pointers (not just keys)
	return fStack.top() == aOtherIter.fStack.top();
}

// Inequality comparison
template <typename T>
bool BinarySearchTreeIterator<T>::operator!=(const Iterator &aOtherIter) const
{
	return !(*this == aOtherIter); // FIX 6: Should be *this not this
}

// Return iterator at begin position
template <typename T>
typename BinarySearchTreeIterator<T>::Iterator BinarySearchTreeIterator<T>::begin() const
{
	return Iterator(fBSTree);
}

// Return iterator at end position
template <typename T>
typename BinarySearchTreeIterator<T>::Iterator BinarySearchTreeIterator<T>::end() const
{
	Iterator iter(fBSTree);
	// Clear the stack to create end iterator
	iter.fStack = BTNStack();
	return iter;
}