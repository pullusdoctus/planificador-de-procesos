#include <ui.h>
#include <fileParser.h>

int main() {
	UI* ui = new UI();
	ui->run();

	Scheduler* scheduler = ui->getScheduler();
	std::string filename = ui->getFilename();
	FileParser* fileParser = new FileParser(filename);
	fileParser->parseFile(filename);
	
	SinglyLinkedListNode<Process>* current = fileParser->getProcesses()->getHead();
	while (current) {
		Process currentProcess = current->getData();
		Process* pProcess = &currentProcess;
		scheduler->addProcess(pProcess); 
		current = current->getNext();
	}
	scheduler->run();

	delete ui;
	delete fileParser;
  return 0;
}