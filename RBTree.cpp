//#include "RBTree.h"

#include<queue>
#include<iostream>
#include<fstream>
#include<sstream>
#include<vector>
#include<math.h>
#include<climits>
#include<string>
#include <iterator>

 /*int MakeNumber(std::string digits)
{
    int number = 0;
    
    for(int i = 0; i < digits.size(); i++)
    	number  = number*10 + (digits.at(i)-'0');
    
    return number;
}*/

 std::vector<std::string> Tokenize(std::string s, char delim) 
 {
	std::vector<std::string> tokens;
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) 
        tokens.push_back(item);
    return tokens;
}


 //enum for storing color of the node
enum NodeColor
{
	RED = 0,
	BLACK
};

struct TreeNode
{
	//only for null node
	TreeNode()
	{
		left = right = parent = 0;
		val = -1;
		color = BLACK;
		count = 0;
	}

	TreeNode(int value)
	{
		left = right = parent = 0;
		val = value;
		color = RED;
		count = 1;
	}

	TreeNode(int value, int cnt)
	{
		left = right = parent = 0;
		val = value;
		color = RED;
		count = cnt;
	}

	TreeNode* left; //left child of the node
	TreeNode* right; //right child of the node
	TreeNode* parent; //parent of the node
	NodeColor color; //color of the node
	int count;		//count of the event
	int val;		//key ot id of the event

};


//main class which implements the RBTree functions of insertion, deletion, etc
class RBTree
{
public:
	RBTree();
	RBTree(std::string fileName);

	~RBTree();

	void Swap(NodeColor nodeA, NodeColor nodeB)
	{
		NodeColor temp = nodeA;
		nodeA = nodeB;
		nodeB= temp;
	}

	bool isNull(TreeNode* pNode)
	{
		return pNode == m_pNullNode;
	}

	void LevelOrderTraversal()
	{
		std::queue<TreeNode *> q;
		q.push(m_pRoot);
 
		while (!q.empty())
		{
			TreeNode *temp = q.front();
			std::cout << temp->val << "  ";
			q.pop();
 
			if (!isNull(temp->left) )
				q.push(temp->left);
 
			if (!isNull(temp->right))
				q.push(temp->right);
		}
	}

	TreeNode* GetRoot()
	{
		return m_pRoot;
	}

	//rotates the node
	void RotateLeft(TreeNode *x);
	void RotateRight(TreeNode *x);

	//Return null if the key is not present in the tree
	TreeNode* FindNode(int key);

	//Checks if a key already exists. If key exists, it calls the RemoveNode function
	void Insert(int key);
	void Insert(int key, int count);

	//Rebalances the tree after insertion
	void Insert_FixUp(TreeNode *z);

	//Returns the node with smallest key in the tree rooted at pNode
	TreeNode* TreeMinimun(TreeNode* pNode);

	//Returns the node with the key just larger than that of pNode
	TreeNode* TreeSuccessor(TreeNode* pNode);

	//remove the node having key
	void Remove(int key);

	void Remove_FixUp(TreeNode* x);

	void Increase(int ID, int count);

	void Reduce(int ID, int count);

	void Count(int ID);

	void InRange(int ID1, int ID2);

	//Displays smallest value greater than the given value
	void Next(int value)
	{
		TreeNode* pNext = m_pNullNode;
		int minNode = INT_MAX;
		TreeNode* pNode = m_pRoot;
		while(!isNull(pNode))
		{

			if(pNode->val <= value)
				pNode = pNode->right;
			else
			{
				if(pNode->val < minNode)
				{
					minNode = pNode->val;
					pNext = pNode;
					pNode = pNode->left;
				}
			}
		}

		if(!isNull(pNext))
			std::cout<<"\n"<<pNext->val<<" "<<pNext->count;
		else
			std::cout<<"\n0,0";
	}

	//returns the node with the smallest value greater than the given value
	TreeNode* NextNode(int value)
	{
		TreeNode* pNext = m_pNullNode;
		int minNode = INT_MAX;
		TreeNode* pNode = m_pRoot;
		while(!isNull(pNode))
		{

			if(pNode->val <= value)
				pNode = pNode->right;
			else
			{
				if(pNode->val < minNode)
				{
					minNode = pNode->val;
					pNext = pNode;
					pNode = pNode->left;
				}
			}
		}

		return pNext;
	}

