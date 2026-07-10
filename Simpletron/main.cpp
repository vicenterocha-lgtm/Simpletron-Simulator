#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>

using namespace std;


const int TAMANO_MEMORIA = 1000;


const int READ = 10;
const int WRITE = 11;
const int LOAD = 20;
const int STORE = 21;
const int ADD = 30;
const int SUBTRACT = 31;
const int DIVIDE = 32;
const int MULTIPLY = 33;
const int MODULUS = 34; // Nueva operacion: Residuo (modulo)
const int BRANCH = 40;
const int BRANCHNEG = 41;
const int BRANCHZERO = 42;
const int HALT = 43;


bool validarRango(int valor) {
    if (valor < -9999 || valor > 9999) {
        return false;
    }
    return true;
}


void mostrarBienvenida() {
    cout << "*** Bienvenido a Simpletron! ***\n";
    cout << "*** Por favor, introduzca su programa una instruccion ***\n";
    cout << "*** (o palabra de datos) a la vez. Yo tecleare el ***\n";
    cout << "*** numero de ubicacion y un signo de interrogacion (?). ***\n";
    cout << "*** Entonces usted tecleara la palabra para esa ubicacion. ***\n";
    cout << "*** Teclee -99999 para terminar de introducir el programa. ***\n\n";
}


void cargarPrograma(int memory[]) {
    ifstream archivo("programa.sml");
    
    if (archivo.is_open()) {
        cout << "*** Cargando programa de forma automatica desde 'programa.sml'... ***\n";
        int entrada = 0;
        int ubicacion = 0;
        
        while (ubicacion < TAMANO_MEMORIA && archivo >> entrada) {
            if (entrada == -99999) break;
            
            if (!validarRango(entrada)) {
                cout << "*** Error en archivo (posicion " << ubicacion << "): Valor fuera de rango. Ejecucion abortada. ***\n";
                archivo.close();
                exit(1);
            }
            memory[ubicacion] = entrada;
            ubicacion++;
        }
        archivo.close();
        cout << "*** Carga automatica desde archivo terminada con exito ***\n\n";
    } else {
        cout << "--- Archivo 'programa.sml' no encontrado. Conservando carga interactiva linea por linea. ---\n\n";
        mostrarBienvenida();
        
        int entrada = 0;
        int ubicacion = 0;
        while (ubicacion < TAMANO_MEMORIA) {
            cout << setfill('0') << setw(3) << ubicacion << " ? ";
            cin >> entrada;
            if (entrada == -99999) break;
            
            if (!validarRango(entrada)) {
                cout << "*** Error: Valor fuera del rango permitido (-9999 a +9999). Intente de nuevo. ***\n";
                continue;
            }
            memory[ubicacion] = entrada;
            ubicacion++;
        }
        cout << "*** Carga del programa terminada ***\n";
    }
}


void ejecutarPrograma(int memory[], int& accumulator, int& instructionCounter, int& instructionRegister, int& operationCode, int& operand) {
    cout << "*** Comienza la ejecucion del programa ***\n\n";
    bool keepRunning = true;
    
    while (keepRunning) {
        instructionRegister = memory[instructionCounter];
        
        operationCode = instructionRegister / 1000;
        operand = instructionRegister % 1000;

        switch (operationCode) {
            case READ: {
                int valorEntrada = 0;
                bool entradaValida = false;
                while (!entradaValida) {
                    cout << "Ingrese un entero (-9999 a +9999): ";
                    cin >> valorEntrada;
                    if (validarRango(valorEntrada)) {
                        memory[operand] = valorEntrada;
                        entradaValida = true;
                    } else {
                        cout << "*** Error: Entrada invalida. Intente de nuevo. ***\n";
                    }
                }
                instructionCounter++;
                break;
            }
                
            case WRITE:
                cout << "Salida: " << memory[operand] << endl;
                instructionCounter++;
                break;
                
            case LOAD:
                accumulator = memory[operand];
                instructionCounter++;
                break;
                
            case STORE:
                memory[operand] = accumulator;
                instructionCounter++;
                break;
                
            case ADD:
                accumulator += memory[operand];
                if (!validarRango(accumulator)) {
                    cout << "\n*** ERROR: DESBORDAMIENTO DEL ACUMULADOR (EJECUCION ABORTADA) ***\n";
                    keepRunning = false;
                } else {
                    instructionCounter++;
                }
                break;
                
            case SUBTRACT:
                accumulator -= memory[operand];
                if (!validarRango(accumulator)) {
                    cout << "\n*** ERROR: DESBORDAMIENTO DEL ACUMULADOR (EJECUCION ABORTADA) ***\n";
                    keepRunning = false;
                } else {
                    instructionCounter++;
                }
                break;
                
            case MULTIPLY:
                accumulator *= memory[operand];