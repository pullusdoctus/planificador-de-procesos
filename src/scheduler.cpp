#include <scheduler.h>
#include <ui.h>

Scheduler::Scheduler() : currentProcess(nullptr) {
	readyQueue = new SinglyLinkedList<Process>();
	blockedQueue = new SinglyLinkedList<Process>();
	finishedProcesses = new SinglyLinkedList<Process>();
	ioTimer = new Timer(15);
}

Scheduler::~Scheduler() {
	delete readyQueue;
	readyQueue = nullptr;
	delete blockedQueue;
	blockedQueue = nullptr;
	delete finishedProcesses;
	finishedProcesses = nullptr;
	delete currentProcess;
	currentProcess = nullptr;
	delete ioTimer;
	readyQueue = nullptr;
}

void Scheduler::run() {
	while (hasUnfinishedProcesses()) {
		checkBlockedProcesses();
		executeQuantum();
		schedule();
		displayStatus();
	}
}

void Scheduler::addProcess(Process* newProcess) {
	readyQueue->insertTail(*newProcess);
}

void Scheduler::removeProcess(Process* process) {
	switch(process->getState()) {
		case ProcessState::READY:
			readyQueue->deleteByValue(*process);
			break;
		case ProcessState::BLOCKED:
			blockedQueue->deleteByValue(*process);
			break;
		case ProcessState::FINISHED:
			finishedProcesses->deleteByValue(*process);
			break;
		default:
			break;
	}
}

void Scheduler::schedule() {
	selectNextProcess();
	
	if (currentProcess) {
		currentProcess->setState(ProcessState::RUNNING_ACTIVE);
	}
}

void Scheduler::executeQuantum() {
	if (!currentProcess) {
		return;
	}

	while (currentProcess->getInstructions()->getSize() >= currentProcess->getInstructionIndex()) {
		std::string	currentInstruction = currentProcess->getInstructions()->getAt(currentProcess->getInstructionIndex())->getData();
		double quantumCost = (currentInstruction == "e/s") ? 1.5 : 1;
		if (currentProcess->getState() == ProcessState::BLOCKED) {
			break;
		}
		if (currentProcess->getQuantum() < quantumCost) {
			currentProcess->executeNextInstruction();
			break;
		}
		UI::presentState(currentProcess);
		currentProcess->executeNextInstruction();
	}
	switch(currentProcess->getState()) {
		case ProcessState::FINISHED:
			moveToFinished(currentProcess);
			break;
		case ProcessState::BLOCKED:
			moveToBlocked(currentProcess);
			break;
		case ProcessState::RUNNING_PREEMPTED:
			moveToReady(currentProcess);
			break;
		default:
			break;
	}

	checkBlockedProcesses();
}

void Scheduler::preemptCurrentProcess() {
	if (currentProcess) {
		currentProcess->setState(ProcessState::RUNNING_PREEMPTED);
		moveToReady(currentProcess);
		currentProcess = nullptr;
	}
}

void Scheduler::checkBlockedProcesses() {
	if (blockedQueue->getHead()) {
		if (ioTimer->getStartTime().time_since_epoch() == std::chrono::steady_clock::duration::zero()) {
			ioTimer->start();
		}
		if (ioTimer->checkTime()) {
			Process process = blockedQueue->getTail()->getData();
			moveToReady(&process);
			clearTimer();
		}
	}
}

void Scheduler::handleProcessStateChange(Process* process, ProcessState newState) {
	process->setState(newState);
	
	switch(newState) {
		case ProcessState::READY:
			moveToReady(process);
			break;
		case ProcessState::BLOCKED:
			moveToBlocked(process);
			break;
		case ProcessState::FINISHED:
			moveToFinished(process);
			break;
		default:
			break;
	}
	
	if (process == currentProcess) {
		currentProcess = nullptr;
	}
}

void Scheduler::moveToReady(Process* process) {
	removeProcess(process);
	if (process->getState() == ProcessState::BLOCKED) {
		process->finishIO();
	}
	
	process->setState(ProcessState::READY);
	readyQueue->insertTail(*process);
}

void Scheduler::moveToBlocked(Process* process) {
	removeProcess(process);
	
	process->setState(ProcessState::BLOCKED);
	blockedQueue->insertTail(*process);
}

void Scheduler::moveToFinished(Process* process) {
	removeProcess(process);
	
	process->setState(ProcessState::FINISHED);
	finishedProcesses->insertTail(*process);
}

void Scheduler::clearTimer() {
	Timer* old = ioTimer;
	ioTimer = new Timer(15);
	delete old;
}


Process* Scheduler::getCurrent() {
	return currentProcess;
}

