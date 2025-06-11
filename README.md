# PROYECTO 2: Planificador de Procesos
- El objetivo de este proyecto es el de crear un planificador de procesos que lee definiciones de procesos de un archivo de texto y simula su ejecucion mediante dos algoritmos de planificacion: Round Robin y Planificacion por prioridad. Cada proceso tiene instrucciones que duran cierto tiempo en ejecutarse; entre ellas instruciones 'normales', y operaciones de IO.

- Todo esto por medio de la utilización de código basado en la Programación Orientada a Objetos, así como el uso de códigos reutilizables, genéricos, claros y precisos para la ejecución de los algoritmos,

## PARTICIPANTES
- Jeremy Andrey Rojas Cerna, B96804.
- Ángel Gabriel Garos Dávila, C33186.
- Alexander Wang Wu, C28559.

## Comandos de compilación para terminal
* make: Compila el código.
* make run: Compila el código si es necesario lo y ejecuta.
* make clean: Elimina los archivos binarios necesarios para compilación, entre ellos los ejecutables.
* make test: Compila y ejecuta las pruebas unitarias.
* Si se utilizara Windows se requiere instalar MinGW, que suele traer mingw32-make como comando, en vez de make.
* Para evitar problemas de compatibilidad, se recomienda encarecidamente instalar Windows Subsystem for Linux.
### Pasos de instalación
1. En una consola de Windows, ejecutar: wsl --install
2. Reiniciar el sistema
3. Crear un usuario y una contraseña cuando WSL lo pida
4. Instalar el software necesario dentro de WSL, en este orden:
  - sudo apt update
  - sudo apt upgrade
  - sudo apt install gcc
  - sudo apt install g++
  - sudo apt install gdb
  - sudo apt install make
5. Reiniciar el sistema después de instalar lo anterior  
Para abrir WSL, simplemente se ejecuta el comando 'wsl' dentro de cualquier consola de Windows. Si se ejecuta en un fólder que no es la raíz, WSL se abre en ese fólder.

## REQUISITOS DEL PROYECTO 
1. Input: archivo de texto con definiciones de los procesos (nombre, prioridad, instrucciones).
2. Estado de los procesos: listo, ejecutandose (activo, preparado), bloqueado, terminado.
3. Algoritmos de planificacion: Round Robin y por prioridad.
4. Unidades de tiempo: CPU quantum (5 ciclos; 5 segundos), instruccion normal (1 ciclo; 1 segundo), operacion IO (1.5 ciclos p/ operacion; 3 segundos en total).

## RESTRICCIONES DEL PROYECTO 
1. Usar programacion a objetos.
2. No usar contenedores STL (list, vector, map).
3. No usar bibliotecas externas.
4. Usar la estructura de folderes vista en clase.
5. Grupo maximo: 3.
6. No usar IA.

## PLAN DE IMPLEMENTACION 

### I. Estructura de clases 

```
enum class ProcessState {
  READY,
  RUNNING_ACTIVE,
  RUNNING_PREEMPTED,
  BLOCKED,
  FINISHED
};

class Process {
  private:
    string name;
    int priority;                     // prioridad del proceso (0-10)
    SinglyLinkedList* instructions;
    ProcessState state;
    int instructionIndex;
    int remainingQuantum;             // tiempo restante en quantum
    bool IOPending;
    
  public:
  // constructor y destructor
  Process();
  Process(const string newName, int newPriority);
  ~Process();
  
  // getters y setters
  const string getName() const;
  int getPriority() const;
  ProcessState getState() const;
  void setState(ProcessState newState);

  // manejo de procesos
  void addInstruction(const string instruction);
  bool executeNextInstruction();
  bool hasMoreInstrucions() const;
  void resetExecution();

  // IO
  bool isInIO() const;
  void startIO();
  void finishIO();
};

class Scheduler {
  private:
    SinglyLinkedList* readyQueue;         // procesos listos para ejecutarse
    SinglyLinkedList* blockedQueue;       // procesos bloqueados*
    SinglyLinkedList* preemptedQueue;     // procesos cortados
    SinglyLinkedList* finishedProcesses;
    BinarySearchTree* priorityQueue;      // para algoritmos de planificacion
    
    int quantum;                          // tiempo asignado a cada proceso
    bool isRoundRobin;                    // para determinar cual algoritmo de planificacion
    Process* currentProcess;            

    // helpers
    Process* selectNextProcess();
    void preemptCurrentProcess();
    void checkBlockedProcesses();

  public:
    // constructor y destructor
    Scheduler();
    Scheduler(int newQuantum, bool newRoundRobin);
    ~Scheduler();

    // manejo de procesos
    void addProcess(Process* newProcess);
    void removeProcess(Process* process);

    // operaciones de planificacion
    void schedule();
    void executeQuantum();
    void handleProcessStateChange(Process* process, ProcessState newState);

    // manejo de colas
    void moveToReady(Process* process);
    void moveToBlocked(Process* process);
    void moveToFinished(Process* process);

    // metodos utiles
    bool hasUnfinishedProcesses() const;
    void displayStatus() const;
};

// lector de archivos
class FileParser {
private:
  string filename;
  SinglyLinkedList* processes;
    
  // helpers
  Process* parseProcess(FILE* file);
  bool parseInstructions(FILE* file, Process* process);

public:
  FileParser();
  FileParser(const char* inputFile);
  ~FileParser();
  
  // operaciones de archivos
  bool parseFile();
  LinkedList* getProcesses();
};
```

### II. Estrategia de testeo 

