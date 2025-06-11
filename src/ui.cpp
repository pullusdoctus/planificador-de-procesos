#include <iostream>
#include <scheduler.h>
#include <ui.h>

UI::UI() : filename(""), scheduler(nullptr) {}

UI::~UI() {
	delete scheduler;
}

void UI::setFilename(const std::string& newFilename) {
	filename = newFilename;
}
void UI::setScheduler(Scheduler* newScheduler) {
	scheduler = newScheduler;
}

std::string UI::getFilename() {
	return filename;
}

Scheduler* UI::getScheduler() {
	return scheduler;
}

void UI::askForFileName() {
	int option;
	while (true) {
		std::cout << "Elija el archivo a utilizar: [1: enrique.txt], [2: jonathan.txt], [3: jose.txt], [4: manfred.txt], [5. pruebas.txt]" << std::endl;
		std::cin >> option;
		if (option==1) {
			filename = "database/enrique.txt";
			break;
		} else if (option == 2){
			filename = "database/jonathan.txt";
			break;
		} else if (option == 3){
			filename = "database/jose.txt";
			break;
		} else if (option == 4){
			filename = "database/manfred.txt";
			break;
		} else if (option == 5) {
			filename = "database/pruebas.txt";
			break;
		} else {
			std::cout << "Opción inválida, inténtelo de nuevo." << std::endl;
		}
	}
}

void UI::askForScheduler() {
	int option;
	while (true) {
		std::cout << "Elija el algoritmo de planificación a utilizar: [1: Round Robin], [2: Planificación por prioridad]" << std::endl; std::cin >> option;
		if (option==1) {
			scheduler = new RoundRobin();
			break;
		} else if (option == 2){
			scheduler = new Priority();
			break;
		} else {
			std::cout << "Opción inválida, inténtelo de nuevo." << std::endl;
		}
	}
}

//esto va antes de "schedule();" (line81 scheduler.cpp)
void UI::presentState(Process* currentProcess) {
	//clearCLI();
	std::cout << "Proceso: " << currentProcess->getName()
		<< ", Prioridad: " << currentProcess->getPriority() 
		<< ", Estado: " << currentProcess->getState();
	if (currentProcess->getQuantum() <= 5) {
		std::cout << ", Quantum Restante: " << currentProcess->getQuantum() ;
	}
	std::cout << "." << std::endl;
	if (currentProcess->getState() != ProcessState::BLOCKED || currentProcess->getState() != ProcessState::RUNNING_PREEMPTED) {
		std::cout << "Instrucción actual: " << currentProcess->getInstructions()->getAt(currentProcess->getInstructionIndex())->getData() << std::endl;
		std::cout << "Índice de la instrucción: " << currentProcess->getInstructionIndex() << std::endl;
	}
}

void UI::clearCLI() {
#ifdef _WIN32
	system("cls"); // Windows CLI clear 
#else
	system("clear"); // UNIX CLI clear
#endif
}

void UI::run() {
	askForFileName();
	askForScheduler();
}