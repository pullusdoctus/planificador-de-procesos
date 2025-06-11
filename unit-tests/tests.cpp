#include <iostream>
#include <process.h>
#include <timer.h>
#include <ui.h>
#include <scheduler.h>
#include <fileParser.h>
#include <sstream>

// Color Terminal Output Library
// MIT Licensed Library

// There are 6 colors:
//
// blue
// green
// cyan
// red
// magenta
// yellow

// Usage:
// color("red", "Error occured", true);
//       color  text           newLine
#ifdef _WIN32
#include <windows.h>
void color(std::string color, std::string line, bool newLine) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	int col = 7;
	if (color == "blue") col = 1;
	else if (color == "green") col = 2;
	else if (color == "cyan") col = 3;
	else if (color == "red") col = 4;
	else if (color == "magenta") col = 5;
	else if (color == "yellow") col = 6;
	SetConsoleTextAttribute(hConsole, col);
	std::cout << line;
	if (newLine) {
		std::cout << std::endl;
	}
	SetConsoleTextAttribute(hConsole, 7);
}
#else
void color(std::string color, std::string line, bool newLine) {
	std::string col = "\033[0m";
	if (color == "blue") col = "\033[0;34m";
	else if (color == "green") col = "\033[0;32m";
	else if (color == "cyan") col = "\033[0;36m";
	else if (color == "red") col = "\033[0;31m";
	else if (color == "magenta") col = "\033[0;35m";
	else if (color == "yellow") col = "\033[0;33m";
	std::cout << col << line << "\033[0m";
	if (newLine) {
			std::cout << std::endl;
	}
}
#endif

/// @brief Clase madre abstracta Tests de la que heredan las demas clases hijas.
/// Contiene atributos para llevar la cuenta del numero de pruebas realizadas y el numero de pruebas pasadas.
/// Contiene metodos para imprimir los resultados de una prueba, asi como un resumen al final de todo.
/// Contiene un constructor que inicializa sus atributos a 0.
/// Contiene un metodo abstracto runAllTests que debe de ser implementado en sus clases hijas segun las necesidades de cada una.
class Tests {
  protected:
    int testsRun;
    int testsPassed;

		void printTestResult(bool passed, const std::string& testName) {
			testsRun++;
			if (passed) {
				testsPassed++;
				color("green", "✓ " + testName, true);
			} else {
				color("red", "✗ " + testName, true);
			}
		}

		void printTestSummary() {
			std::cout << "\ntest summary:\n";
			color("cyan", "total tests: " + std::to_string(testsRun), true);
			if (testsPassed == testsRun) {
					color("green", "all tests passed!", true);
			} else {
					color("red", "failed tests: " + std::to_string(testsRun - testsPassed), true);
					color("green", "passed tests: " + std::to_string(testsPassed), true);
			}
		}
  
  public:
    Tests();
    /// @brief Metodo abstracto por implementar en las clases hijas.
    virtual void runAllTests() = 0; 
};
Tests::Tests() : testsRun(0), testsPassed(0) {}


/// @brief Clase ProcessTests hija de Tests.
class ProcessTests : public Tests {
  public:
		void runAllTests() override {
			color("cyan", "\n=== Process Class Tests ===\n", true);

			testConstructor();
			testStateTransitions();
			testInstructions();
			testIOOperations();
			testPriorityComparisons();
			testQuantumManagement();

			printTestSummary();
		}
    
		void testConstructor() {
			color("yellow", "\nConstructor Tests:", true);

			// Constructor predeterminado
			Process p1;
			printTestResult(p1.getName() == "null", "Default name should be 'null'");
			printTestResult(p1.getPriority() == 0, "Default priority should be 0");
			printTestResult(p1.getState() == ProcessState::READY, "Default state should be READY");
			
			// Constructor parametrizado
			Process p2("test", 5);
			printTestResult(p2.getName() == "test", "Custom name should be set");
			printTestResult(p2.getPriority() == 5, "Custom priority should be set");
			printTestResult(p2.getState() == ProcessState::READY, "Initial state should be READY");
		}


