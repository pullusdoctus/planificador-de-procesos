#include <singlyLinkedList.hxx>

template <typename T> 
SinglyLinkedListNode<T>::SinglyLinkedListNode() : data(), next(nullptr) {};

template <typename T> 
SinglyLinkedListNode<T>::SinglyLinkedListNode(T newData) : data(newData), next(nullptr) {};

template <typename T>
SinglyLinkedListNode<T>::SinglyLinkedListNode(const SinglyLinkedListNode& other) : data(other.data), next(other.next ? new SinglyLinkedListNode(*other.next) : nullptr) {}

template <typename T>
T SinglyLinkedListNode<T>::getData() {
  return data;
}

template <typename T>
T* SinglyLinkedListNode<T>::getPData() {
	return &data;
}

template<typename T>
SinglyLinkedListNode<T>* SinglyLinkedListNode<T>::getNext() {
	return next;
}

template <typename T>
SinglyLinkedList<T>::SinglyLinkedList() : head (nullptr), size(0) {};

template <typename T>
SinglyLinkedList<T>::SinglyLinkedList(const SinglyLinkedList<T>& other) : head(nullptr), size(0) {
  if (other.head == nullptr) {
    return;
  }
  head = new SinglyLinkedListNode<T>(other.head->data);
  size = 1;
  SinglyLinkedListNode<T>* current = head;
  SinglyLinkedListNode<T>* otherCurrent = other.head->next;
  while (otherCurrent != nullptr) {
    current->next = new SinglyLinkedListNode<T>(otherCurrent->data);
    current = current->next;
    otherCurrent = otherCurrent->next;
    size++;
  }
}

template <typename T>
SinglyLinkedList<T>& SinglyLinkedList<T>::operator=(const SinglyLinkedList<T>& other) {
  if (this == &other) {
    return *this;
  }
  clear();
  if (other.head == nullptr) {
    return *this;
  }
  head = new SinglyLinkedListNode<T>(other.head->data);
  size = 1;
  SinglyLinkedListNode<T>* current = head;
  SinglyLinkedListNode<T>* otherCurrent = other.head->next;
  while (otherCurrent != nullptr) {
    current->next = new SinglyLinkedListNode<T>(otherCurrent->data);
    current = current->next;
    otherCurrent = otherCurrent->next;
    size++;
  }
  return *this;
}

template <typename T>
SinglyLinkedListNode<T>::SinglyLinkedListNode(SinglyLinkedListNode&& other) : data(other.data), next(other.next) {
	other.next = nullptr;
}

template <typename T>
SinglyLinkedListNode<T>& SinglyLinkedListNode<T>::operator=(SinglyLinkedListNode&& other) {
	if (this != other) {
		data = other.data;
		delete next;
		next = other.next;
		other.next = nullptr;
	}
	return *this;
}

template <typename T>
SinglyLinkedList<T>::SinglyLinkedList(SinglyLinkedList&& other) : head(other.head), size(other.size) {
	other.head = nullptr;
	other.size = 0;
}

template <typename T>
SinglyLinkedList<T>&& SinglyLinkedList<T>::operator=(SinglyLinkedList&& other) {
	if (this != other) {
		clear();
		head = other.head;
		size = other.size;
		other.head = nullptr;
		other.size = 0;
	}
	return *this;
}

template <typename T>
SinglyLinkedList<T>::~SinglyLinkedList() {
	if (head) {
  	clear();
	}
}

template <typename T>
void SinglyLinkedList<T>::insertHead(const T& data) {
  SinglyLinkedListNode<T>* newNode = new SinglyLinkedListNode<T>(data);
  newNode->next = head;
  head = newNode;
  size++;
}

template <typename T>
void SinglyLinkedList<T>::insertTail(const T& data) {
  SinglyLinkedListNode<T>* newNode = new SinglyLinkedListNode<T>(data);
  if (head == nullptr) {
    head = newNode;
    size++;
    return;
  }
  SinglyLinkedListNode<T>* current = head;
  while (current->next != nullptr) {
    current = current->next;
  }
  current->next = newNode;
  size++;
}