- Pruebas unitarias -
1. Creacion de procesos y manejo de estados
2. Lectura de archivos
3. Archivos de planificacion correctamente implementados
4. Manejo de tiempo

- Pruebas de integracion -
1. Completar el ciclo de un proceso
2. Planificacion de multiples procesos
3. Manejo de operaciones IO
4. Manejo de colas

- Pruebas de sistema -
1. Simulacion completa con archivos de prueba
2. Casos extremos (procesos maximos, instrucciones largas, etc.)
3. Condiciones de error

### III. Ejemplo de proceso
```
# EN MV EL PROFE SUBIO PROCESOS DE PRUEBA!!!
# 1 quantum = 5 ciclos (5 segundos)
# se le asigna 1 quantum a cada proceso; lo que quede sin realizar porque falta tiempo queda sin hacerse y el proceso se termina
# para la prioridad: +1 por instruccion normal; -1 por io

# sintaxis de declaracion de procesos:
# flag-de-proceso nombre-de-proceso prioridad-inicial
Proceso p1 6    # La instruccion de inicio no consume tiempo
i1              # instruccion normal: 1 ciclo; 1 ciclo en total
i2              # 1 ciclo; 2 ciclos total
i3              # 1 ciclo; 3 ciclos total
io              # 1.5 ciclos; 4.5 ciclos total
io              # 1.5 ciclos; 6 ciclos total -> se pasa del limite de 1 quantum: la instruccion no se llega a realizar y se termina el proceso prematuramente
i4
end             # La instruccion final tampoco
Proceso ptrew 3
i1
i2
end

# un proceso completado no se tiene que volver a realizar (sale de la cola); un proceso a medias tiene que volver a la cola para poder ser terminado en una segunda o tercera pasada 
```

# DIVISON DE TAREAS

## PERSONA A: Procesos y estructuras de datos 

### Semana 1 
1. Implementar clase Process
2. manejo de estados de procesos
3. logica de ejecucion de instrucciones
4. crear utilidades de manejo de tiempo (metodo sleep)
5. Interpretar definiciones de proceso

### Semana 2 
1. IO
2. transiciones entre estados
3. sistema de monitoreo de procesos
4. testeo unitario de clase Process

### Semana 3 
1. Ayudar en integracion
2. bugs
3. Documentacion final de procesos
4. ayudar en integracion final

## PERSONA B: archivos y planificador 

### Semana 1 
1. implementar FileParser
2. crear funcionalidad de leer los archivos
3. interpretar las definiciones los procesos en los archivos
4. iniciar Infraestructura del planificador

### Semana 2 
1. Round Robin
2. temporizador en quantums
3. manejo de colas de procesos
4. testeo unitario del Planificador

### Semana 3 
1. testeo de integracion
2. optimizacion
3. documentacion de Planificador
4. ayudar en integracion final

## PERSONA C: planificador de prioridad y UI 

### Semana 1 
1. planificacion por prioridad
2. crear cola de prioridad con un arbol binario
3. disennar UI
4. implementacion basica del menu

### Semana 2 
1. implementar UI
2. sistema de display de estado de procesos
3. lidiar con input de usuario
4. implementar manejo de errores

### Semana 3 
1. testeo de sistema
2. pulido de UI
3. documentacion final
4. coordinar integracion final

## PARA LOS TRES 
1. Revision de codigo (miercoles, sabado)
2. Testeo de integracion
3. Revision de la documentacion
4. Preparacion de la presentacion

## NOTAS DE INTEGRACION 
1. clase Process -> clase Scheduler (Planificador)
2. clase FileParser -> creacion de procesos
3. Planificador -> UI
4. Manejo de colas de prioridad

## RESPONSABILIDADES DE TESTEO 

###  Persona A 
1. transiciones de estados
3. execucion de instrucciones
4. IO

### Persona B 
1. Round Robin
2. lectura de archivos
3. manejo de colas

### Persona C 
1. planificacion por prioridad
2. UI
3. integracion de sistema

## RESPONSABILIDADES DE DOCUMENTACION 

### Persona A 
1. clase Process
3. manejo de estados
4. especificaciones tecnicas

### Persona B 
1. lector de archivos
2. planificador
3. API?

### Persona C 
1. manual de usuario
2. guia de instalacion
3. documentacion de testeo

## A TENER EN CUENTA 
1. usar git (github.com/doctuspullus/planificador-de-procesos)
2. commits diarios MINIMO (aunque sea un for loop xd)
3. usar branches para componentes grandes
4. documentar lineas dificiles de entender, o escribir codigo facil de entender sin documentar
5. realizar pruebas unitarias ANTES de integrar
6. comunicacion acerca de dependencias 
7. respaldo regular del progreso 

## PARA EVITAR PROBLEMAS 
1. el codigo tiene que entenderse por los tres
2. discutir regularmente acerca del codigo escrito
3. documentar suposiciones claramente
4. estar pendiente de los cambios diarios
5. solucionar problemas de bloqueo lo mas pronto posible
6. tener copias de respaldo
7. planes de contingencia

## ESTRATEGIA DE INTEGRACION 

### Semana 1 
1. definir interfaces entre componentes
2. decidir como usar las estructuras de datos
3. establecer una Infraestructura de testeo compartida

### Semana 2 
1. iniciar integracion de componentes
2. testeo diario de la integracion
3. lidiar con problemas de interfaces

### Semana 3 
1. Completar integracion de sistema
2. Testeo completo de sistema
3. optimizacion
4. finalizacion de documentacion