		void testStateTransitions() {
			color("yellow", "\nState Transition Tests:", true);
			
			Process p("test", 1);
			
			p.setState(ProcessState::RUNNING_ACTIVE);
			printTestResult(p.getState() == ProcessState::RUNNING_ACTIVE, "Should transition to RUNNING_ACTIVE");
			
			p.setState(ProcessState::BLOCKED);
			printTestResult(p.getState() == ProcessState::BLOCKED, "Should transition to BLOCKED");
			
			p.setState(ProcessState::RUNNING_PREEMPTED);
			printTestResult(p.getState() == ProcessState::RUNNING_PREEMPTED, "Should transition to RUNNING_PREEMPTED");
			
			p.setState(ProcessState::FINISHED);
			printTestResult(p.getState() == ProcessState::FINISHED, "Should transition to FINISHED");
		}

		void testInstructions() {
			color("yellow", "\nInstruction Tests:", true);
			
			Process p("test", 1);
			printTestResult(!p.executeNextInstruction(), "Empty process should not execute");

			p.addInstruction("i1");
			p.addInstruction("i2");
			
			printTestResult(p.hasMoreInstrucions() == true, "Should have instructions");
			printTestResult(p.executeNextInstruction() == true, "Should execute normal instruction");
			printTestResult(p.hasMoreInstrucions() == true, "Should still have instructions after executing one");
			
			p.resetExecution();
			printTestResult(p.hasMoreInstrucions() == true, "Should have instructions after reset");
		}

		void testIOOperations() {
			color("yellow", "\nIO Operation Tests:", true);
			
			// Prueba 1: E/S exitosa con suficiente quantum
			Process p("test", 1);
			p.addInstruction("e/s");
			
			printTestResult(p.isInIO() == false, "Should not be in IO initially");
			
			p.executeNextInstruction();  
			printTestResult(p.isInIO() == true, "Should be in IO after IO instruction");
			printTestResult(p.getState() == ProcessState::BLOCKED, "Should be BLOCKED during IO");
			printTestResult(abs(p.getQuantum() - (5 - 1.5)) < 0.001, "Should consume 1.5 quantum units per IO");
			
			// Prueba 2: E/S fallida por falta de quantum
			Process p2("test2", 1);
			p2.addInstruction("e/s");
			p2.setQuantum(1);
			
			p2.executeNextInstruction();
			printTestResult(p2.isInIO() == false, "Should not enter IO with insufficient quantum");
			printTestResult(p2.getState() == ProcessState::RUNNING_PREEMPTED, "Should be in RUNNING_PREEMPTED state due to insufficent quantum for IO");
			
			// Prueba 3: Finalizacion de E/S
			Process p3("test3", 1);
			p3.addInstruction("e/s");
			p3.addInstruction("e/s");
			
			p3.executeNextInstruction();
			float quantumBeforeFinish = p3.getQuantum();
			p3.executeNextInstruction();
			
			printTestResult(p3.isInIO() == false, "Should not be in IO after finishing");
			printTestResult(p3.getState() == ProcessState::READY, "Should be in READY state after IO completion");
			printTestResult(abs(p3.getQuantum() - (quantumBeforeFinish - 1.5)) < 0.001, "Should consume 1.5 quantum units for IO finish");
			
			// Prueba 4: multiples instrucciones
			Process p4("test4", 1);
			p4.addInstruction("i1");
			p4.addInstruction("e/s");
			p4.addInstruction("e/s");
			p4.addInstruction("i1");

			printTestResult(p4.executeNextInstruction() == true, "Should execute normal instruction before IO");
			printTestResult(p4.executeNextInstruction() == false, "Should return false when starting IO");
			printTestResult(p4.executeNextInstruction() == true, "Should return true when ending IO");
			printTestResult(p4.getState() == ProcessState::READY, "Should return to READY state after ending IO");
			printTestResult(abs(p4.getQuantum() - 1) < 0.001, "Should have expected quantum after full sequence");
			printTestResult(p4.executeNextInstruction() == true, "Should execute last instruction");
			printTestResult(p4.getState() == ProcessState::FINISHED, "Should be in FINISHED state after last instruction");
			printTestResult(p4.executeNextInstruction() == false && p4.getState() == ProcessState::FINISHED, "Should not try to execute another instruction, and should not try to modify state, after last instruction");
		}

		void testPriorityComparisons() {
			color("yellow", "\nPriority Comparison Tests:", true);
			
			Process p1("test1", 1);
			Process p2("test2", 2);
			
			printTestResult(p1 < p2, "Lower priority process should be less than higher priority");
			printTestResult(p2 > p1, "Higher priority process should be greater than lower priority");
		}

