#include <iostream>
#include <stack>
#include <vector>
#include <string>
#include <stdexcept>
#include <chrono>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <thread>

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable : 4996)
#endif

// Forward declaration for Problem 4
template <typename T>
class BinarySearchTreeReverseIterator;

// =============================================================================
// MAIN BST CLASS - Contains all problems integrated
// =============================================================================

template <typename T>
class BinarySearchTree
{
private:
	T fKey;
	BinarySearchTree* fLeft;
	BinarySearchTree* fRight;

	// PROBLEM 1: Augmentation for rank queries
	size_t fSubtreeSize;

	// PROBLEM 2: Metadata for copy control
	std::string fCreationTimestamp;

	// PROBLEM 3: Static logging for move operations
	static std::vector<std::string> moveLog;

	// =============================================================================
	// HELPER METHODS
	// =============================================================================

	void updateSubtreeSize()
	{
		if (!empty())
		{
			fSubtreeSize = 1 +
				(fLeft && !fLeft->empty() ? fLeft->fSubtreeSize : 0) +
				(fRight && !fRight->empty() ? fRight->fSubtreeSize : 0);
		}
	}

	std::string getCurrentTimestamp() const
	{
		auto now = std::chrono::system_clock::now();
		auto time_t = std::chrono::system_clock::to_time_t(now);
		std::stringstream ss;

#ifdef _MSC_VER
		char buffer[100];
		ctime_s(buffer, sizeof(buffer), &time_t);
		ss << buffer;
#else
		ss << std::ctime(&time_t);
#endif

		std::string result = ss.str();
		if (!result.empty() && result.back() == '\n')
		{
			result.pop_back();
		}
		return result;
	}

	std::string toString(const T& value) const
	{
		std::stringstream ss;
		ss << value;
		return ss.str();
	}

	BinarySearchTree* findMin()
	{
		BinarySearchTree* current = this;
		while (current->fLeft && !current->fLeft->empty())
		{
			current = current->fLeft;
		}
		return current;
	}

public:
	using Iterator = BinarySearchTreeReverseIterator<T>;

	// =============================================================================
	// PROBLEM 1: AUGMENTED BST INFRASTRUCTURE (120 marks)
	// =============================================================================

	// Basic constructor with augmentation
	BinarySearchTree(const T& aKey) : fKey(aKey), fLeft(nullptr), fRight(nullptr),
		fSubtreeSize(1), fCreationTimestamp(getCurrentTimestamp()) {
	}

	// NIL constructor
	BinarySearchTree() : fLeft(nullptr), fRight(nullptr), fSubtreeSize(0),
		fCreationTimestamp(getCurrentTimestamp()) {
	}

	// Destructor
	~BinarySearchTree()
	{
		delete fLeft;
		delete fRight;
	}

	// Basic operations
	bool empty() const { return fSubtreeSize == 0; }

	bool leaf() const
	{
		return !empty() && (!fLeft || fLeft->empty()) && (!fRight || fRight->empty());
	}

	size_t size() const { return fSubtreeSize; }

	size_t depth() const
	{
		if (empty())
			return 0;
		size_t leftDepth = (fLeft && !fLeft->empty()) ? fLeft->depth() : 0;
		size_t rightDepth = (fRight && !fRight->empty()) ? fRight->depth() : 0;
		return 1 + std::max(leftDepth, rightDepth);
	}

	const T& operator*() const
	{
		if (empty())
			throw std::domain_error("Cannot dereference NIL tree");
		return fKey;
	}

	// Insert with subtree size maintenance
	bool insert(const T& aKey)
	{
		if (empty())
		{
			*this = BinarySearchTree(aKey);
			return true;
		}

		bool inserted = false;
		if (aKey < fKey)
		{
			if (!fLeft)
			{
				fLeft = new BinarySearchTree(aKey);
				inserted = true;
			}
			else
			{
				inserted = fLeft->insert(aKey);
			}
		}
		else if (aKey > fKey)
		{
			if (!fRight)
			{
				fRight = new BinarySearchTree(aKey);
				inserted = true;
			}
			else
			{
				inserted = fRight->insert(aKey);
			}
		}

		if (inserted)
		{
			updateSubtreeSize();
		}
		return inserted;
	}