// === ROUND ROBIN ===

RoundRobin::RoundRobin() : Scheduler(), quantumSlice(5) {
	preemptedQueue = new SinglyLinkedList<Process>();
}

RoundRobin::~RoundRobin() {
	delete preemptedQueue;
	preemptedQueue = nullptr;
	delete readyQueue;
	readyQueue = nullptr;
	delete blockedQueue;
	blockedQueue = nullptr;
	delete finishedProcesses;
	finishedProcesses = nullptr;
	delete currentProcess;
	currentProcess = nullptr;
	delete ioTimer;
	ioTimer = nullptr;
}

bool RoundRobin::hasUnfinishedProcesses() {
	return readyQueue->getSize() > 0 || blockedQueue->getSize() > 0;
}

void RoundRobin::displayStatus() {
	std::cout << "Procesos listos: " << readyQueue->getSize() << std::endl;
	std::cout << "Procesos bloqueados: " << blockedQueue->getSize() << std::endl;
	std::cout << "Procesos terminados: " << finishedProcesses->getSize() << std::endl;
	std::cout << std::endl;
}

void RoundRobin::selectNextProcess() {
	if (currentProcess) {
		readyQueue->deleteByValue(*currentProcess);
		delete currentProcess;
		currentProcess = nullptr;
	}
	if (readyQueue->getSize() == 0) {
		if (blockedQueue->getSize() > 0) {
			Process blockedTail = blockedQueue->getTail()->getData();
			currentProcess = new Process(blockedTail);
			moveToReady(currentProcess);
			currentProcess->setQuantum(quantumSlice);
		}
		return;
	}

	Process process = readyQueue->getHead()->getData();
	currentProcess = new Process(process);
	currentProcess->setQuantum(quantumSlice);
}

// === PLANIFICACION POR PRIORIDAD ===

Priority::Priority() : Scheduler() {
	priorityQueue = new BinarySearchTree<Process>();
}

Priority::~Priority() {
	delete priorityQueue;
	priorityQueue = nullptr;
	delete readyQueue;
	readyQueue = nullptr;
	delete blockedQueue;
	blockedQueue = nullptr;
	delete finishedProcesses;
	finishedProcesses = nullptr;
	delete currentProcess;
	currentProcess = nullptr;
	delete ioTimer;
	ioTimer = nullptr;
}

void Priority::selectNextProcess() {
	if (currentProcess) {
		priorityQueue->remove(*currentProcess);
		delete currentProcess;
		currentProcess = nullptr;
	}
	if (priorityQueue->getSize() == 0) {
		if (blockedQueue->getSize() > 0) {
			Process blockedTail = blockedQueue->getTail()->getData();
			currentProcess = new Process(blockedTail);
			moveToReady(currentProcess);
		}
		return;
	}

	Process process = priorityQueue->getMax()->getData();
	currentProcess = new Process(process);
	currentProcess->setQuantum(1024);
}

bool Priority::hasUnfinishedProcesses() {
	return priorityQueue->getRoot() != nullptr || blockedQueue->getSize() > 0;
}

void Priority::displayStatus() {
	std::cout << "Procesos listos: " << priorityQueue->getSize() << std::endl;
	std::cout << "Procesos bloqueados: " << blockedQueue->getSize() << std::endl;
	std::cout << "Procesos terminados: " << finishedProcesses->getSize() << std::endl;
	std::cout << std::endl;
}

void Priority::calculateInitialPriority(Process& process) {
	int priority = process.getPriority();
	SinglyLinkedListNode<std::string>* current = process.getInstructions()->getHead();
	while (current) {
		if (current->getData() == "e/s") {
			priority = std::max(0, priority - 1);
		} else {
			priority = std::min(10, priority + 1);
		}
		current = current->getNext();
	}
	process.setPriority(priority);
}

void Priority::addProcess(Process* newProcess) {
	calculateInitialPriority(*newProcess);
	priorityQueue->insert(*newProcess);
}

void Priority::adjustProcessPriority(Process& process) {
	int temp = process.getPriority();
	SinglyLinkedListNode<std::string>* current = process.getInstructions()->getAt(process.getInstructionIndex());
	while (current) {
		if (current->getData() == "e/s") {
			temp = std::max(0, temp -1);
		} else {
			temp = std::min(10, temp +1);
		} 
		current = current->getNext();
	}
	process.setPriority(temp);
	
	priorityQueue->insert(process);
}

void Priority::moveToReady(Process* process) {
	removeProcess(process);
	if (process->getState() == ProcessState::BLOCKED) {
		process->finishIO();
	}
	
	process->setState(ProcessState::READY);
	adjustProcessPriority(*process);
}