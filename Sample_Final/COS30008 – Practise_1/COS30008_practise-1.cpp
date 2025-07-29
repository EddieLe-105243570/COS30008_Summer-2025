#include <iostream>
#include <stack>
#include <vector>
#include <string>
#include <stdexcept>
#include <chrono>
#include <ctime>
#include <sstream>

// Forward declaration for iterator
template <typename T>
class BinarySearchTreeReverseIterator;

template <typename T>
class BinarySearchTree
{
private:
    T fKey;
    BinarySearchTree *fLeft;
    BinarySearchTree *fRight;
    size_t fSubtreeSize;                     // Problem 1: Augmentation
    std::string fCreationTimestamp;          // Problem 2: Metadata
    static std::vector<std::string> moveLog; // Problem 3: Move logging

    // Helper methods
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
        ss << std::ctime(&time_t);
        std::string result = ss.str();
        if (!result.empty() && result.back() == '\n')
        {
            result.pop_back();
        }
        return result;
    }

    BinarySearchTree *findMin()
    {
        BinarySearchTree *current = this;
        while (current->fLeft && !current->fLeft->empty())
        {
            current = current->fLeft;
        }
        return current;
    }

public:
    using Iterator = BinarySearchTreeReverseIterator<T>;

    // Problem 1: Basic constructor with augmentation
    BinarySearchTree(const T &aKey) : fKey(aKey), fLeft(nullptr), fRight(nullptr),
                                      fSubtreeSize(1), fCreationTimestamp(getCurrentTimestamp()) {}

    // NIL constructor
    BinarySearchTree() : fLeft(nullptr), fRight(nullptr), fSubtreeSize(0) {}

    // Problem 2: Copy constructor
    BinarySearchTree(const BinarySearchTree &aOtherBST)
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

    // Problem 3: Move constructor from key
    BinarySearchTree(T &&aKey) : fKey(std::move(aKey)), fLeft(nullptr), fRight(nullptr),
                                 fSubtreeSize(1), fCreationTimestamp(getCurrentTimestamp())
    {
        moveLog.push_back("Moved node with key: " + std::to_string(fKey));
    }

    // Problem 3: Move constructor
    BinarySearchTree(BinarySearchTree &&aOtherBST) noexcept
    {
        if (aOtherBST.empty())
        {
            fLeft = nullptr;
            fRight = nullptr;
            fSubtreeSize = 0;
            fCreationTimestamp = getCurrentTimestamp();
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

            moveLog.push_back("Moved BST with root key: " + std::to_string(fKey));
        }
    }

    // Destructor
    ~BinarySearchTree()
    {
        delete fLeft;
        delete fRight;
    }

    // Problem 2: Assignment operator
    BinarySearchTree &operator=(const BinarySearchTree &aOtherBST)
    {
        if (this != &aOtherBST)
        {
            if (aOtherBST.empty())
            {
                throw std::domain_error("Cannot assign NIL tree");
            }

            // Clean up existing resources
            delete fLeft;
            delete fRight;

            // Copy data
            fKey = aOtherBST.fKey;
            fSubtreeSize = aOtherBST.fSubtreeSize;
            fCreationTimestamp = aOtherBST.fCreationTimestamp;

            fLeft = (aOtherBST.fLeft && !aOtherBST.fLeft->empty()) ? new BinarySearchTree(*aOtherBST.fLeft) : nullptr;
            fRight = (aOtherBST.fRight && !aOtherBST.fRight->empty()) ? new BinarySearchTree(*aOtherBST.fRight) : nullptr;
        }
        return *this;
    }

    // Problem 3: Move assignment
    BinarySearchTree &operator=(BinarySearchTree &&aOtherBST) noexcept
    {
        if (this != &aOtherBST)
        {
            // Clean up existing resources
            delete fLeft;
            delete fRight;

            if (aOtherBST.empty())
            {
                fSubtreeSize = 0;
                fLeft = nullptr;
                fRight = nullptr;
                fCreationTimestamp = getCurrentTimestamp();
            }
            else
            {
                // Move data
                fKey = std::move(aOtherBST.fKey);
                fLeft = aOtherBST.fLeft;
                fRight = aOtherBST.fRight;
                fSubtreeSize = aOtherBST.fSubtreeSize;
                fCreationTimestamp = std::move(aOtherBST.fCreationTimestamp);

                // Make source NIL
                aOtherBST.fLeft = nullptr;
                aOtherBST.fRight = nullptr;
                aOtherBST.fSubtreeSize = 0;

                moveLog.push_back("Move assigned BST with root key: " + std::to_string(fKey));
            }
        }
        return *this;
    }

    // Problem 2: Clone method
    BinarySearchTree *clone() const
    {
        if (empty())
        {
            throw std::domain_error("Cannot clone NIL tree");
        }

        BinarySearchTree *cloned = new BinarySearchTree(*this);
        cloned->fCreationTimestamp = getCurrentTimestamp(); // New timestamp
        return cloned;
    }

    // Basic operations
    bool empty() const
    {
        return fSubtreeSize == 0;
    }

    bool leaf() const
    {
        return !empty() && (!fLeft || fLeft->empty()) && (!fRight || fRight->empty());
    }

    size_t size() const
    {
        return fSubtreeSize;
    }

    size_t depth() const
    {
        if (empty())
            return 0;

        size_t leftDepth = (fLeft && !fLeft->empty()) ? fLeft->depth() : 0;
        size_t rightDepth = (fRight && !fRight->empty()) ? fRight->depth() : 0;

        return 1 + std::max(leftDepth, rightDepth);
    }

    const T &operator*() const
    {
        if (empty())
        {
            throw std::domain_error("Cannot dereference NIL tree");
        }
        return fKey;
    }

    // Problem 1: Insert with subtree size maintenance
    bool insert(const T &aKey)
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

    // Problem 1: Remove with subtree size maintenance
    bool remove(const T &aKey)
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
            // Node to be removed found
            removed = true;

            if (leaf())
            {
                // Leaf node - make it NIL
                fSubtreeSize = 0;
            }
            else if (!fLeft || fLeft->empty())
            {
                // Only right child
                BinarySearchTree *temp = fRight;
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
                // Only left child
                BinarySearchTree *temp = fLeft;
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
                // Two children - find inorder successor
                BinarySearchTree *successor = fRight->findMin();
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

    // Find method
    const T *find(const T &aKey) const
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

    // Problem 1: Rank query
    size_t rank(const T &aKey) const
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

    // Problem 4: Iterator support
    BinarySearchTreeReverseIterator<T> rbegin() const
    {
        return BinarySearchTreeReverseIterator<T>(this);
    }

    BinarySearchTreeReverseIterator<T> rend() const
    {
        return BinarySearchTreeReverseIterator<T>(nullptr);
    }

    // Utility methods for iterator
    const BinarySearchTree *getLeft() const { return fLeft; }
    const BinarySearchTree *getRight() const { return fRight; }

    // Problem 3: Access move log
    static const std::vector<std::string> &getMoveLog()
    {
        return moveLog;
    }

    static void clearMoveLog()
    {
        moveLog.clear();
    }

    // Utility method to get timestamp
    const std::string &getCreationTimestamp() const
    {
        return fCreationTimestamp;
    }
};