	// Remove with subtree size maintenance
	bool remove(const T& aKey)
	{
		if (empty())
			return false;

		bool removed = false;

		if (aKey < fKey)
		{
			if (fLeft)
			{
				removed = fLeft->remove(aKey);
				if (fLeft->empty())
				{
					delete fLeft;
					fLeft = nullptr;
				}
			}
		}
		else if (aKey > fKey)
		{
			if (fRight)
			{
				removed = fRight->remove(aKey);
				if (fRight->empty())
				{
					delete fRight;
					fRight = nullptr;
				}
			}
		}
		else
		{
			removed = true;

			if (leaf())
			{
				fSubtreeSize = 0;
			}
			else if (!fLeft || fLeft->empty())
			{
				BinarySearchTree* temp = fRight;
				fKey = temp->fKey;
				fLeft = temp->fLeft;
				fRight = temp->fRight;
				fSubtreeSize = temp->fSubtreeSize;
				fCreationTimestamp = temp->fCreationTimestamp;

				temp->fLeft = nullptr;
				temp->fRight = nullptr;
				delete temp;
			}
			else if (!fRight || fRight->empty())
			{
				BinarySearchTree* temp = fLeft;
				fKey = temp->fKey;
				fLeft = temp->fLeft;
				fRight = temp->fRight;
				fSubtreeSize = temp->fSubtreeSize;
				fCreationTimestamp = temp->fCreationTimestamp;

				temp->fLeft = nullptr;
				temp->fRight = nullptr;
				delete temp;
			}
			else
			{
				BinarySearchTree* successor = fRight->findMin();
				fKey = successor->fKey;
				fRight->remove(successor->fKey);
				if (fRight->empty())
				{
					delete fRight;
					fRight = nullptr;
				}
			}
		}

		if (removed && !empty())
		{
			updateSubtreeSize();
		}

		return removed;
	}

	// Find operation
	const T* find(const T& aKey) const
	{
		if (empty())
			return nullptr;

		if (aKey == fKey)
		{
			return &fKey;
		}
		else if (aKey < fKey)
		{
			return fLeft ? fLeft->find(aKey) : nullptr;
		}
		else
		{
			return fRight ? fRight->find(aKey) : nullptr;
		}
	}

	// NEW: Rank query - O(log n) due to augmentation
	size_t rank(const T& aKey) const
	{
		if (empty())
			return 0;

		if (aKey < fKey)
		{
			return fLeft ? fLeft->rank(aKey) : 0;
		}
		else if (aKey > fKey)
		{
			size_t leftSize = (fLeft && !fLeft->empty()) ? fLeft->fSubtreeSize : 0;
			size_t rightRank = fRight ? fRight->rank(aKey) : 0;
			return leftSize + 1 + rightRank;
		}
		else
		{
			return (fLeft && !fLeft->empty()) ? fLeft->fSubtreeSize : 0;
		}
	}

	// =============================================================================
	// PROBLEM 2: COPY CONTROL WITH METADATA (64 marks)
	// =============================================================================

	// Copy constructor
	BinarySearchTree(const BinarySearchTree& aOtherBST)
	{
		if (aOtherBST.empty())
		{
			throw std::domain_error("Cannot copy NIL tree");
		}

		fKey = aOtherBST.fKey;
		fSubtreeSize = aOtherBST.fSubtreeSize;
		fCreationTimestamp = aOtherBST.fCreationTimestamp; // Copy timestamp

		fLeft = (aOtherBST.fLeft && !aOtherBST.fLeft->empty()) ? new BinarySearchTree(*aOtherBST.fLeft) : nullptr;
		fRight = (aOtherBST.fRight && !aOtherBST.fRight->empty()) ? new BinarySearchTree(*aOtherBST.fRight) : nullptr;
	}

	// Assignment operator
	BinarySearchTree& operator=(const BinarySearchTree& aOtherBST)
	{
		if (this != &aOtherBST)
		{
			if (aOtherBST.empty())
			{
				throw std::domain_error("Cannot assign NIL tree");
			}

			delete fLeft;
			delete fRight;

			fKey = aOtherBST.fKey;
			fSubtreeSize = aOtherBST.fSubtreeSize;
			fCreationTimestamp = aOtherBST.fCreationTimestamp; // Copy timestamp

			fLeft = (aOtherBST.fLeft && !aOtherBST.fLeft->empty()) ? new BinarySearchTree(*aOtherBST.fLeft) : nullptr;
			fRight = (aOtherBST.fRight && !aOtherBST.fRight->empty()) ? new BinarySearchTree(*aOtherBST.fRight) : nullptr;
		}
		return *this;
	}

