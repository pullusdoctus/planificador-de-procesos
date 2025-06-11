#include <fileParser.h>

FileParser::FileParser() : filename("null"){
  processes = new SinglyLinkedList<Process>();
  if (!processes) {
    throw std::runtime_error("Failed to allocate memory for process");
  }
}

FileParser::FileParser(const std::string inputFile) : filename(inputFile){
  std::ifstream archivo;
  processes = new SinglyLinkedList<Process>();
  if (!processes) {
    throw std::runtime_error("Failed to allocate memory for process");
  }
}

FileParser::~FileParser() {
  delete processes;
}

bool FileParser::parseFile(std::string filename) {
  std::ifstream archivo;
  archivo.open(filename, std::ios::in | std::ios::binary);

  if (!archivo.is_open()) {
    std::cerr << "Error al abrir el archivo" << std::endl;
    return false;
  } 

	std::string linea;
	char delimitador = ' ';
	Process* nuevoProceso = nullptr;

	while (getline(archivo, linea)) {  
		std::stringstream stream(linea);
		std::string instrucciones;
		getline(stream, instrucciones);

		std::string encontrar1 = "proceso";
		std::string encontrar2 = "fin proceso";
		size_t pos1 = instrucciones.find(encontrar1);
		size_t pos2 = instrucciones.find(encontrar2);

		// si llega al final del proceso
		if (pos2 != std::string::npos) {
			if (nuevoProceso) {
				processes->insertHead(*nuevoProceso);
				delete nuevoProceso;
				nuevoProceso = nullptr;
			}
		// si llega al encabezado
		} else if (pos1 != std::string::npos) {
			std::stringstream stream1(linea);
			std::string proceso, nombrePrograma, numPrioridad;
			getline(stream1, proceso, delimitador);
			getline(stream1, nombrePrograma, delimitador);
			getline(stream1, numPrioridad, delimitador);

			// Pasar nombrePrograma, int numPrioridad
			int priority = stoi(numPrioridad);
			nuevoProceso = new Process(nombrePrograma, priority);
		// instrucciones del proceso
		} else {
			// introducir instrucción a la lista del proceso correspondiente
			nuevoProceso->addInstruction(instrucciones);
		}
	}
	if (nuevoProceso) {
		delete nuevoProceso;	
	}

  archivo.close();
  return true;
}

SinglyLinkedList<Process>* FileParser::getProcesses() {
  return processes;
}