#pragma once

#include <fstream>
#include <iostream>
#include <sstream>
#include <singlyLinkedList.hxx>
#include <process.h>

/// @brief Clase que lee e interpreta procesos de un archivo de texto
class FileParser {
  private:
    /// @brief El nombre del archivo
    std::string filename;
    /// @brief La lista de los procesos interpretada
    SinglyLinkedList<Process>* processes;

  public:
    /// @brief Constructor predeterminado
    FileParser();
    /// @brief Constructor parametrizado
    /// @param inputFile El nombre del archivo por abrir
    FileParser(const std::string inputFile);
    /// @brief Destructor
    ~FileParser();

    /// @brief Lee el archivo indicado y almacena sus procesos. Guarda el nombre del archivo especificado como su nuevo nombre.
    /// @param filename El nombre del archivo por leer.
    /// @return true si logro hacer todo bien; false si tuvo algun error
    bool parseFile(const std::string filename);
    /// @brief Getter de la lista de procesos
    /// @return La lista de procesos
    SinglyLinkedList<Process>* getProcesses();
};