// Static member definition
template <typename T>
std::vector<std::string> BinarySearchTree<T>::moveLog;

// Problem 4: Reverse In-order Iterator Implementation
template <typename T>
class BinarySearchTreeReverseIterator
{
private:
    using BST = BinarySearchTree<T>;
    using BSTNode = const BST *;
    using NodeStack = std::stack<BSTNode>;

    const BST *fBST;
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
        if (aBST)
        {
            pushRightPath(aBST);
        }
    }

    const T &operator*() const
    {
        if (fStack.empty())
        {
            throw std::out_of_range("Iterator out of range");
        }
        return **fStack.top();
    }

    BinarySearchTreeReverseIterator &operator++()
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

    bool operator==(const BinarySearchTreeReverseIterator &aOther) const
    {
        // Two iterators are equal if they have the same stack state
        if (fStack.size() != aOther.fStack.size())
        {
            return false;
        }

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

    bool operator!=(const BinarySearchTreeReverseIterator &aOther) const
    {
        return !(*this == aOther);
    }
};

// Test driver functions
void testAugmentedBST()
{
    std::cout << "=== Testing Augmented BST (Problem 1) ===" << std::endl;

    BinarySearchTree<int> bst(50);
    bst.insert(30);
    bst.insert(70);
    bst.insert(20);
    bst.insert(40);
    bst.insert(60);
    bst.insert(80);

    std::cout << "Tree size: " << bst.size() << std::endl;
    std::cout << "Rank of 45: " << bst.rank(45) << std::endl; // Should be 4
    std::cout << "Rank of 30: " << bst.rank(30) << std::endl; // Should be 1
    std::cout << "Rank of 75: " << bst.rank(75) << std::endl; // Should be 6

    std::cout << "Removing 30..." << std::endl;
    bst.remove(30);
    std::cout << "New size: " << bst.size() << std::endl;
    std::cout << "New rank of 45: " << bst.rank(45) << std::endl; // Should be 3
}

void testCopyControl()
{
    std::cout << "\n=== Testing Copy Control (Problem 2) ===" << std::endl;

    BinarySearchTree<int> original(42);
    original.insert(25);
    original.insert(75);

    std::cout << "Original timestamp: " << original.getCreationTimestamp() << std::endl;

    // Test copy constructor
    BinarySearchTree<int> copied(original);
    std::cout << "Copied timestamp: " << copied.getCreationTimestamp() << std::endl;

    // Test clone (should have new timestamp)
    auto *cloned = original.clone();
    std::cout << "Cloned timestamp: " << cloned->getCreationTimestamp() << std::endl;

    delete cloned;
}

void testMoveSemantics()
{
    std::cout << "\n=== Testing Move Semantics (Problem 3) ===" << std::endl;

    BinarySearchTree<int>::clearMoveLog();

    // Test move constructor from key
    BinarySearchTree<int> bst1(std::move(42));

    // Test move constructor
    BinarySearchTree<int> bst2(std::move(bst1));

    // Test move assignment
    BinarySearchTree<int> bst3(100);
    bst3 = std::move(bst2);

    std::cout << "Move operations log:" << std::endl;
    for (const auto &entry : BinarySearchTree<int>::getMoveLog())
    {
        std::cout << "  " << entry << std::endl;
    }
}

void testReverseIterator()
{
    std::cout << "\n=== Testing Reverse Iterator (Problem 4) ===" << std::endl;

    BinarySearchTree<int> bst(50);
    bst.insert(30);
    bst.insert(70);
    bst.insert(20);
    bst.insert(40);
    bst.insert(60);
    bst.insert(80);

    std::cout << "Reverse in-order traversal: ";
    for (auto it = bst.rbegin(); it != bst.rend(); ++it)
    {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
}

int main()
{
    try
    {
        testAugmentedBST();
        testCopyControl();
        testMoveSemantics();
        testReverseIterator();

        std::cout << "\n=== All tests completed successfully! ===" << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}