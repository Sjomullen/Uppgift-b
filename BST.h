#ifndef BST_H
#define BST_H

#include <vector>
#include <string>
#include <stack>

template <class T>
class BST
{
private:
	class Node
	{
	public:
		T element;
		Node *left;
		Node *right;
		Node() {};
		~Node() {};
	};

	Node *root;
	void ToGraphvizHelper(std::string& listOfNodes, std::string& listOfConnections, Node* toWorkWith, size_t& uniqueID);
public:
	BST();
	~BST();
	void insert(T element);
	void remove(T element);
	bool find(T element);
	std::vector<T> inOrderWalk();
	std::vector<T> preOrderWalk();
	std::vector<T> postOrderWalk();
	int get_Height(Node* node);
	int getTreeHeight();
	T getMin();
	T getMax();
	std::string ToGraphviz();

};

template <class T>
BST<T>::BST()
{
	root = nullptr;
}
	
template <class T>
BST<T>::~BST()
{
	delete root;
}

template <class T>
void BST<T>::insert(T element)
{
	Node *newNode = new Node();
	newNode->element = element;
	newNode->left = nullptr;
	newNode->right = nullptr;

	if(!root)
	{
        root = newNode;
		return;
    }

	Node *current = root;
	while(current)
	{
		if(element < current->element)
		{
			if(!current->left)
			{
                current->left = newNode;
                return;
            }
			current = current->left;

		}

		else if(element > current->element)
		{
			if(!current->right)
			{
				current->right = newNode;
                return;
			}
			current = current->right;
		}

		else
		{
			return;
		}
	}
}

template <class T>
void BST<T>::remove(T element)
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
}

template <class T>
bool BST<T>::find(T element)
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
std::vector<T> BST<T>::inOrderWalk()
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
std::vector<T> BST<T>::preOrderWalk()
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
std::vector<T> BST<T>::postOrderWalk()
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
int BST<T>::getTreeHeight()
{
	return get_Height(root);
}

template <class T>
int BST<T>::get_Height(Node* node)
{
	if(node == nullptr)
	{
		return -1; //Höjden på en tom nod är -1
	}

	int leftHeight = get_Height(node->left); //Höjden för vänster delträd
	int rightHeight = get_Height(node->right); //Höjden för höger delträd

	return 1 + std::max(leftHeight, rightHeight); //Höjden för noden är 1 plus högsta höjden av delträden
}


template <class T>
T BST<T>::getMin()
{
	Node* current = root;
	while(current->left != nullptr)
	{
		current = current->left;
	}
	return current->element;
}

template <class T>
T BST<T>::getMax()
{
	Node* current = root;
	while(current->right != nullptr)
	{
		current = current->right;
	}
	return current->element;
}


template <class T>

std::string BST<T>::ToGraphviz() // Member function of the AVLTree class

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
void BST<T>::ToGraphvizHelper(std::string& listOfNodes, std::string& listOfConnections, Node* toWorkWith, size_t& uniqueID) // Member function of the AVLTree class
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
#endif //BST_H
