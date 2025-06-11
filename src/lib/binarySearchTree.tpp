#include <binarySearchTree.hxx>

template <typename T>
BinarySearchTreeNode<T>::BinarySearchTreeNode() : data(), left(nullptr), right(nullptr) {}

template <typename T>
BinarySearchTreeNode<T>::BinarySearchTreeNode(const BinarySearchTreeNode& other) : data(other.data), left(nullptr), right(nullptr) {
  if (other.left != nullptr) {
    left = new BinarySearchTreeNode<T>(*other.left);
  }
  if (other.right != nullptr) {
    right = new BinarySearchTreeNode<T>(*other.right);
  }
}

template <typename T>
BinarySearchTreeNode<T>::BinarySearchTreeNode(BinarySearchTreeNode&& other) : data(other.data), left(other.left), right(other.right) {
	other.left = nullptr;
	other.right = nullptr;
}

template <typename T>
BinarySearchTreeNode<T>& BinarySearchTreeNode<T>::operator=(BinarySearchTreeNode&& other) {
	if (this != other) {
		data = other.data;
		delete left;
		delete right;
		left = other.left;
		right = other.right;
		other.left = nullptr;
		other.right = nullptr;
	}
	return *this;
}

template <typename T>
BinarySearchTreeNode<T>::BinarySearchTreeNode(T newData) : data(newData), left(nullptr), right(nullptr) {}

template <typename T>
T BinarySearchTreeNode<T>::getData() {
	return data;
}

template <typename T>
T* BinarySearchTreeNode<T>::getPData() {
	return &data;
}

template <typename T>
bool BinarySearchTreeNode<T>::operator==(const BinarySearchTreeNode<T>& otherNode) const {
  bool dataEqual = this->data == otherNode.data;
  bool leftEqual = (this->left == nullptr && otherNode.left == nullptr) || (this->left != nullptr && otherNode.left != nullptr && *this->left == *otherNode.left);
  bool rightEqual = (this->right == nullptr && otherNode.right == nullptr) || (this->right != nullptr && otherNode.right != nullptr && *this->right == *otherNode.right);
  return dataEqual && leftEqual && rightEqual;
}

template <typename T>
BinarySearchTree<T>::BinarySearchTree() : root(nullptr) {}

template <typename T>
BinarySearchTree<T>::BinarySearchTree(const BinarySearchTree& other) : root(nullptr) {
  if (other.root != nullptr) {
    root = new BinarySearchTreeNode<T>(*other.root);
  }
}

template <typename T>
BinarySearchTree<T>& BinarySearchTree<T>::operator=(const BinarySearchTree& other) {
  if (this != &other) {
    deletePostOrder(root);
    root = nullptr;
    if (other.root != nullptr) {
      root = new BinarySearchTreeNode<T>(*other.root);
    }
  }
  return *this;
}

template <typename T>
BinarySearchTree<T>::BinarySearchTree(BinarySearchTree&& other) : root(other.root) {
	other.root = nullptr;
}

template <typename T>
BinarySearchTree<T>& BinarySearchTree<T>::operator=(BinarySearchTree&& other) {
	if (this != other) {
		deletePostOrder(root);
		root = other.root;
		other.root = nullptr;
	}
	return *this;
}

template <typename T>
BinarySearchTree<T>::~BinarySearchTree() {
  deletePostOrder(root);
  root = nullptr;
}

template <typename T>
void BinarySearchTree<T>::insert(const T& data) {
  if (!root) {
    root = new BinarySearchTreeNode(data);
    return;
  }
  recursiveInsert(data, root);
}

template <typename T>
void BinarySearchTree<T>::remove(const T& data) {
  BinarySearchTreeNode<T>* toRemove = search(data);
  if (!toRemove) {
    return;
  }
  if (!toRemove->left || !toRemove->right) {
    BinarySearchTreeNode<T>* temp = nullptr;
    if (!toRemove->left) {
      temp = toRemove->right;
    } else if (!toRemove->right) {
      temp = toRemove->left;
    }
    if (toRemove == root) {
      root = temp;
      delete toRemove;
      return;
    }
    BinarySearchTreeNode<T>* parent = findParent(toRemove);
    if (toRemove->data < parent->data) {
      parent->left = temp;
    } else {
      parent->right = temp;
    }
    delete toRemove;
    return;
  }
  BinarySearchTreeNode<T>* successor = findSuccessor(toRemove);
  T temp = successor->data;
  remove(successor->data);
  toRemove->data = temp;
}

