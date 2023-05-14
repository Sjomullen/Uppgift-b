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
		Node(T element);
		~Node();
	};
	Node *root;
	void ToGraphvizHelper(std::string& listOfNodes, std::string& listOfConnections, Node* toWorkWith, size_t& uniqueID);



public:
	AVL();
	~AVL();
	void insert(T element);
	Node* insertNode(Node* node, T element);
	void leftRotate(Node* node, Node* parent);
	void rightRotate(Node* node, Node* parent);
    int height(Node* node);
    int getBalance(Node* node);
	Node* removeHelper(Node* node, T element, bool& found);
	void remove(T element);
	int max(int a, int b);
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
AVL<T>::Node::Node(T element)
{
	this->element = element;
	this->left = nullptr;
	this->right = nullptr;
	this->height = 0;
}

template <class T>
AVL<T>::Node::~Node()
{

}


template <typename T>
void AVL<T>::insert(T element)
{
    std::stack<Node*> nodes;

    if (root == nullptr)
    {
        root = new Node(element);
    }
    else if (!find(element))
    {
        Node* current = root;
        Node* parent = nullptr;

        while (current != nullptr)
        {
            nodes.push(current);
            parent = current;
            if (element < current->element)
            {
                current = current->left;
            }
            else
            {
                current = current->right;
            }
        }

        if (element < parent->element)
        {
            parent->left = new Node(element);
        }
        else
        {
            parent->right = new Node(element);
        }

        //Balancera trädet
        while (!nodes.empty())
		{
			Node* rotationNode = nodes.top();
            nodes.pop();

			Node* rotationParent;
			if (!nodes.empty())
				rotationParent = nodes.top();
			else
				rotationParent = nullptr;

			rotationNode->height = 1 + max(height(rotationNode->left), height(rotationNode->right));

			//Beräkna balansfaktorn för denna noden
			int balance = getBalance(rotationNode);

			//Om noden har en balansfaktor som inte är tillåten, balansera trädet
			if(balance > 1)
			{
				if(getBalance(rotationNode->left) < 0)
				{
					leftRotate(rotationNode->left, rotationNode);
				}
				rightRotate(rotationNode, rotationParent);
			}

			if(balance < -1)
			{
				if(getBalance(rotationNode->right) > 0)
				{
					rightRotate(rotationNode->right, rotationNode);
				}
				leftRotate(rotationNode, rotationParent);
			}
		}
    }
}

template<class T>
typename AVL<T>::Node* AVL<T>::insertNode(Node* node, T element)
{
//Trädet är tomt eller en lövnod har nåtts
if(node == nullptr)
{
    return new Node(element);
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

//Beräkna balansfaktorn för denna noden
int balance = getBalance(node);

//Om noden har en balansfaktor som inte är tillåten, balansera trädet
if(balance > 1)
{
	if(getBalance(node->left) < 0)
	{
   		leftRotate(node->left->right, node->left);
	}
	rightRotate(node->left, node);
}

if(balance < -1)
{
	if(getBalance(node->right) > 0)
	{
    	rightRotate(node->right->left, node->right);
	}
    leftRotate(node->right, node);
}

//Returnerar oförändrad nod
return node;
}

template<class T>
void AVL<T>::rightRotate(Node* node, Node* parent)
{
	Node* localRoot = node->left;
    node->left = localRoot->right;
    localRoot->right = node;

    if (parent != nullptr)
    {
        if (parent->left == node)
            parent->left = localRoot;
        else
            parent->right = localRoot;
    }
    else
        root = localRoot;


	//Uppdatera höjden
	//target->height = 1 + max(height(node->right), height(node->left));
	//localroot->height = 1 + max(height(rightchild->right), height(rightchild->left));

	//Returnerar den nya roten
}

template<class T>
void AVL<T>::leftRotate(Node* node, Node* parent)
{
	Node* localRoot = node->right;
    node->right = localRoot->left;
    localRoot->left = node;

    //Replace pointers of parent if it exists
    if (parent != nullptr)
    {
        if (parent->right == node)
            parent->right = localRoot;
        else
            parent->left = localRoot;
    }
    else
        root = localRoot;

	//Uppdatera höjden
	//node->height = 1 + max(height(node->right), height(node->left));
	//target->height = 1 + max(height(rightchild->right), height(rightchild->left));

	//Returnerar den nya roten
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

template<class T>
int AVL<T>::max(int a, int b)
{
    return (a > b)? a : b;
}

template<class T>
void AVL<T>::remove(T element) 
{
	Node *current = root;
    Node *parent = nullptr;

    while(current != nullptr)
    {
		if (element == current->element)
		{
			// Found the node to remove
			if(current == root)
			{
				// If the root node is the one to remove, 
				// replace it with its right subtree
				Node *temp = root;
				if(root->right == nullptr)
				{
					root = root->left;
				}

				else if(root->left == nullptr)
				{
					root = root->right;
				}

				else
				{
					root = root->right;
					while(root->left != nullptr)
					{
						root = root->left;
					}
					root->left = temp->left;
				}
				delete temp;
			}

			else
			{
				// Otherwise, adjust parent's left or right 
				// pointer to point to current's right subtree
				if(parent->left == current)
				{
					Node *temp = current;
					if(current->right == nullptr)
					{
						parent->left = current->left;
					}

					else if(current->left == nullptr)
					{
						parent->left = current->right;
					}

					else
					{
						parent->left = current->right;
						while(parent->left->left != nullptr)
						{
							parent->left = parent->left->left;
						}
						parent->left->left = temp->left;
					}
					delete temp;
				}
				else
				{
					Node *temp = current;
					if(current->right == nullptr)
					{
						parent->right = current->left;
					}

					else if(current->left == nullptr)
					{
							parent->right = current->right;
					}

					else
					{
						parent->right = current->right;
						while(parent->right->left != nullptr)
						{
								parent->right = parent->right->left;
						}
						parent->right->left = temp->left;
					}
					delete temp;
				}
			}
			return;
		}
		else if(element < current->element)
		{
			parent = current;
			current = current->left;
		}

		else
		{
			parent = current;
			current = current->right;
		}
	}

	int balance = getBalance(current);
	if(balance == 2 && getBalance(current->left) >= 0)
	{
		return rightRotate(current);
	}

	else if(balance == 2 && getBalance(current->left) == -1)
	{
		current->left = leftRotate(current->left);
		return rightRotate(current);
	}

	else if(balance == -2 && getBalance(current->right) <= -0)
	{
		return leftRotate(current);
	}

	else if(balance == -2 && getBalance(current->right) == 1)
	{
		current->right = rightRotate(current->right);
        return leftRotate(current);
	}


	return current;
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