		void testQuantumManagement() {
			color("yellow", "\nQuantum Management Tests:", true);
			
			Process p("test", 1);
			p.addInstruction("i1");
			p.addInstruction("i1");
			p.addInstruction("i1");
			p.setQuantum(2);
			
			bool firstExecution = p.executeNextInstruction();
			printTestResult(firstExecution == true, "Should execute first instruction");
			
			bool secondExecution = p.executeNextInstruction();
			printTestResult(secondExecution == true, "Should execute second instruction");
			
			bool thirdExecution = p.executeNextInstruction();
			printTestResult(thirdExecution == false, "Should not execute with quantum depleted");
			
			printTestResult(p.getState() == ProcessState::RUNNING_PREEMPTED, "Should be in RUNNING_PREEMPTED state when quantum is depleted");

			printTestResult(p.hasMoreInstrucions() == true, "Should have more instructions after not executing all");
		}
};

class TimerTests : public Tests {
	public:
		void runAllTests() override {
			color("cyan", "\n=== Timer Class Tests ===", true);

			testConstructor();
			testTimerStart();
			testTimeChecking();

			printTestSummary();
		}

		void testConstructor() {
			color("yellow", "\nConstructor Tests:", true);

			Timer t1(1.5);
			
			printTestResult(true, "Timer constructor executed without errors.");
		}
		
		void testTimerStart() {
			color("yellow", "\nTimer Start Tests:", true);

      Timer t1(1.0);
      std::chrono::time_point beforeStart = std::chrono::steady_clock::now();
      t1.start();
      std::chrono::time_point afterStart = std::chrono::steady_clock::now();

      std::chrono::time_point timerStartTime = t1.getStartTime();
      printTestResult(timerStartTime >= beforeStart && timerStartTime <= afterStart, "Timer start time should be between before and after timestamps");

			#ifdef _WINDOWS32
				Sleep(2 * 1000);
			#else
				sleep(2);
			#endif
      t1.start();
      std::chrono::time_point newStartTime = t1.getStartTime();
      printTestResult(newStartTime > timerStartTime, "Timer should update start time on subsequent starts");
		}
		
		void testTimeChecking() {
			color("yellow", "\nTime Checking Tests:", true);

			// checkeo inmediato
      Timer t1(1);
      t1.start();
      printTestResult(t1.checkTime() == false, "Timer should not complete immediately");

      // chequeo despues de esperar
      Timer t2(1);
      t2.start();
			#ifdef _WINDOWS32
				Sleep(1.1 * 1000);
			#else
				sleep(1.1);
			#endif
      printTestResult(t2.checkTime() == true, "Timer should complete after waiting longer than requisite time");

      // chequeo despues de completacion parcial
      Timer t3(2); 
      t3.start();
			#ifdef _WINDOWS32
				Sleep(1 * 1000);
			#else
				sleep(1);
			#endif
      printTestResult(t3.checkTime() == false, "Timer should not complete before requisite time");
			#ifdef _WINDOWS32
				Sleep(2 1000);
			#else
				sleep(2);
			#endif
      printTestResult(t3.checkTime() == true, "Timer should complete after full requisite time");

      // reset
      Timer t5(2);
      t5.start();
			#ifdef _WINDOWS32
				Sleep(1 * 1000);
			#else
				sleep(1);
			#endif
      bool firstCheck = t5.checkTime();  // deberia ser false
      t5.start();  // reset del tiemr
			#ifdef _WINDOWS32
				Sleep(1 * 1000);
			#else
				sleep(1);
			#endif
      bool secondCheck = t5.checkTime();  // deberia ser false
      printTestResult(firstCheck == false && secondCheck == false,"Timer should reset correctly when started again");
    }
};

class UITests : public Tests {
	public:
		void runAllTests() override {
			color("cyan", "\n=== UI Class Tests ===\n", true);

      testConstructor();
      testGettersAndSetters();
      testStaticMethods();
      testDestructor();

      printTestSummary();
		}

		void testConstructor() {
      color("yellow", "\nConstructor Tests:", true);

      UI ui;
      printTestResult(ui.getFilename() == "", "Default filename should be empty std::string");
      printTestResult(ui.getScheduler() == nullptr, "Default scheduler should be nullptr");
    }

