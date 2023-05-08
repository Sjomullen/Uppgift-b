#ifndef AVL_H
#define AVL_H

#include <vector>
#include <string>

template <class T>
class AVL
{
private:
	class Node
	{
	public:
		T element;
		Node *left;
		Node *right;
		int height;
		Node() {};
		~Node() {};
	};
	Node *root;
	void ToGraphvizHelper(std::string& listOfNodes, std::string& listOfConnections, Node* toWorkWith, size_t& uniqueID);
	Node* insertNode(Node* node, T element);
	Node* leftRotate(Node* node);
	Node* rightRotate(Node* node);
public:
	AVL();
	~AVL();
	void insert(T element);
	void remove(T element);
	bool find(T element);
	std::vector<T> inOrderWalk();
	std::vector<T> preOrderWalk();
	std::vector<T> postOrderWalk();
	int getTreeHeight();
	T getMin();
	T getMax();
	std::string ToGraphviz();
};

template <class T>
class AVLNode {
public:
    T dataset;
    AVLNode<T> *left;
    AVLNode<T> *right;
    int height;

    AVLNode(T dataset) {
        this->dataset = dataset;
        left = nullptr;
        right = nullptr;
        height = 1;
    }
};

template <class T>
AVL<T>::AVL()
{
	root = nullptr;
}

template <class T>
AVL<T>::~AVL()
{
	delete root;
}

template <class T>
void AVL<T>::insert(T element)
{
	root = insertNode(root, element);
}

template<class T>
AVLNode<T>* AVL<T>::insertNode(AVLNode<T>* node, T element)
{
	//Trädet är tomt eller en lövnod har nåtts
	if(node == nullptr)
	{
		return new AVLNode<T>(element);
	}

	//Sätt in i rätt subtråd beroende på jämförelsen mellan elementet och noden
	if(element < node->dataset)
	{
		node->left = insertNode(node->left, element);
	}

	else if(element > node->dataset)
	{
		node->right = insertNode(node->right, element);
	}

	else //elementet finns redan i trädet
	{
		return node;
	}

    //Uppdatera höjden på den här noden
	node->height = 1 + max(height(node->left), height(node->right));

	//Beräkna balansfakotrn för denna noden
	int balance = getBalance(node);
	
	//Om noden har en balansfakor som inte är tillåten, balansera trädet
	if(balance > 1 && element > node->left->dataset)
	{
		return rightRotate(node);
	}

	if(balance < -1 && element > node->right->dataset)
	{
		return leftRotate(node);
	}

	if(balance > 1 && element > node->left->right->dataset)
	{
		node->left = leftRotate(node->left);
		return rightRotate(node);
	}

	if(balance < -1 && element < node->right->dataset)
	{
		node->right = rightRotate(node->right);
		return leftRotate(node);
	}

	//Returnerar oförändrad nod
	return node;
}

template<class T>
AVLNode<T>* AVL<T>::leftRotate(AVLNode<T>* node)
{
	//Child = Leaf
	AVLNode<T>* rightchild = node->right;
	AVLNode<T>* rightgrandchild = rightchild->left;

	//Roteringssteg
	rightchild->left = node;
	node->right = leftgrandchild;

	//Uppdatera höjden
	node->height = 1 + max(height(node->left), height(node->right));
	rightchild->height = 1 + max(height(rightchild->left), height(rightchild->right));

	//Returnerar den nya roten
	return rightchild;
}

template<class T>
AVLNode<T>* AVL<T>::rightRotate(AVLNode<T>* node)
{
	AVLNode<T>* leftchild = node->left;
	AVL<T>* rightgrandchild = leftchild->right;

	//Roteringssteg
	leftchild->right = node;
	node->left = rightgrandchild;

	//Uppdatera höjden
	node->height = 1 + max(height(node->left), height(node->right));
	leftchild->height = 1 + max(height(leftchild->left), height(leftchild->right));

	//Returnerar den nya roten
	return leftchild;
}

template<class T>
int AVL<T>::height(AVLNode<T>* node)
{
	if(node == nullptr)
	{
		return 0;
	}
	return node->height;
}

template<class T>
int AVL<T>::getBalance(AVLNode<T>* node)
{
	if(node == nullptr)
	{
		return 0;
	}
	return height(node->left) - height(node->right);
}

template <class T>
void AVL<T>::remove(T element)
{
        
}

template <class T>
bool AVL<T>::find(T element)
{
        
}

template <class T>
std::vector<T> AVL<T>::inOrderWalk()
{
        
}

template <class T>
std::vector<T> AVL<T>::preOrderWalk()
{
        
}
	
template <class T>
std::vector<T> AVL<T>::postOrderWalk()
{
        
}

template <class T>
int AVL<T>::getTreeHeight()
{
    
}

template <class T>
T AVL<T>::getMin()
{
        
}

template <class T>
T AVL<T>::getMax()
{
        
}

std::string AVL<T>::ToGraphviz() // Member function of the AVLTree class
{
	std::string toReturn = "";
	if(this->root) // root is a pointer to the root node of the tree
	{
		std::string listOfNodes;
		std::string listOfConnections = std::string("\t\"Root\" -> ") + std::to_string(0) + std::string(";\n");
		toReturn += std::string("digraph {\n");
		size_t id = 0;
		ToGraphvizHelper(listOfNodes, listOfConnections, root, id);
		toReturn += listOfNodes;
		toReturn += listOfConnections;
		toReturn += std::string("}");
	}
	return toReturn;
}

template <class T>
void AVL<T>::ToGraphvizHelper(std::string& listOfNodes, std::string& listOfConnections, Node* toWorkWith, size_t& uniqueID) // Member function of the AVLTree class
{
	size_t myID = uniqueID;
	listOfNodes += std::string("\t") + std::to_string(myID) + std::string(" [label=\"") + std::to_string(toWorkWith->element) + std::string("\"];\n");
	if(toWorkWith->left)
	{
		listOfConnections += std::string("\t") + std::to_string(myID) + std::string(" -> ") + std::to_string(uniqueID + 1) + std::string(" [color=blue];\n");
		ToGraphvizHelper(listOfNodes, listOfConnections, toWorkWith->left, ++uniqueID);
	}
	else
	{
		listOfNodes += std::string("\t") + std::to_string(++uniqueID) + std::string(" [label=") + std::string("nill, style = invis];\n");
		listOfConnections += std::string("\t") + std::to_string(myID) + std::string(" -> ") + std::to_string(uniqueID) + std::string(" [ style = invis];\n");
	}
			
	if(toWorkWith->right)
	{
		listOfConnections += std::string("\t") + std::to_string(myID) + std::string(" -> ") + std::to_string(uniqueID + 1) + std::string(" [color=red];\n");
		ToGraphvizHelper(listOfNodes, listOfConnections, toWorkWith->right, ++uniqueID);
	}
	else
	{
		listOfNodes += std::string("\t") + std::to_string(++uniqueID) + std::string(" [label=") + std::string("nill, style = invis];\n");
		listOfConnections += std::string("\t") + std::to_string(myID) + std::string(" -> ") + std::to_string(uniqueID) + std::string(" [ style = invis];\n");
	}
}
#endif //AVL_H