template <typename T>
void SinglyLinkedList<T>::insertAfter(const T& target, const T& data) {
  if (head == nullptr) {
    return;
  }
  SinglyLinkedListNode<T>* current = head;
  while (current != nullptr && current->data != target) {
    current = current->next;
  }
  if (current == nullptr) {
    return;
  }
  SinglyLinkedListNode<T>* newNode = new SinglyLinkedListNode<T>(data);
  newNode->next = current->next;
  current->next = newNode;
  size++;
}

template <typename T>
void SinglyLinkedList<T>::insertBefore(const T& target, const T& data) {
  if (head == nullptr) {
    return;
  }
  if (head->data == target) {
    insertHead(data);
    return;
  }
  SinglyLinkedListNode<T>* current = head;
  while (current != nullptr && current->next->data != target) {
    current = current->next;
  }
  if (current == nullptr) {
    return;
  }
  SinglyLinkedListNode<T>* newNode = new SinglyLinkedListNode<T>(data);
  newNode->next = current->next;
  current->next = newNode;
  size++;
}

template <typename T>
void SinglyLinkedList<T>::insertAt(int position, const T& data) {
  if (position <= 0 || position > size) {
    return;
  }
  if (position == 1 || head == nullptr) {
    insertHead(data);
    return;
  }
  SinglyLinkedListNode<T>* current = head;
  SinglyLinkedListNode<T>* previous;
  for (int i = 0; i < position - 1 && current != nullptr; i++) {
    previous = current;
    current = current->next;
  }
  if (current == nullptr) {
    return;
  }
  SinglyLinkedListNode<T>* newNode = new SinglyLinkedListNode<T>(data);
  previous->next = newNode;
  newNode->next = current;
  size++;
}

template <typename T>    
void SinglyLinkedList<T>::insertOrdered(const T& data) {
  if (head == nullptr || data < head->data) {
    insertHead(data);
    return;
  }
  SinglyLinkedListNode<T>* current = head;
  SinglyLinkedListNode<T>* previous;
  while (current->next != nullptr && data > current->data) {
    previous = current;
    current = current->next;
  }
  if (current->next == nullptr) {
    insertTail(data);
    return;
  }
  SinglyLinkedListNode<T>* newNode = new SinglyLinkedListNode<T>(data);
  previous->next = newNode;
  newNode->next = current;
  size++;
}

template <typename T>
void SinglyLinkedList<T>::deleteHead() {
  if (head == nullptr) {
    return;
  }
  SinglyLinkedListNode<T>* oldHead = head;
  head = head->next;
	oldHead->next = nullptr;
  delete oldHead;
  size--;
}

template <typename T>
void SinglyLinkedList<T>::deleteTail() {
  if (head == nullptr) {
    return;
  }
  if (head->next == nullptr) {
    deleteHead();
    return;
  }
  SinglyLinkedListNode<T>* current = head;
  while (current->next->next != nullptr) {
    current = current->next;
  }
  delete current->next;
  current->next = nullptr;
  size--;
}
    
template <typename T>
void SinglyLinkedList<T>::deleteAfter(const T& target) {
  if (head == nullptr) {
    return;
  }
  SinglyLinkedListNode<T>* current = head;
  while (current != nullptr && current->data != target) {
    current = current->next;
  }
  if (current == nullptr || current->next == nullptr) {
    return;
  }
  SinglyLinkedListNode<T>* targetNode = current->next;
  current->next = current->next->next;
  delete targetNode;
  size--;
}

template <typename T>
void SinglyLinkedList<T>::deleteBefore(const T& target) {
  if (head == nullptr || head->data == target) {
    return;
  }
  SinglyLinkedListNode<T>* current = head;
  while (current != nullptr && current->next->next->data != target) {
    current = current->next;
  }
  SinglyLinkedListNode<T>* targetNode = current->next;
  current->next = current->next->next;
  delete targetNode;
  size--;
}