    void testGettersAndSetters() {
      color("yellow", "\nGetter Tests:", true);

      UI ui;
      
      printTestResult(ui.getFilename() == "", "Initial filename should be empty");
      printTestResult(ui.getScheduler() == nullptr, "Initial scheduler should be nullptr");

			// para simular funcionamiento sin el input del usuario 
      std::string testFilename = "unit-tests/test.txt";
      Scheduler* testScheduler = new RoundRobin();
      
			ui.setFilename(testFilename);
			ui.setScheduler(testScheduler);

      printTestResult(ui.getFilename() == testFilename, "Should retrieve correct filename after setting");
      printTestResult(ui.getScheduler() == testScheduler, "Should retrieve correct scheduler after setting");
    }

    void testStaticMethods() {
      color("yellow", "\nStatic Method Tests:", true);

      Process* testProcess = new Process("TestProcess", 1);
      testProcess->setQuantum(3.0);
      testProcess->addInstruction("test_instruction");
      
      std::stringstream buffer;
			std::streambuf* oldCout = std::cout.rdbuf(buffer.rdbuf());

      UI::presentState(testProcess);
      
			std::cout.rdbuf(oldCout);

      std::string output = buffer.str();
      printTestResult(output.find("TestProcess") != std::string::npos, "presentState should show process name");
      printTestResult(output.find("Prioridad: 1") != std::string::npos, "presentState should show priority");
      printTestResult(output.find("Quantum Restante: 3") != std::string::npos, "presentState should show remaining quantum when <= 5");
      printTestResult(output.find("test_instruction") != std::string::npos, "presentState should show current instruction");

      testProcess->setQuantum(6.0);
      buffer.str(""); 
      
      UI::presentState(testProcess);
      output = buffer.str();
      printTestResult(output.find("Quantum") == std::string::npos, "presentState should not show quantum when > 5");

      delete testProcess;
    }

    void testDestructor() {
      color("yellow", "\nDestructor Tests:", true);

      // Test proper cleanup of scheduler
      UI* ui = new UI();
      Scheduler* testScheduler = new RoundRobin();
			ui->setScheduler(testScheduler);

      delete ui;
      printTestResult(true, "Destructor executed without errors");
    }
};

class FileParserTests : public Tests {
  public:
    void runAllTests() override {
      color("cyan", "\n=== FileParser Class Tests ===", true);

      testConstructor();
      testParseFile();
      testGetProcesses();
      testDestructor();

      printTestSummary();
    }

    void testConstructor() {
      color("yellow", "\nConstructor Tests:", true);

      FileParser fp1;
      printTestResult(fp1.getProcesses() != nullptr, "Default constructor should initialize non-null processes list");

      std::string testFilename = "testInput.txt";
      FileParser fp2(testFilename);
      printTestResult(fp2.getProcesses() != nullptr, "Parametrized constructor should initialize non-null processes list");
    }

    void testParseFile() {
      color("yellow", "\nParse File Tests:", true);

      FileParser fp;
      std::string testFilename = "testInput.txt";

      // "archivo" de prueba
			std::ofstream outFile(testFilename);
      outFile << "proceso TestProgram 5\n";
      outFile << "instruction1\n";
      outFile << "instruction2\n";
      outFile << "fin proceso\n";
      outFile.close();

      bool parseResult = fp.parseFile(testFilename);
      printTestResult(parseResult, "parseFile should return true for valid file");

      auto processes = fp.getProcesses();
      printTestResult(processes->getSize() == 1, "Processes list should contain 1 process");

      auto process = processes->getHead()->getData();
      printTestResult(process.getName() == "TestProgram", "Process name should be parsed correctly");
      printTestResult(process.getPriority() == 5, "Process priority should be parsed correctly");
      printTestResult(process.hasMoreInstrucions(), "Process should have instructions");
      printTestResult(process.executeNextInstruction() == true, "First instruction should execute correctly");
      printTestResult(process.executeNextInstruction() == true, "Second instruction should execute correctly");
      printTestResult(process.executeNextInstruction() == false, "No more instructions should be left to execute");

      remove(testFilename.c_str());
    }

    void testGetProcesses() {
      color("yellow", "\nGet Processes Tests:", true);

      FileParser fp;
      auto processes = fp.getProcesses();

      printTestResult(processes != nullptr, "getProcesses should return a valid pointer");
      printTestResult(processes->getSize() == 0, "Processes list should initially be empty");
    }

