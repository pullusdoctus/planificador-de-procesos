#pragma once
#include <iostream>
#include <chrono>

/// @brief Clase que sirve de temporizador para verificar si el tiempo necesario para llamar el desbloqueo de un proceso por IO ya pasó.
/// Contiene: 1. el tiempo en el que se inició el temporizador y 2. el tiempo necesario, en segundos, que debe de haber pasado para llamar el desbloqueo.
class Timer {
	private:
		std::chrono::steady_clock::time_point IOStartTime;
		double IORequisite;
		
	public:
		/// @brief Constructor parametrizado de la clase Timer.
		/// @param newIORequisite El nuevo tiempo requerido para llamar el desbloqueo del proceso por IO. En segundos.
		Timer(double newIORequisite);
		
		/// @brief Inicia el temporizador.
		void start();
		/// @brief Revisa el tiempo que ha pasado desde que se inició el temporizador. 
		/// @return true si ha pasado el tiempo necesario o más para desbloquear; false si no.
		bool checkTime();
		
		/// @brief Getter de IOStartTime
		/// @return IOStartTime
		std::chrono::steady_clock::time_point getStartTime();
};