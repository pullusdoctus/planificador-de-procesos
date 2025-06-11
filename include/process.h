#pragma once

#include <iostream>
#include <string>
#ifdef _WINDOWS32
  #include <windows.h>
#else
  #include <unistd.h>
#endif
#include <singlyLinkedList.hxx>

/// @brief Enum para cambiar facilmente entre los distintos estados del proceso
enum class ProcessState {
  READY,
  RUNNING_ACTIVE,
  RUNNING_PREEMPTED,
  BLOCKED,
  FINISHED
};

/// @brief Sobrecarga del operador << para enviar un ProcessState a un stream de salida de manera que se pueda imprimir.
/// @param os El stream de salida al que se envia el ProcessState.
/// @param state El ProcessState que se envia.
/// @return El stream de salida.
std::ostream& operator<<(std::ostream& os, const ProcessState& state);

/// @brief Clase que representa un proceso.
/// Contiene: 1. nombre del proceso, 2. prioridad del proceso, 3. las instrucciones del proceso, 4. el estado del proceso,
/// 5. el indice de la instruccion en ejecucion, 6. el quantum que le queda al proceso, y 7. una bandera que indica si aun debe terminar una instruccion de E/S
class Process {
  private:
    std::string name;
    int priority;                     
    SinglyLinkedList<std::string>* instructions;
    ProcessState state;
    int instructionIndex;
    float remainingQuantum;          
    bool IOPending;
    
  public:
  /// @brief Constructor de Process.
  Process();
  /// @brief Constructor parametrizado de Process.
  /// @param newName El nombre del nuevo proceso.
  /// @param newPriority La prioridad del nuevo proceso.
  Process(const std::string newName, int newPriority);
	/// @brief Constructor de movimiento de Process.
	/// @param other El otro proceso a mover en este
	Process(const Process& other);
	/// @brief Constructor de asignacion movimiento 
	/// @param other El otro proceso a mover en este
	/// @return Este proceso con los datos movidos
	Process& operator=(const Process& other);
  /// @brief Destructor de Process.
  ~Process();
  
  /// @brief Getter del nombre del proceso.
  /// @return El nombre del proceso.
  const std::string getName() const;
	/// @brief Setter para name
	/// @param newName El nuevo name
	void setName(const std::string newName);
  /// @brief Getter de la prioridad del proceso.
  /// @return La prioridad del proceso.
  int getPriority() const;
	/// @brief Setter para priority
	/// @param newPriority El nuevo priority
	void setPriority(int newPriority);
  /// @brief Getter del estado del proceso.
  /// @return El estado del proceso.
  ProcessState getState() const;
  /// @brief Setter del estado del proceso.
  /// @param newState El nuevo estado al que pasa el proceso.
  void setState(ProcessState newState);
	/// @brief Getter para instructionIndex
	/// @return InstructionIndex
	int getInstructionIndex() const;
  /// @brief Getter del quantum restante.
  /// @return El quantum restante.
  float getQuantum() const;
  /// @brief Setter para el quantum del proceso.
  /// @param quantum El nuevo quantum del proceso.
  void setQuantum(float newQuantum);
	/// @brief 
	/// @return 
	SinglyLinkedList<std::string>* getInstructions() const;


  /// @brief Añade una nueva instruccion al proceso.
  /// @param instruction La nueva instruccion que se va a añadir.
  void addInstruction(const std::string instruction);
  /// @brief Simula la ejecucion de la siguiente instruccion en la lista.
  /// @return True si logra ejecutarla; false si el proceso se corta (quantum insuficiente) o se bloquea (E/S).
  bool executeNextInstruction();
  /// @brief Revisa si quedan instrucciones en el proceso.
  /// @return True si sí; false si no.
  bool hasMoreInstrucions() const;
  /// @brief Reinicia la ejecucion del proceso al principio.
  void resetExecution();

  /// @brief Revisa si el proceso esta bloqueado (en E/S).
  /// @return True si sí; false si no.
  bool isInIO() const;
  /// @brief Bloquea el proceso para simular que esta en E/S.
  void startIO();
  /// @brief Desbloquea el proceso para simular que sale de E/S.
  void finishIO();
  
  /// @brief Sobrecarga del operador <. Realiza la comparacion mediante la prioridad del proceso.
  /// @param other El otro proceso con el que se compara.
  /// @return True si este tiene menor prioridad que el otro; false si no.
  bool operator<(const Process& other);
  /// @brief Sobrecarga del operador >. Realiza la comparacion mediante la prioridad del proceso.
  /// @param other El otro proceso con el que se compara.
  /// @return True si este tiene mayor prioridad que el otro; false si no.
  bool operator>(const Process& other);
  
  /// @brief Metodo multiplataforma para pausar el programa durante una cierta cantidad de segundos.
  /// @param seconds Los segundos por los que el programa debe de pausarse.
  void sleepInSeconds(float seconds);
	
	// Sobrecarga de operadores de comparacion para comparar procesos usando su prioridad como criterio
	bool operator<(const Process& other) const;
	bool operator>(const Process& other) const;
	// == en particular usa la prioridad y el nombre del proceso
	bool operator==(const Process& other) const;
	bool operator!=(const Process& other) const;
	bool operator<=(const Process& other) const;
	bool operator>=(const Process& other) const;
	friend std::ostream& operator<<(std::ostream& os, const Process& process);
};