	// Clone with new timestamp
	BinarySearchTree* clone() const
	{
		if (empty())
		{
			throw std::domain_error("Cannot clone NIL tree");
		}

		BinarySearchTree* cloned = new BinarySearchTree(*this);
		cloned->fCreationTimestamp = getCurrentTimestamp(); // New timestamp
		return cloned;
	}

	// =============================================================================
	// PROBLEM 3: MOVE SEMANTICS WITH LOGGING (72 marks)
	// =============================================================================

	// Move constructor from key
	BinarySearchTree(T&& aKey) : fKey(std::move(aKey)), fLeft(nullptr), fRight(nullptr),
		fSubtreeSize(1), fCreationTimestamp(getCurrentTimestamp())
	{
		moveLog.push_back("Moved node with key: " + toString(fKey));
	}

	// Move constructor
	BinarySearchTree(BinarySearchTree&& aOtherBST) noexcept
	{
		if (aOtherBST.empty())
		{
			fLeft = nullptr;
			fRight = nullptr;
			fSubtreeSize = 0;
			fCreationTimestamp = getCurrentTimestamp();
			moveLog.push_back("Moved empty BST");
		}
		else
		{
			fKey = std::move(aOtherBST.fKey);
			fLeft = aOtherBST.fLeft;
			fRight = aOtherBST.fRight;
			fSubtreeSize = aOtherBST.fSubtreeSize;
			fCreationTimestamp = std::move(aOtherBST.fCreationTimestamp);

			// Make source NIL
			aOtherBST.fLeft = nullptr;
			aOtherBST.fRight = nullptr;
			aOtherBST.fSubtreeSize = 0;

			moveLog.push_back("Moved BST with root key: " + toString(fKey));
		}
	}

	// Move assignment
	BinarySearchTree& operator=(BinarySearchTree&& aOtherBST) noexcept
	{
		if (this != &aOtherBST)
		{
			delete fLeft;
			delete fRight;

			if (aOtherBST.empty())
			{
				fSubtreeSize = 0;
				fLeft = nullptr;
				fRight = nullptr;
				fCreationTimestamp = getCurrentTimestamp();
				moveLog.push_back("Move assigned empty BST");
			}
			else
			{
				fKey = std::move(aOtherBST.fKey);
				fLeft = aOtherBST.fLeft;
				fRight = aOtherBST.fRight;
				fSubtreeSize = aOtherBST.fSubtreeSize;
				fCreationTimestamp = std::move(aOtherBST.fCreationTimestamp);

				// Make source NIL
				aOtherBST.fLeft = nullptr;
				aOtherBST.fRight = nullptr;
				aOtherBST.fSubtreeSize = 0;

				moveLog.push_back("Move assigned BST with root key: " + toString(fKey));
			}
		}
		return *this;
	}

	// =============================================================================
	// PROBLEM 4: REVERSE IN-ORDER ITERATOR SUPPORT (90 marks)
	// =============================================================================

	BinarySearchTreeReverseIterator<T> rbegin() const
	{
		return BinarySearchTreeReverseIterator<T>(this);
	}

	BinarySearchTreeReverseIterator<T> rend() const
	{
		return BinarySearchTreeReverseIterator<T>(nullptr);
	}

	// Utility methods for iterator
	const BinarySearchTree* getLeft() const { return fLeft; }
	const BinarySearchTree* getRight() const { return fRight; }

	// =============================================================================
	// UTILITY METHODS
	// =============================================================================

	// Access move log
	static const std::vector<std::string>& getMoveLog() { return moveLog; }
	static void clearMoveLog() { moveLog.clear(); }

	// Get timestamp
	const std::string& getCreationTimestamp() const { return fCreationTimestamp; }
};

// Static member definition
template <typename T>
std::vector<std::string> BinarySearchTree<T>::moveLog;