	//displays the greatest value smaller than the given value
	TreeNode* Previous(int value)
	{
		TreeNode* pPrev = m_pNullNode;
		int maxNode = INT_MIN;
		TreeNode* pNode = m_pRoot;
		while(!isNull(pNode))
		{

			if(pNode->val >= value)
				pNode = pNode->left;
			else
			{
				if(pNode->val > maxNode)
				{
					maxNode = pNode->val;
					pPrev = pNode;
					pNode = pNode->right;
				}
			}
		}

		if(!isNull(pPrev))
			std::cout<<"\n"<<pPrev->val<<" "<<pPrev->count;
		else
			std::cout<<"\n0,0";

		return pPrev;
	}

	//Initialise the RBTree
	TreeNode* initRBTree(int* pID, int* pCnt, int start, int end, TreeNode* pParent, int currHeight);
	
private:

	//inserts node into the RBTree
	void Insert(TreeNode *z);

	//removes the node from the BS Tree
	void Remove(TreeNode *z);

private:

	TreeNode* m_pRoot;
	TreeNode* m_pNullNode;
	std::ifstream m_file;
	int m_maxHeight;
};


RBTree::RBTree()
{
	m_pNullNode = new TreeNode();
	m_pNullNode->color = BLACK;
	m_pRoot = m_pNullNode;
	m_pRoot->parent = m_pRoot->left = m_pRoot->right = m_pNullNode;
	m_maxHeight = 0;
}


//constructor gets the filename and buils the tree according to the contents of the file
RBTree::RBTree(std::string fileName)
{
	m_pNullNode = new TreeNode();
	m_pNullNode->color = BLACK;
	
	m_pRoot = m_pNullNode;
	m_pRoot->parent = m_pRoot->left = m_pRoot->right = m_pNullNode;

	m_maxHeight = 0;

	m_file.open(fileName.c_str());
	if(m_file)
	{
		int nodeCount;
		m_file>>nodeCount;

		int *pID = new int[nodeCount];
		int *pCnt = new int[nodeCount];

		for(int i = 0; i < nodeCount; i++)
		{
			if(m_file>>pID[i]);
			if(m_file>>pCnt[i]);
		}

		m_file.close();
		m_maxHeight = (int) ceil(log(nodeCount + 1)/log(2));

		m_pRoot = initRBTree(pID, pCnt, 0, nodeCount-1, m_pNullNode, 0);

		//std::cout<<"Tree Constructed";
		m_pRoot->color = BLACK;
	}
}

RBTree::~RBTree()
{

}


//performs a left rotate operation on x
void RBTree::RotateLeft(TreeNode *x)
{
	TreeNode *y = x->right;
	x->right = y->left;

	if (!isNull(y->left))
		y->left->parent = x;

	y->parent = x->parent;

	//if x's parent is null
	if (isNull(x->parent))
		m_pRoot = y;
	//else if x is the left child of it's parent
	else if (x == x->parent->left)
		x->parent->left = y;
	//else x is the right child of it's parent
	else
		x->parent->right = y;

	y->left = x;
	x->parent = y;
}

//performs a right rotate operation on x
void RBTree::RotateRight(TreeNode *y)
{
	TreeNode *x = y->left;

	y->left = x->right;

	if (!isNull(x->right))
		x->right->parent = y;

	x->parent = y->parent;

	// y's parent is nil
	if (isNull(y->parent))
		m_pRoot = x;

	//else if y is a right child of it's parent.
	else if (y == y->parent->right)
		y->parent->right = x;
	//else y is a left child of it's parent.
	else
		y->parent->left = x;

	x->right = y;
	y->parent = x;
}

//inserts the node z and rebalances the tree after insertion
void RBTree::Insert(TreeNode* z)
{
	TreeNode* y = m_pNullNode;
	TreeNode* x = m_pRoot;

	while(!isNull(x))
	{
		y = x;
		if(z->val < x->val)
			x = x->left;
		else
			x = x->right;
	}

	z->parent = y;

	if(isNull(y))
		m_pRoot = z;

	else if(z->val< y->val)
		y->left = z;
	else
		y->right = z;

	z->left = m_pNullNode;
	z->right = m_pNullNode;
	z->color = RED;

	//rebalancing
	Insert_FixUp(z);

}

