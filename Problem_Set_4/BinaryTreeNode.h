// COS30008, Problem Set 4, Problem 1, 2022

#pragma once

#include <stdexcept>
#include <algorithm>

template <typename T>
struct BinaryTreeNode
{
	using BNode = BinaryTreeNode<T>;
	using BTreeNode = BNode *;

	T key;
	BTreeNode left;
	BTreeNode right;

	static BNode NIL;

	const T &findMax() const
	{
		if (empty())
		{
			throw std::domain_error("Empty tree encountered.");
		}

		return right->empty() ? key : right->findMax();
	}

	const T &findMin() const
	{
		if (empty())
		{
			throw std::domain_error("Empty tree encountered.");
		}

		return left->empty() ? key : left->findMin();
	}

	bool remove(const T &aKey, BTreeNode aParent)
	{
		BTreeNode x = this;
		BTreeNode y = aParent;

		while (!x->empty())
		{
			if (aKey == x->key)
			{
				break;
			}

			y = x; // new parent

			x = aKey < x->key ? x->left : x->right;
		}

		if (x->empty())
		{
			return false; // delete failed
		}

		if (!x->left->empty())
		{
			const T &lKey = x->left->findMax(); // find max to left
			x->key = lKey;
			x->left->remove(lKey, x);
		}
		else
		{
			if (!x->right->empty())
			{
				const T &lKey = x->right->findMin(); // find min to right
				x->key = lKey;
				x->right->remove(lKey, x);
			}
			else
			{
				if (y != &NIL) // y can be NIL
				{
					if (y->left == x)
					{
						y->left = &NIL;
					}
					else
					{
						y->right = &NIL;
					}
				}

				delete x; // free deleted node
			}
		}

		return true;
	}

	// PS4 starts here

	BinaryTreeNode();
	BinaryTreeNode(const T &aKey);
	BinaryTreeNode(T &&aKey);

	~BinaryTreeNode();

	bool empty() const;
	bool leaf() const;
	size_t height() const;

	bool insert(const T &aKey);
};

// Default constructor
template <typename T>
BinaryTreeNode<T>::BinaryTreeNode() : key(), left(&NIL), right(&NIL)
{
}

// Copy constructor
template <typename T>
BinaryTreeNode<T>::BinaryTreeNode(const T &aKey) : key(aKey), left(&NIL), right(&NIL)
{
}

// Move constructor
template <typename T>
BinaryTreeNode<T>::BinaryTreeNode(T &&aKey) : key(std::move(aKey)), left(&NIL), right(&NIL)
{
}

// Destructor
template <typename T>
BinaryTreeNode<T>::~BinaryTreeNode()
{
	if (!left->empty())
		delete left;
	if (!right->empty())
		delete right;
}

// Check if this node is the NIL node
template <typename T>
bool BinaryTreeNode<T>::empty() const
{
	return this == &NIL;
}

// Check if the node is a leaf - CORRECTED
template <typename T>
bool BinaryTreeNode<T>::leaf() const
{
	return left->empty() && right->empty();
}

// Compute height of the tree
template <typename T>
size_t BinaryTreeNode<T>::height() const
{
	if (empty())
	{
		throw std::domain_error("Empty tree encountered");
	}

	size_t l = left->empty() ? 0 : left->height() + 1;
	size_t r = right->empty() ? 0 : right->height() + 1;

	return std::max(l, r);
}

// Insert key into the tree
template <typename T>
bool BinaryTreeNode<T>::insert(const T &aKey)
{
	if (empty())
	{
		return false;
	}

	if (aKey < key)
	{
		return left->empty() ? (left = new BNode(aKey), true) : left->insert(aKey);
	}
	else if (key < aKey)
	{
		return right->empty() ? (right = new BNode(aKey), true) : right->insert(aKey);
	}

	return false; // duplicate
}

// Static NIL definition
template <typename T>
BinaryTreeNode<T> BinaryTreeNode<T>::NIL;