// =============================================================================
// PROBLEM 4: REVERSE IN-ORDER ITERATOR IMPLEMENTATION (90 marks)
// =============================================================================

template <typename T>
class BinarySearchTreeReverseIterator
{
private:
	using BST = BinarySearchTree<T>;
	using BSTNode = const BST*;
	using NodeStack = std::stack<BSTNode>;

	const BST* fBST;
	NodeStack fStack;

	void pushRightPath(BSTNode aNode)
	{
		while (aNode && !aNode->empty())
		{
			fStack.push(aNode);
			aNode = aNode->getRight(); // Go right first for reverse order
		}
	}

public:
	BinarySearchTreeReverseIterator(BSTNode aBST) : fBST(aBST)
	{
		if (aBST && !aBST->empty())
		{
			pushRightPath(aBST);
		}
	}

	const T& operator*() const
	{
		if (fStack.empty())
		{
			throw std::out_of_range("Iterator out of range");
		}
		return **fStack.top();
	}

	BinarySearchTreeReverseIterator& operator++()
	{
		if (fStack.empty())
		{
			throw std::out_of_range("Iterator out of range");
		}

		BSTNode current = fStack.top();
		fStack.pop();

		// Move to left subtree and push its rightmost path
		if (current->getLeft() && !current->getLeft()->empty())
		{
			pushRightPath(current->getLeft());
		}

		return *this;
	}

	BinarySearchTreeReverseIterator operator++(int)
	{
		BinarySearchTreeReverseIterator temp = *this;
		++(*this);
		return temp;
	}

	bool operator==(const BinarySearchTreeReverseIterator& aOther) const
	{
		if (fStack.empty() && aOther.fStack.empty())
		{
			return true;
		}
		if (fStack.empty() || aOther.fStack.empty())
		{
			return false;
		}
		return fStack.top() == aOther.fStack.top();
	}

	bool operator!=(const BinarySearchTreeReverseIterator& aOther) const
	{
		return !(*this == aOther);
	}
};

// =============================================================================
// TEST DRIVERS FOR ALL PROBLEMS
// =============================================================================

void testProblem1_AugmentedBST()
{
	std::cout << "=== PROBLEM 1: AUGMENTED BST INFRASTRUCTURE (120 marks) ===" << std::endl;

	BinarySearchTree<int> bst(50);
	bst.insert(30);
	bst.insert(70);
	bst.insert(20);
	bst.insert(40);
	bst.insert(60);
	bst.insert(80);

	std::cout << "Tree structure: 50(30(20,40),70(60,80))" << std::endl;
	std::cout << "Tree size: " << bst.size() << std::endl;
	std::cout << "Tree depth: " << bst.depth() << std::endl;

	// Test rank queries
	std::cout << "\n--- Rank Queries (NEW FEATURE) ---" << std::endl;
	std::cout << "Values in order: 20, 30, 40, 50, 60, 70, 80" << std::endl;
	std::cout << "Rank of 15: " << bst.rank(15) << " (expected: 0)" << std::endl;
	std::cout << "Rank of 30: " << bst.rank(30) << " (expected: 1)" << std::endl;
	std::cout << "Rank of 45: " << bst.rank(45) << " (expected: 3)" << std::endl;
	std::cout << "Rank of 75: " << bst.rank(75) << " (expected: 6)" << std::endl;

	// Test removal with size updates
	std::cout << "\n--- Testing Removal ---" << std::endl;
	bst.remove(30);
	std::cout << "After removing 30, size: " << bst.size() << std::endl;
	std::cout << "New rank of 45: " << bst.rank(45) << " (expected: 2)" << std::endl;

	std::cout << "✓ Problem 1 tests completed\n"
		<< std::endl;
}