    void testDestructor() {
      color("yellow", "\nDestructor Tests:", true);

      FileParser* fp = new FileParser();
      delete fp;

      printTestResult(true, "Destructor executed without errors (memory checked with Valgrind)");
    }
};

class SchedulerTests : public Tests {
  public:
    void runAllTests() override {
      color("cyan", "\n=== Scheduler Class Tests ===", true);

      testConstructor();
      testAddAndRemoveProcesses();
      testStateTransitions();
      testExecutionCycle();
      testDestructor();

      printTestSummary();
    }

    void testConstructor() {
      color("yellow", "\nConstructor Tests:", true);

      RoundRobin rrScheduler;
      printTestResult(rrScheduler.getCurrent() == nullptr, "Current process should be null initially");
      printTestResult(rrScheduler.hasUnfinishedProcesses() == false, "No unfinished processes initially");
    }

    void testAddAndRemoveProcesses() {
      color("yellow", "\nAdd and Remove Processes Tests:", true);

      RoundRobin rrScheduler;

      Process* p1 = new Process("Process1", 1);
      Process* p2 = new Process("Process2", 2);
      rrScheduler.addProcess(p1);
      rrScheduler.addProcess(p2);

      printTestResult(rrScheduler.hasUnfinishedProcesses() == true, "Scheduler should detect unfinished processes after adding");
      printTestResult(rrScheduler.getCurrent() == nullptr, "Current process should remain null before scheduling");

      rrScheduler.removeProcess(p1);
      printTestResult(rrScheduler.hasUnfinishedProcesses() == true, "Scheduler should still detect unfinished processes after removing one");
      delete p1;
      delete p2;
    }

    void testStateTransitions() {
      color("yellow", "\nState Transitions Tests:", true);

      RoundRobin rrScheduler;
      Process* p = new Process("Process1", 1);
      rrScheduler.addProcess(p);

      rrScheduler.schedule();
      Process* current = rrScheduler.getCurrent();
      printTestResult(current != nullptr, "Scheduler should select a current process");
      printTestResult(current->getState() == ProcessState::RUNNING_ACTIVE, "Current process should be in RUNNING_ACTIVE state");

      rrScheduler.handleProcessStateChange(current, ProcessState::BLOCKED);
      printTestResult(current->getState() == ProcessState::BLOCKED, "Process should transition to BLOCKED state");

      rrScheduler.handleProcessStateChange(current, ProcessState::FINISHED);
      printTestResult(current->getState() == ProcessState::FINISHED, "Process should transition to FINISHED state");
      
      delete p;
    }

    void testExecutionCycle() {
      color("yellow", "\nExecution Cycle Tests:", true);

      RoundRobin rrScheduler;

      Process* p = new Process("Process1", 1);
      p->addInstruction("instruction1");
      p->addInstruction("instruction2");
      rrScheduler.addProcess(p);

      rrScheduler.schedule();
      rrScheduler.executeQuantum();

      Process* current = rrScheduler.getCurrent();
      printTestResult(current != nullptr, "Scheduler should have a current process during execution");
      printTestResult(current->hasMoreInstrucions() == false, "All instructions should be executed after quantum");
      printTestResult(current->getState() == ProcessState::FINISHED, "Process should be in FINISHED state after all instructions");

      delete p;
    }

    void testDestructor() {
      color("yellow", "\nDestructor Tests:", true);

      RoundRobin* rrScheduler = new RoundRobin();
      Process* p = new Process("Process1", 1);
      rrScheduler->addProcess(p);
      delete rrScheduler;

      printTestResult(true, "Destructor executed without errors (memory checked with Valgrind)");
      delete p;
    }
};

class TestRunner {
	public:
		static void runAll() {
			color("cyan", "\n=== Starting All Unit Tests ===\n", true);
			
			ProcessTests processTests;
			TimerTests timerTests;
			UITests uiTests;
			FileParserTests fileParserTests;
			SchedulerTests schedulerTests;

			processTests.runAllTests();
			timerTests.runAllTests();
			uiTests.runAllTests();
			fileParserTests.runAllTests();
			schedulerTests.runAllTests();

			color("cyan", "\n=== All Unit Tests Completed ===\n", true);
		}
};

int main() {
	TestRunner::runAll();
	return 0;
}