template <typename T>
void SinglyLinkedList<T>::deleteAt(int position) {
  if (position <= 0 || position > size || head == nullptr) {
    return;
  }
  if (position == 1) {
    deleteHead();
    return;
  }
  SinglyLinkedListNode<T>* current = head;
  SinglyLinkedListNode<T>* previous;
  for (int i = 0; i < position - 1 && current != nullptr; i++) {
    previous = current;
    current = current->next;
  }
  if (current == nullptr) {
    return;
  }
  previous->next = current->next;
  delete current;
  size--;
}

template <typename T>
void SinglyLinkedList<T>::deleteByValue(const T& target) {
  if (head == nullptr) {
    return;
  }
  if (head->data == target) {
    deleteHead();
    return;
  }
  SinglyLinkedListNode<T>* current = head;
  while (current->next != nullptr && !(current->next->data == target)) {
    current = current->next;
  }
  if (current->next == nullptr) {
    return;
  }
  SinglyLinkedListNode<T>* targetNode = current->next;
  current->next = targetNode->next;
  delete targetNode;
  size--;
}

template <typename T>
SinglyLinkedListNode<T>* SinglyLinkedList<T>::getHead() {
  return head;
}

template <typename T>
SinglyLinkedListNode<T>* SinglyLinkedList<T>::getTail() {
  SinglyLinkedListNode<T>* current = head;
  while (current->next) {
    current = current->next;
  }
  return current;
}

template <typename T>
SinglyLinkedListNode<T>* SinglyLinkedList<T>::getAt(const int position) {
  if (position < 1) {
    return nullptr;
  }
  if (!head) {
    return nullptr;
  }
  SinglyLinkedListNode<T>* current = head;
  for (int i = 1; current && i < position; i++) {
    current = current->next;
  }
  return current;
}

template <typename T>
int SinglyLinkedList<T>::getSize() {
  return size;
}

template <typename T>
SinglyLinkedListNode<T>* SinglyLinkedList<T>::search(const T& target) const {
  if (head == nullptr) {
    return nullptr;
  }
  SinglyLinkedListNode<T>* current = head;
  while (current != nullptr && current->data != target) {
    current = current->next;
  }
  return current;
}

template <typename T>
void SinglyLinkedList<T>::sort() {
  if (head == nullptr || head->next == nullptr) {
    return;
  }
  bool swapMade;
  SinglyLinkedListNode<T>* current;
  SinglyLinkedListNode<T>* last = nullptr;
  do {
    swapMade = false;
    current = head;
    while (current && current->next != last) {
      if (current->data > current->next->data) {
        swapData(current, current->next);
        swapMade = true;
      }
      current = current->next;
    }
    last = current;
  } while (swapMade);
}

template <typename T>
void SinglyLinkedList<T>::print() const {
  if (head == nullptr) {
    std::cout << "List is empty." << std::endl;
    std::cout << std::endl;
    return;
  }
  std::cout << "List size: " << size << std::endl;
  SinglyLinkedListNode<T>* current = head;
  int position = 1;
  while (current != nullptr) {
    std::cout << "Node " << position << ": " << current->data << std::endl;
    current = current->next;
    position++;
  }
  std::cout << std::endl;
}

template <typename T>
void SinglyLinkedList<T>::clear() {
  while (head != nullptr) {
    deleteHead();
  }
  size = 0; 
	head = nullptr;
}

template <typename T>
void SinglyLinkedList<T>::swapData(SinglyLinkedListNode<T>*& a, SinglyLinkedListNode<T>*& b) {
  T temp = a->data;
  a->data = b->data;
  b->data = temp;
}

template <typename T>
bool SinglyLinkedList<T>::operator==(const SinglyLinkedList<T>& other) const {
  SinglyLinkedListNode<T>* current = head, *otherCurrent = other.head;
  while (current && otherCurrent) {
    if (current->data != otherCurrent->data) {
      return false;
    }
    current = current->next;
    otherCurrent = otherCurrent->next;
  }
  return current == nullptr && otherCurrent == nullptr;
}