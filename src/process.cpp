#include <process.h>

std::ostream& operator<<(std::ostream& os, const ProcessState& state) {
  switch (state) {
    case ProcessState::READY:
      os << "READY";
      break;
    case ProcessState::RUNNING_ACTIVE: 
      os << "RUNNING_ACTIVE"; 
      break;
    case ProcessState::RUNNING_PREEMPTED: 
      os << "RUNNING_PREEMPTED"; 
      break;
    case ProcessState::BLOCKED: 
      os << "BLOCKED"; 
      break;
    case ProcessState::FINISHED: 
      os << "FINISHED"; 
      break;
    default: 
      os << "UNKNOWN"; 
      break;
  }
  return os;
}

Process::Process() 
  : name("null"), priority(0), instructionIndex(1), remainingQuantum(5), IOPending(false) {
  instructions = new SinglyLinkedList<std::string>();
  if (!instructions) {
    throw std::runtime_error("Failed to allocate memory for instructions list");
  }
  state = ProcessState::READY;
}

Process::Process(const std::string newName, int newPriority) 
  : name(newName), priority(newPriority),
    instructionIndex(1), remainingQuantum(5), IOPending(false) {
  instructions = new SinglyLinkedList<std::string>();
  if (!instructions) {
    throw std::runtime_error("Failed to allocate memory for instructions list");
  }
  state = ProcessState::READY;
}

Process::Process(const Process& other) : name(other.name), priority(other.priority), 
	state(other.state), instructionIndex(other.instructionIndex), remainingQuantum(other.remainingQuantum), IOPending(other.IOPending) {
	instructions = new SinglyLinkedList<std::string>(*other.instructions);
}

Process& Process::operator=(const Process& other) {
	if (this != &other) {
		name = other.name;
		priority = other.priority;
		instructionIndex = other.instructionIndex;
		remainingQuantum = other.remainingQuantum;
		IOPending = other.IOPending;
		state = other.state;
		delete instructions;
		instructions = new SinglyLinkedList<std::string>(*other.instructions);
	}
	return *this;
}

Process::~Process() {
  delete instructions;
}

const std::string Process::getName() const {
  return name;
}

void Process::setName(const std::string newName) {
	name = newName;
}

int Process::getPriority() const {
  return priority;
}

void Process::setPriority(int newPriority) {
	priority = newPriority;
}

ProcessState Process::getState() const {
  return state;
}

void Process::setState(ProcessState newState) {
  state = newState;
}

int Process::getInstructionIndex() const {
	return instructionIndex;
}

float Process::getQuantum() const {
  return remainingQuantum;
}

void Process::setQuantum(float newQuantum) {
  remainingQuantum = newQuantum;
}

SinglyLinkedList<std::string>* Process::getInstructions() const {
	return instructions;
}

void Process::addInstruction(const std::string instruction) {
  instructions->insertTail(instruction);
}

bool Process::executeNextInstruction() {
  if (!instructions || state == ProcessState::FINISHED) {
    return false;
  }
  if (remainingQuantum <= 0) {
    state = ProcessState::RUNNING_PREEMPTED;
    return false;
  }
  if (this->hasMoreInstrucions()) {
    SinglyLinkedListNode<std::string>* node = instructions->getAt(instructionIndex);
    if (!node) {
      return false;
    }
    if (node->getData() == "e/s") {
      if (remainingQuantum >= 1.5) {
        if (IOPending) {
          finishIO();
          return true;
        }
        startIO();
      } else {
        state = ProcessState::RUNNING_PREEMPTED;
      }
      return false;
    }
    sleepInSeconds(1);
    remainingQuantum--;
    instructionIndex++;
    if (instructionIndex > instructions->getSize()) {
      state = ProcessState::FINISHED;
    }
    return true;
  }
  return false;
}

bool Process::hasMoreInstrucions() const {
  return instructionIndex <= instructions->getSize();
}
void Process::resetExecution() {
  instructionIndex = 1;
  remainingQuantum = 0;
  IOPending = false;
  state = ProcessState::READY;
}

bool Process::isInIO() const {
  return IOPending;
}

void Process::startIO() {
  IOPending = true;
  state = ProcessState::BLOCKED;
  sleepInSeconds(1.5);
  remainingQuantum -= 1.5;
  instructionIndex++;
}

void Process::finishIO() {
  IOPending = false;
  state = ProcessState::READY;
  sleepInSeconds(1.5);
  remainingQuantum -= 1.5;
  instructionIndex++;
}

bool Process::operator<(const Process& other) {
  return this->priority < other.priority;
}

bool Process::operator>(const Process& other) {
  return this->priority > other.priority;
} 

void Process::sleepInSeconds(float seconds) {
#ifdef _WINDOWS32
  Sleep(seconds * 1000);
#else
  sleep(seconds);
#endif
}

bool Process::operator<(const Process& other) const {
	if (this->priority == other.priority) {
		return true;
	}
	return this->priority < other.priority;
}

bool Process::operator>(const Process& other) const {
	return this->priority > other.priority;
}

bool Process::operator==(const Process& other) const {
	if (this == &other) {
		return true;
	}
	return this->priority == other.priority && this->name == other.name;
}

bool Process::operator!=(const Process& other) const {
	return !(*this == other);
}

bool Process::operator<=(const Process& other) const {
	return !(*this > other);
}

bool Process::operator>=(const Process& other) const {
	return !(*this < other);
}

std::ostream& operator<<(std::ostream& os, const Process& process) {
	os << "Process[" 
			<< "name: " << process.getName() 
			<< ", priority: " << process.getPriority()
			<< ", state: " << process.getState()
			<< ", instruction index: " << process.getInstructionIndex()
			<< ", quantum: " << process.getQuantum()
			<< ", IO pending: " << (process.isInIO() ? "true" : "false")
			<< ", instructions: {";
	
	SinglyLinkedList<std::string>* instructions = process.getInstructions();
	if (instructions) {
		for (int i = 1; i <= instructions->getSize(); i++) {
			if (i > 1) {
				os << ", ";
			}
			SinglyLinkedListNode<std::string>* node = instructions->getAt(i);
			if (node) {
				os << node->getData();
				}
		}
	}
	os << "}]";
	
	return os;
}