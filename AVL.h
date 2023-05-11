#ifndef AVL_H
#define AVL_H

#include <vector>
#include <string>
#include <stack>

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
    int height(Node* node);
    int getBalance(Node* node);
	int max(int a, int b);
	Node* removeHelper(Node* node, T element);


public:
	AVL();
	~AVL();
	void insert(T element);
	void remove(T element);
	bool find(T element);
	std::vector<T> inOrderWalk();
	std::vector<T> preOrderWalk();
	std::vector<T> postOrderWalk();
	int get_TreeHeightHelper(Node* node);
    int getTreeHeight();
	T getMin();
	T getMax();
	std::string ToGraphviz();
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
typename AVL<T>::Node* AVL<T>::insertNode(Node* node, T element)
{
	//Trädet är tomt eller en lövnod har nåtts
	if(node == nullptr)
	{
		return new Node*(element);
	}

	//Sätt in i rätt subtråd beroende på jämförelsen mellan elementet och noden
	if(element < node->element)
	{
		node->left = insertNode(node->left, element);
	}

	else if(element > node->element)
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
	if(balance > 1 && element > node->left->element)
	{
		return rightRotate(node);
	}

	if(balance < -1 && element > node->right->element)
	{
		return leftRotate(node);
	}

	if(balance > 1 && element > node->left->right->element)
	{
		node->left = leftRotate(node->left);
		return rightRotate(node);
	}

	if(balance < -1 && element < node->right->element)
	{
		node->right = rightRotate(node->right);
		return leftRotate(node);
	}

	//Returnerar oförändrad nod
	return node;
}

template<class T>
typename AVL<T>::Node* AVL<T>::leftRotate(Node* node)
{
	//Child = Leaf
	Node* rightchild = node->right;
	Node* rightgrandchild = rightchild->right;

	//Roteringssteg
	rightchild->left = node;
	node->right = rightgrandchild;

	//Uppdatera höjden
	node->height = 1 + max(height(node->left), height(node->right));
	rightchild->height = 1 + max(height(rightchild->left), height(rightchild->right));

	//Returnerar den nya roten
	return rightchild;
}

template<class T>
typename AVL<T>::Node* AVL<T>::rightRotate(Node* node)
{
	Node* leftchild = node->left;
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
int AVL<T>::height(Node* node)
{
	if(node == nullptr)
	{
		return 0;
	}
	return node->height;
}

template<class T>
int AVL<T>::getBalance(Node* node)
{
	if(node == nullptr)
	{
		return 0;
	}
	return height(node->left) - height(node->right);
}

int max(int a, int b)
{
    return (a > b)? a : b;
}


template <class T>
void AVL<T>::remove(T element)
{
    root = removeHelper(root, element);
}

template <class T>
typename AVL<T>::Node* AVL<T>::removeHelper(Node* node, T element)
{
    if (node == nullptr) 
	{
        return nullptr;
    }
    
    if (element < node->element) 
	{
        node->left = removeHelper(node->left, element);
    } 
	
	else if (element > node->element) 
	{
        node->right = removeHelper(node->right, element);
    } 
	
	else {
        Node* left = node->left;
        Node* right = node->right;

        if (left == nullptr && right == nullptr) 
		{
            delete node;
            return nullptr;
        } 
		
		else if (left == nullptr) 
		{
            delete node;
            return right;
        } 
		
		else if (right == nullptr) 
		{
            delete node;
            return left;
        } 
		
		else 
		{
            Node* temp = getMin(right);
            node->element = temp->element;
            node->right = removeHelper(right, temp->element);
        }
    }

    node->height = 1 + max(height(node->left), height(node->right));
    int balance = getBalance(node);

    if (balance > 1 && getBalance(node->left) >= 0) 
	{
        return rightRotate(node);
    } 
	
	else if (balance > 1 && getBalance(node->left) < 0) 
	{
        node->left = leftRotate(node->left);
        return rightRotate(node);
    } 
	else if (balance < -1 && getBalance(node->right) <= 0) 
	{
        return leftRotate(node);
    } 
	
	else if (balance < -1 && getBalance(node->right) > 0) 
	{
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node;
}

template <class T>
bool AVL<T>::find(T element)
{
	Node *current = root;
	while(current != nullptr)	
	{
		if(element == current->element)
		{
			return true;
		}

		else if(element < current->element)
		{
			current = current->left;
		}

		else
		{
			current = current->right;
		}			
	}
	return false;
}

template <class T>
std::vector<T> AVL<T>::inOrderWalk()
{
	std::vector<T> elements;
	std::stack<Node*>	stack;
	Node* current = root;

	while(current != nullptr || !stack.empty())
	{
		while(current != nullptr)
		{
			stack.push(current);
			current = current->left;
		}

		current = stack.top();
		stack.pop();
		elements.push_back(current->element);
		current = current->right;	
	}
	return elements;
}

template <class T>
std::vector<T> AVL<T>::preOrderWalk()
{
	std::vector<T> result;

	if(root == nullptr)
	{
		return result;
	}

	std::stack<Node*> stack;
	stack.push(root);

	while(!stack.empty())
	{
		Node* current = stack.top();
		stack.pop();

		result.push_back(current->element);

		if(current->right != nullptr)
		{
			stack.push(current->right);
		}

		if(current->left != nullptr)
		{
			stack.push(current->left);
		}
	}
	return result;
}

template <class T>
std::vector<T> AVL<T>::postOrderWalk()
{
	std::vector<T> result;

	if(root == nullptr)
	{
		return result;
	}

	std::stack<Node*> s1, s2;
	s1.push(root);

	while(!s1.empty())
	{
		Node* node =s1.top();
		s1.pop();
		s2.push(node);

		if(node->left != nullptr)
		{
			s1.push(node->left);
		}

		if(node->right != nullptr)
		{
			s1.push(node->right);
		}
	}

	while(!s2.empty())
	{
		Node* node = s2.top();
		s2.pop();
		result.push_back(node->element);
	}
	return result;
}

template <class T>
int AVL<T>::getTreeHeight()
{
	return get_TreeHeightHelper(root);
}

template <class T>
int AVL<T>::get_TreeHeightHelper(Node* node)
{
	if(node == nullptr)
	{
		return -1; //Höjden på en tom nod är -1
	}

	int leftHeight = get_TreeHeightHelper(node->left); //Höjden för vänster delträd
	int rightHeight = get_TreeHeightHelper(node->right); //Höjden för höger delträd

	return 1 + std::max(leftHeight, rightHeight); //Höjden för noden är 1 plus högsta höjden av delträden
}

template <class T>
T AVL<T>::getMin()
{
	Node* current = root;
	while(current->left != nullptr)
	{
		current = current->left;
	}
	return current->element;
}

template <class T>
T AVL<T>::getMax()
{
    Node* current = root;
	while(current->right != nullptr)
	{
		current = current->right;
	}
	return current->element;
}

template <class T>
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
