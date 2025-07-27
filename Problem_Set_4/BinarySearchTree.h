// COS30008, Problem Set 4, Problem 2, 2022

#pragma once

#include "BinaryTreeNode.h"

#include <stdexcept>

// Problem 3 requirement
template <typename T>
class BinarySearchTreeIterator;

template <typename T>
class BinarySearchTree
{
private:
	using BNode = BinaryTreeNode<T>;
	using BTreeNode = BNode *;

	BTreeNode fRoot;

public:
	BinarySearchTree();

	~BinarySearchTree();

	bool empty() const;
	size_t height() const;

	bool insert(const T &aKey);
	bool remove(const T &aKey);

	// Problem 3 methods

	using Iterator = BinarySearchTreeIterator<T>;

	// Allow iterator to access private member variables
	friend class BinarySearchTreeIterator<T>;

	Iterator begin() const;
	Iterator end() const;
};

// IMPLEMENTATIONS - Add these at the end of your BinarySearchTree.h file

// Constructor
template <typename T>
BinarySearchTree<T>::BinarySearchTree() : fRoot(&BNode::NIL)
{
}

// Destructor
template <typename T>
BinarySearchTree<T>::~BinarySearchTree()
{
	if (!fRoot->empty())
	{
		delete fRoot;
	}
}

// Check if tree is empty
template <typename T>
bool BinarySearchTree<T>::empty() const
{
	return fRoot->empty();
}

// Get height of tree
template <typename T>
size_t BinarySearchTree<T>::height() const
{
	if (empty())
	{
		throw std::domain_error("Empty tree has no height.");
	}
	return fRoot->height();
}

// Insert key into tree
template <typename T>
bool BinarySearchTree<T>::insert(const T &aKey)
{
	if (empty())
	{
		fRoot = new BNode(aKey);
		return true;
	}
	return fRoot->insert(aKey);
}

// Remove key from tree
template <typename T>
bool BinarySearchTree<T>::remove(const T &aKey)
{
	if (empty())
	{
		return false;
	}

	// Special case: removing root when root will become NIL
	if (fRoot->key == aKey && fRoot->left->empty() && fRoot->right->empty())
	{
		delete fRoot;
		fRoot = &BNode::NIL;
		return true;
	}

	return fRoot->remove(aKey, &BNode::NIL);
}

// Begin iterator - will be implemented after Problem 3
template <typename T>
typename BinarySearchTree<T>::Iterator BinarySearchTree<T>::begin() const
{
	return Iterator(*this);
}

// End iterator - will be implemented after Problem 3
template <typename T>
typename BinarySearchTree<T>::Iterator BinarySearchTree<T>::end() const
{
	Iterator iter(*this);
	return iter.end();
}