void RBTree::Insert_FixUp(TreeNode* z)
{
	TreeNode* y = m_pNullNode;
	// While there is a violation of the RedBlackTree
	while (z->parent->color == RED)
	{
		// If z's parent is the left child of z's grandparent.
		if (z->parent == z->parent->parent->left)
		{
			// Initialize y to z's cousin
			y = z->parent->parent->right;

			// Case 1: if y is red, then just recolor
			if (y->color == RED)
			{
				z->parent->color = BLACK;
				y->color = BLACK;
				z->parent->parent->color = RED;
				z = z->parent->parent;
			}

			// Case 2: if y is black & z is a right child. rotate left around z's parent
			else if (z == z->parent->right)
			{
				z = z->parent;
				RotateLeft(z);
			}

			// Case 3: else y is black & z is a left child
			else
			{
				z->parent->color = BLACK;
				z->parent->parent->color = RED;
				RotateRight(z->parent->parent);
			}
		}

		// If z's parent is the right child of z's grandparent.
		else{

			// set y to z's cousin
			y = z->parent->parent->left;

			// Case 1: if y is red, just recolor
			if (y->color == RED)
			{
				z->parent->color = BLACK;
				y->color = BLACK;
				z->parent->parent->color = RED;
				z = z->parent->parent;
			}

			else if (z == z->parent->left)
			{
				z = z->parent;
				RotateRight(z);
			}
			else
			{
				z->parent->color = BLACK;
				z->parent->parent->color = RED;
				RotateLeft(z->parent->parent);
			}
		}
	}
	
	// The root must remain BLACK
	m_pRoot->color = BLACK;

}

void RBTree::Insert(int val)
{
	/*if(!isNull(FindNode(val)))
	{
		std::cout<<"Key already exists"<<"\n";
		return;
	}*/

	TreeNode* pNode = new TreeNode(val);
	Insert(pNode);
}

void RBTree::Insert(int val, int count)
{
	TreeNode* pNode = new TreeNode(val, count);
	Insert(pNode);
}

//Find the node with the specified  key
TreeNode* RBTree::FindNode(int key)
{
	TreeNode* pNode = m_pRoot;

	while(!isNull(pNode))
	{
		if(pNode->val == key)
			return pNode;
		else if(key < pNode->val)
			pNode = pNode->left;
		else if(key > pNode->val)
			pNode = pNode->right;
	}

	return m_pNullNode;
}

//remove the node with the specified key
void RBTree::Remove(int key)
{
	TreeNode* pNode = FindNode(key);
	if(!pNode)
	{
		std::cout<<"Key not found";
		return;
	}

	Remove(pNode);
}

//gets the node with the minimum value in the tree rooted at pNode
TreeNode* RBTree::TreeMinimun(TreeNode* pNode)
{
	while(!isNull(pNode->left))
		pNode = pNode->left;

	return pNode;
}

//gets the  successor of the node x
TreeNode* RBTree::TreeSuccessor(TreeNode* x)
{
	if (!isNull(x->right))
		return TreeMinimun(x->right);

	TreeNode* y = x->parent;

	while (!isNull(y) && x == y->right)
	{
		x = y;
		y = y->parent;
	}

	return y;
}

//remove node z from the tree and rebalance it
void RBTree::Remove(TreeNode* z)
{
	TreeNode* x = m_pNullNode;
	TreeNode* y = m_pNullNode;

	if (isNull(z->left) || isNull(z->right))
		y = z;

	else y = TreeSuccessor(z);

	// Let x be the left or right child of y
	if (!isNull(y->left))
		x = y->left;
	else
		x = y->right;

	// link x's parent to y's parent
	x->parent = y->parent;

	// If y's parent is null, then x is the root
	if (isNull(y->parent))
		m_pRoot = x;

	// else if y is a left child, set x to be y's left sibling
	else if (!isNull(y->parent->left) && y->parent->left == y)
		y->parent->left = x;

	// else if y is a right child, set x to be y's right sibling
	else if (!isNull(y->parent->right) && y->parent->right == y)
		y->parent->right = x;

	// if y is != z, set y's value to z's value
	if (y != z)
		z->val = y->val;

	// If y's color is black, call removeFixup() to rebalance the tree
	if (y->color == BLACK)
		Remove_FixUp(x);
}


//used for rebalancing the tree
void RBTree::Remove_FixUp(TreeNode* x)
{
	TreeNode* w;

	// While the tree still violates RB properties
	while (x != m_pRoot && x->color == BLACK)
	{
		// if x is it's parent's left child
		if (x == x->parent->left)
		{
			// set w = x's sibling
			w = x->parent->right;

			// Case 1: w's color is red, just recolor
			if (w->color == RED)
			{
				w->color = BLACK;
				x->parent->color = RED;
				RotateLeft(x->parent);
				w = x->parent->right;
			}
			// Case 2: both of w's children are black
			if (w->left->color == BLACK && w->right->color == BLACK)
			{
				w->color = RED;
				x = x->parent;
			}
			// Case 3 & 4
			else
			{
				// Case 3: w's right child is black
				if (w->right->color == BLACK)
				{
					w->left->color = BLACK;
					w->color = RED;
					RotateRight(w);
					w = x->parent->right;
				}
				// Case 4: w = black, w.right = red
				w->color = x->parent->color;
				x->parent->color = BLACK;
				w->right->color = BLACK;
				RotateLeft(x->parent);
				x = m_pRoot;
			}
		}
		// if x is it's parent's right child
		else
		{
			w = x->parent->left;

			if (w->color == RED)
			{
				w->color = BLACK;
				x->parent->color = RED;
				RotateRight(x->parent);
				w = x->parent->left;
			}

			if (w->right->color == BLACK && w->left->color == BLACK)
			{
				w->color = RED;
				x = x->parent;
			}

			else{
				if (w->left->color == BLACK)
				{
					w->right->color = BLACK;
					w->color = RED;
					RotateLeft(w);
					w = x->parent->left;
				}

				w->color = x->parent->color;
				x->parent->color = BLACK;
				w->left->color = BLACK;
				RotateRight(x->parent);
				x = m_pRoot;
			}
		}
	}

	//set the color of to be BLACK
	x->color = BLACK;
}