template <typename T>
BinarySearchTreeNode<T>* BinarySearchTree<T>::search(const T& target) const {
  return recursiveSearch(target, root);
}

template <typename T>
void BinarySearchTree<T>::print() {
  std::cout << "Tree's contents: " << std::endl;
  if (!root) {
    std::cout << "Tree is empty" << std::endl;
    return;
  }
  printInOrder(root);
  std::cout << std::endl;
}

template <typename T>
BinarySearchTreeNode<T>* BinarySearchTree<T>::findParent(const BinarySearchTreeNode<T>* child) {
  if (child == root) {
    return nullptr;
  }
  BinarySearchTreeNode<T>* parent = root;
  while (parent) {
    if (parent->left == child || parent->right == child) {
      return parent;
    }
    if (child->data <= parent->data) {
      parent = parent->left;
    } else {
      parent = parent->right;
    }
  }
  return nullptr;
}

template <typename T>
BinarySearchTreeNode<T>* BinarySearchTree<T>::findSuccessor(const BinarySearchTreeNode<T>* predecessor) {
  BinarySearchTreeNode<T>* successor = predecessor->right;
  while (successor && successor->left) {
    successor = successor->left;
  }
  return successor;
}

template <typename T>
BinarySearchTreeNode<T>* BinarySearchTree<T>::recursiveSearch(const T& target, BinarySearchTreeNode<T>* current) const {
  if (!current) {
    return nullptr;
  }
  if (target == current->data) {
    return current;
  } else if (target < current->data) {
		return recursiveSearch(target, current->left);
  } else {
    return recursiveSearch(target, current->right);
  }
}

template <typename T>
void BinarySearchTree<T>::recursiveInsert(const T& data, BinarySearchTreeNode<T>* current) {
  if (data <= current->data) {
    if (!current->left) {
      current->left = new BinarySearchTreeNode(data);
    } else {
      recursiveInsert(data, current->left);
    }
  } else {
    if (!current->right) {
      current->right = new BinarySearchTreeNode(data);
    } else {
      recursiveInsert(data, current->right);
    }
  }
}

template <typename T>
void BinarySearchTree<T>::printInOrder(const BinarySearchTreeNode<T>* current) {
  if (!current) {
    return;
  }
  printInOrder(current->left);
  std::cout << current->data << " ";
  printInOrder(current->right);
}

template <typename T>
void BinarySearchTree<T>::deletePostOrder(BinarySearchTreeNode<T>* current) {
  if (current) {
    deletePostOrder(current->left);
    deletePostOrder(current->right);
    delete current;
  }
}

template <typename T>
int BinarySearchTree<T>::getSizeRecursive(BinarySearchTreeNode<T>* current) {
	if (!current) {
		return 0;
	}
	return 1 + getSizeRecursive(current->left) + getSizeRecursive(current->right);
}

template <typename T>
void BinarySearchTree<T>::clear() {
  deletePostOrder(root);
  root = nullptr;
}

template <typename T>
BinarySearchTreeNode<T>* BinarySearchTree<T>::getRoot() {
	return root;
}

template <typename T>
BinarySearchTreeNode<T>* BinarySearchTree<T>::getMax() {
	if (!root) {
		return nullptr;
	}
	BinarySearchTreeNode<T>* current = root;
	while (current->right) {
		current = current->right;
	}
	if (current->left) {
		if (current->data == current->left->data) {
			current = current->left;
		}
	}
	return current;
}

template<typename T>
int BinarySearchTree<T>::getSize() {
	if (!root) {
		return 0;
	}
	return 1 + getSizeRecursive(root->left) + getSizeRecursive(root->right);
}

template <typename T>
bool BinarySearchTree<T>::operator==(const BinarySearchTree<T>& other) const {
  if (root == nullptr && other.root == nullptr) {
      return true;
  }
  if (root == nullptr || other.root == nullptr) {
      return false;
  }
  return *root == *other.root;
}