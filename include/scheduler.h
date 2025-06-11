#pragma once

#include <iostream>
#include <process.h>
#include <timer.h>
#include <binarySearchTree.hxx>

/// @brief Clase Scheduler para planificar la ejecucion de los proceso, asi como realizar la ejecucion
class Scheduler {
  protected:
    SinglyLinkedList<Process>* readyQueue;      
    SinglyLinkedList<Process>* blockedQueue;     
    SinglyLinkedList<Process>* finishedProcesses;
    
    Process* currentProcess;            
		Timer* ioTimer;

    /// @brief Metodo abstracto para ser modificado por clases hijas
    virtual void selectNextProcess() = 0;
    /// @brief Corta el proceso actual
    void preemptCurrentProcess();
    /// @brief Revisa si hay procesos bloqueados, y desbloquea alguno si ya paso el tiempo necesario
    void checkBlockedProcesses();

  public:
    /// @brief Constructor predeterminado
    Scheduler();
    /// @brief Destructor abstracto
    virtual ~Scheduler();
		
		/// @brief Procesa todos los procesos segun el algoritmo seleccionado
		void run();

    /// @brief Metodo virtual para añadir procesos
    /// @param newProcess El nuevo proceso que se añade
    virtual void addProcess(Process* newProcess);
    /// @brief Elimina un proceso de la cola en la que se encuentra
    /// @param process El proceso a eliminar
    void removeProcess(Process* process);

    /// @brief Selecciona el siguiente proceso en cola y lo marca como activo
    void schedule();
    /// @brief Ejecuta el proceso actual con el quantum proporcionado. Bloquea o corta los procesos cuando se cumplen los criterios.
    void executeQuantum();
    /// @brief Cambia el estado de un proceso y lo envia a la cola que le corresponde
    /// @param process El proceso cuyo estado ha de cambiar
    /// @param newState El estado al que el proceso cambia
    void handleProcessStateChange(Process* process, ProcessState newState);

    /// @brief Mueve un proceso a la cola READY
    /// @param process El proceso que se mueve
    virtual void moveToReady(Process* process);
    /// @brief Mueve un proceso a la cola BLOCKED
    /// @param process El proceso que se mueve
    void moveToBlocked(Process* process);
    /// @brief Mueve un proceso a la cola FINISHED
    /// @param process El proceso que se muev
    void moveToFinished(Process* process);

    /// @brief Metodo abstracto para ser modificado por las clases hijas
    /// @return true si hay procesos sin terminar; false si no
    virtual bool hasUnfinishedProcesses() = 0;
		/// @brief Reinicia el timer de entrada y salida
		void clearTimer();
    /// @brief Metodo abstracto para ser modificado por las clases hijas
    virtual void displayStatus() = 0;

		/// @brief Getter de currentProcess
		/// @return currentProcess
		Process* getCurrent();
};

/// @brief Clase hija RoundRobin de Scheduler
class RoundRobin : public Scheduler {
	private:
    SinglyLinkedList<Process>* preemptedQueue;     
		float quantumSlice;
		
	protected:
		/// @brief Metodo polimorfico. Selecciona el siguiente proceso en la cola READY (LIFO)
		void selectNextProcess() override;
	
	public:
		/// @brief Constructor predeterminado
		RoundRobin();
		/// @brief Destructor polimorfico
		~RoundRobin() override;

    /// @brief Metodo polimorfico. Revisa si hay procesos en la cola READY o en la cola BLOCKED
    /// @return true si quedan procesos en alguna de esas colas; false si no
    bool hasUnfinishedProcesses() override;
    /// @brief Metodo polimorfico. Muestra el numero de procesos en las colas READY, BLOCKED, y FINISHED
    void displayStatus() override;
};

/// @brief Clase hija Priority de Scheduler
class Priority : public Scheduler {
	private:
		BinarySearchTree<Process>* priorityQueue;

	protected:
		/// @brief Metodo polimorfico. Selecciona el proceso con la prioridad mas alta en la cola de prioridad
		void selectNextProcess() override;
	
	public:
		/// @brief Constructor predeterminado
		Priority();
		/// @brief Destructor polimorfico
		~Priority() override;
		
    /// @brief Metodo polimorfico. Revisa si hay procesos en la cola de prioridad o en la cola BLOCKED
    /// @return true si quedan procesos en alguna de esas colas; false si no
    bool hasUnfinishedProcesses() override;
    /// @brief Metodo polimorfico. Muestra el numero de procesos en la cola de prioridad, ademas de el numero de procesos en las colas BLOCKED y FINISHED
    void displayStatus() override;

		/// @brief Calcula la prioridad inicial de un proceso a partir de sus instrucciones antes de insertarlo al arbol de prioridad
		/// @param process El proceso cuya prioridad debe ser calculada
		void calculateInitialPriority(Process& process);
		/// @brief Metodo polimorfico. Añade un proceso a la cola de prioridad despues de calcular su prioridad inicial
		/// @param newProcess El nuevo proceso a añadir
		void addProcess(Process* newProcess) override;
		/// @brief Reajusta la prioridad de un proceso segun las instrucciones que le quedan
		/// @param process El proceso cuya prioridad debe ser reajustada
		void adjustProcessPriority(Process& process);
		/// @brief Mueve un proceso a la cola de prioridad despues de reajustar su prioridad
		/// @param process El proceso que debe volver a la cola de prioridad
		void moveToReady(Process* process) override;
};