void testProblem2_CopyControl()
{
	std::cout << "=== PROBLEM 2: COPY CONTROL WITH METADATA (64 marks) ===" << std::endl;

	BinarySearchTree<int> original(42);
	original.insert(25);
	original.insert(75);

	std::cout << "Original timestamp: " << original.getCreationTimestamp() << std::endl;

	// Small delay for different timestamps
	std::this_thread::sleep_for(std::chrono::milliseconds(100));

	// Test copy constructor
	std::cout << "\n--- Copy Constructor ---" << std::endl;
	BinarySearchTree<int> copied(original);
	std::cout << "Copied timestamp: " << copied.getCreationTimestamp() << std::endl;
	std::cout << "Timestamps match: " << (original.getCreationTimestamp() == copied.getCreationTimestamp() ? "Yes" : "No") << std::endl;

	// Test clone (should have new timestamp)
	std::cout << "\n--- Clone Method ---" << std::endl;
	auto* cloned = original.clone();
	std::cout << "Cloned timestamp: " << cloned->getCreationTimestamp() << std::endl;
	std::cout << "Timestamps different: " << (original.getCreationTimestamp() != cloned->getCreationTimestamp() ? "Yes" : "No") << std::endl;

	// Test error conditions
	std::cout << "\n--- Error Handling ---" << std::endl;
	try
	{
		BinarySearchTree<int> nilTree;
		BinarySearchTree<int> shouldFail(nilTree);
		std::cout << "ERROR: Should have thrown exception!" << std::endl;
	}
	catch (const std::domain_error& e)
	{
		std::cout << "✓ Copy NIL tree correctly threw: " << e.what() << std::endl;
	}

	delete cloned;
	std::cout << "✓ Problem 2 tests completed\n"
		<< std::endl;
}

void testProblem3_MoveSemantics()
{
	std::cout << "=== PROBLEM 3: MOVE SEMANTICS WITH LOGGING (72 marks) ===" << std::endl;

	BinarySearchTree<int>::clearMoveLog();

	// Test move constructor from key
	std::cout << "\n--- Move Constructor from Key ---" << std::endl;
	BinarySearchTree<int> bst1(std::move(42));

	// Test move constructor
	std::cout << "--- Move Constructor ---" << std::endl;
	BinarySearchTree<int> bst2(std::move(bst1));

	// Test move assignment
	std::cout << "--- Move Assignment ---" << std::endl;
	BinarySearchTree<int> bst3(100);
	bst3 = std::move(bst2);

	std::cout << "\n--- Move Operations Log ---" << std::endl;
	const auto& log = BinarySearchTree<int>::getMoveLog();
	for (size_t i = 0; i < log.size(); ++i)
	{
		std::cout << i + 1 << ". " << log[i] << std::endl;
	}

	std::cout << "✓ Problem 3 tests completed\n"
		<< std::endl;
}

void testProblem4_ReverseIterator()
{
	std::cout << "=== PROBLEM 4: REVERSE IN-ORDER ITERATOR (90 marks) ===" << std::endl;

	BinarySearchTree<int> bst(50);
	bst.insert(30);
	bst.insert(70);
	bst.insert(20);
	bst.insert(40);
	bst.insert(60);
	bst.insert(80);

	std::cout << "Tree values in normal order: 20, 30, 40, 50, 60, 70, 80" << std::endl;
	std::cout << "Reverse in-order traversal: ";

	for (auto it = bst.rbegin(); it != bst.rend(); ++it)
	{
		std::cout << *it << " ";
	}
	std::cout << std::endl;
	std::cout << "Expected: 80 70 60 50 40 30 20" << std::endl;

	// Test iterator operations
	std::cout << "\n--- Iterator Operations ---" << std::endl;
	auto it = bst.rbegin();
	std::cout << "First element: " << *it << std::endl;
	++it;
	std::cout << "After ++: " << *it << std::endl;
	auto it2 = it++;
	std::cout << "After post++: " << *it << ", saved: " << *it2 << std::endl;

	std::cout << "✓ Problem 4 tests completed\n"
		<< std::endl;
}

// =============================================================================
// MAIN FUNCTION - RUNS ALL TEST DRIVERS
// =============================================================================

int main()
{
	try
	{
		std::cout << "🧠 COS30008 – Practice 1 - Complete Solution" << std::endl;
		std::cout << "Time: 2 hours | Total Marks: 396" << std::endl;
		std::cout << "================================================\n"
			<< std::endl;

		testProblem1_AugmentedBST();
		testProblem2_CopyControl();
		testProblem3_MoveSemantics();
		testProblem4_ReverseIterator();
		std::cout << "🎉 ALL PROBLEMS COMPLETED SUCCESSFULLY! 🎉" << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cerr << "❌ Error: " << e.what() << std::endl;
		return 1;
	}

	return 0;
}