//increases the count of the node with the specified ID by m
void RBTree::Increase(int ID, int m)
{
	TreeNode* pNode = FindNode(ID);
	if(!isNull(pNode))
	{
		pNode->count+=m;
		std::cout<<"\n"<<pNode->count;
	}
	else
	{
		//std::cout<<"\nevent ID: "<<ID<<" not found.";
		pNode = new TreeNode(ID, m);
		Insert(pNode);
		std::cout<<"\n"<<pNode->count;
	}
}

//decreases the count of the node with the specified ID by m
void RBTree::Reduce(int ID, int m)
{

	TreeNode* pNode = FindNode(ID);
	if(!isNull(pNode))
	{
		pNode->count-=m;
		if(pNode->count > 0)
			std::cout<<"\n"<<pNode->count;
		else
		{
			Remove(pNode);
			std::cout<<"\n0";
		}
	}
	else
		std::cout<<"\n0";
}

//displayes the cpunt of the node with the specified ID
void RBTree::Count(int ID)
{
	TreeNode* pNode = FindNode(ID);
	if(!isNull(pNode))
		std::cout<<"\n"<<pNode->count;
	else
		std::cout<<"\n0";
}

void RBTree::InRange(int ID1, int ID2)
{
	int count = 0;
	TreeNode* pNode = FindNode(ID1);

	if (isNull(pNode))
		pNode = NextNode(ID1);

	if(isNull(pNode))
	{
		std::cout<<"\n";
		std::cout<<"Range out of bounds";
		return;
	}

	if (pNode->val <= ID2)
		count = pNode->count;

	while (!isNull(pNode) && pNode->val <= ID2) 
	{
		pNode = TreeSuccessor(pNode);
		if (pNode->val <= ID2)
			count += pNode->count;
	}

	std::cout<<"\n"<<count;
}

//initialize the RBTree
TreeNode* RBTree::initRBTree(int* pID, int* pCnt, int start, int end, TreeNode* pParent, int currHeight)
{
	if(start > end)
	{
		if(currHeight == m_maxHeight)
			pParent->color = RED;
		return m_pNullNode;
	}

	int mid = (start+end)/2;
	TreeNode* pNode = new TreeNode(pID[mid], pCnt[mid]);
	pNode->color = BLACK;

	pNode->parent = pParent;
	pNode->left = initRBTree(pID, pCnt, start, mid-1, pNode, currHeight+1);
	pNode->right = initRBTree(pID, pCnt, mid+1, end, pNode, currHeight+1);

	return pNode;

}

int main(int argc, char** argv)
{
	//check if command line arguments are more than 1. if true, proceed
	if ( argc > 1 )
	{
		RBTree tree(argv[1]);

		std::string line = "";

		while(std::cin) 
		{
			std::cin>>line;
			//keep looping till quit command is not encountered
			if(line == "quit")
				break;

			if(line == "increase")
			{
				int ID;
				std::cin>>ID;
				int count;
				std::cin>>count;
				tree.Increase(ID, count);
			}
			else if(line == "reduce")
			{
				int ID;
				std::cin>>ID;
				int count;
				std::cin>>count;
				tree.Reduce(ID, count);
			}
			else if(line == "inrange")
			{
				int ID1;
				std::cin>>ID1;
				int ID2;
				std::cin>>ID2;
				tree.InRange(ID1, ID2);
			}
			else if(line == "count")
			{
				int ID;
				std::cin>>ID;
				tree.Count(ID);
			}
			else if(line == "next")
			{
				int ID;
				std::cin>>ID;
				tree.Next(ID);
			}
			else if(line == "previous")
			{
				int ID;
				std::cin>>ID;
				tree.Previous(ID);
			}
		}
		
	}
}


