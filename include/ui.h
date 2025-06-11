#pragma once

#include <iostream>
#include <string>
#include <scheduler.h>

/// @brief Clase UI para imprimir el progreso del programa.
class UI {
	private:
		std::string filename;
		Scheduler* scheduler;

	public: 
		/// @brief Constructor predeterminado
		UI();
		/// @brief Destructor
		~UI();
		
		/// @brief Setter para filename
		/// @param newFilename El nuevo filename
		void setFilename(const std::string& newFilename);
		/// @brief Setter para scheduler
		/// @param newScheduler El nuevo scheduler
		void setScheduler(Scheduler* newScheduler);
		/// @brief Getter de filename
		/// @return filename
		std::string getFilename();
		/// @brief Getter para scheduler
		/// @return scheduler
		Scheduler* getScheduler();

	  /// @brief Ejecuta los procesos de input
	  void run();
	  
	  /// @brief Le pide al usuario que escoja uno de los archivos
	  void askForFileName();
	  /// @brief Le pide al usuario que escoja uno de los algoritmos de planificacion
	  void askForScheduler();
	  /// @brief Muestra el estado del programa en la ejecucion de un proceso actual
	  /// @param currentProcess El proceso que se esta "ejecutando" actualmente
	  static void presentState(Process* currentProcess);
		/// @brief Limpia la consola
		static void clearCLI();
};