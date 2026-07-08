#include <iostream>
#include <iomanip>

using namespace std;

const int READ = 10;
const int WRITE = 11;
const int LOAD = 20;
const int STORE = 21;
const int ADD = 30;
const int SUBTRACT = 31;
const int DIVIDE = 32;
const int MULTIPLY = 33;
const int BRANCH = 40;
const int BRANCHNEG = 41;
const int BRANCHZERO = 42;
const int HALT = 43;

// Funcion para validar que los valores esten en el rango correcto
bool validarRango(int valor) {
    if (valor < -9999 || valor > 9999) {
        return false;
    }
    return true;
}

void dump(int accumulator, int instructionCounter, int instructionRegister, int operationCode, int operand, int memory[]) {
    cout << "\n*** REGISTROS ***\n";
    cout << "accumulator          " << setfill(' ') << setw(5) << internal << showpos << accumulator << endl;
    cout << "instructionCounter   " << setfill('0') << setw(2) << noshowpos << instructionCounter << endl;
    cout << "instructionRegister  " << setfill(' ') << setw(5) << internal << showpos << instructionRegister << endl;
    cout << "operationCode        " << setfill('0') << setw(2) << noshowpos << operationCode << endl;
    cout << "operand              " << setfill('0') << setw(2) << noshowpos << operand << endl;

    cout << "\n*** MEMORIA ***\n       ";
    for (int i = 0; i < 10; i++) cout << setw(5) << i << " ";
    cout << "\n";

    for (int i = 0; i < 100; i += 10) {
        cout << setw(3) << i << "  ";
        for (int j = 0; j < 10; j++) {
            cout << setfill(' ') << setw(5) << internal << showpos << memory[i + j] << " ";
        }
        cout << "\n";
    }
}

int main() {
    int memory[100] = {0}; 
    int accumulator = 0;
    int instructionCounter = 0;
    int instructionRegister = 0;
    int operationCode = 0;
    int operand = 0;
    
    cout << "*** Bienvenido a Simpletron! ***\n";
    cout << "*** Por favor, introduzca su programa una instruccion ***\n";
    cout << "*** (o palabra de datos) a la vez. Yo tecleare el ***\n";
    cout << "*** numero de ubicacion y un signo de interrogacion (?). ***\n";
    cout << "*** Entonces usted tecleara la palabra para esa ubicacion. ***\n";
    cout << "*** Teclee -99999 para terminar de introducir el programa. ***\n\n";

    int entrada = 0;
    int ubicacion = 0;
    while (ubicacion < 100) {
        cout << setfill('0') << setw(2) << ubicacion << " ? ";
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
    cout << "*** Comienza la ejecucion del programa ***\n\n";

    bool keepRunning = true;
    while (keepRunning) {
        instructionRegister = memory[instructionCounter];
        operationCode = instructionRegister / 100;
        operand = instructionRegister % 100;

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
                instructionCounter++;
                break;
                
            case SUBTRACT:
                accumulator -= memory[operand];
                instructionCounter++;
                break;
                
            case MULTIPLY:
                accumulator *= memory[operand];
                instructionCounter++;
                break;
                
            case DIVIDE:
                if (memory[operand] == 0) {
                    cout << "\n*** ERROR: DIVISION ENTRE CERO (EJECUCION ABORTADA) ***\n";
                    keepRunning = false;
                } else {
                    accumulator /= memory[operand];
                    instructionCounter++;
                }
                break;
                
            case BRANCH:
                instructionCounter = operand;
                break;
                
            case BRANCHNEG:
                if (accumulator < 0) instructionCounter = operand;
                else instructionCounter++;
                break;
                
            case BRANCHZERO:
                if (accumulator == 0) instructionCounter = operand;
                else instructionCounter++;
                break;
                
            case HALT:
                cout << "\n*** Simpletron termina su ejecucion de forma limpia ***\n";
                keepRunning = false;
                break;
                
            default:
                cout << "\n*** ERROR: CODIGO DE OPERACION INVALIDO ***\n";
                keepRunning = false;
                break;
        }

        if (instructionCounter < 0 || instructionCounter >= 100) {
            cout << "\n*** ERROR: CONTADOR DE INSTRUCCIONES FUERA DE RANGO ***\n";
            keepRunning = false;
        }
    }

    dump(accumulator, instructionCounter, instructionRegister, operationCode, operand, memory);

    return 0;
}