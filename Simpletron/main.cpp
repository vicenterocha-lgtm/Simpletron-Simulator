#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <cmath>

using namespace std;

const int TAMANO_MEMORIA = 1000;

const int READ = 10;
const int WRITE = 11;
const int READSTRING = 12;
const int WRITESTRING = 13;
const int NEWLINE = 14;
const int LOAD = 20;
const int STORE = 21;
const int ADD = 30;
const int SUBTRACT = 31;
const int DIVIDE = 32;
const int MULTIPLY = 33;
const int MODULUS = 34;
const int EXPONENT = 35;
const int BRANCH = 40;
const int BRANCHNEG = 41;
const int BRANCHZERO = 42;
const int HALT = 43;

bool validarRango(double valor) {
    if (valor < -99999.0 || valor > 99999.0) {
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

void cargarPrograma(double memory[]) {
    ifstream archivo("../programa.sml");
    if (!archivo.is_open()) {
        archivo.open("programa.sml");
    }
    
    if (archivo.is_open()) {
        cout << "*** Cargando programa de forma automatica desde 'programa.sml'... ***\n";
        double entrada = 0.0;
        int direccionDummy = 0;
        int ubicacion = 0;
        
        // Lee primero la dirección (000) y luego la instrucción (+10999)
        while (ubicacion < TAMANO_MEMORIA && archivo >> direccionDummy >> entrada) {
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
        
        double entrada = 0.0;
        int ubicacion = 0;
        while (ubicacion < TAMANO_MEMORIA) {
            cout << setfill('0') << setw(3) << ubicacion << " ? ";
            cin >> entrada;
            if (entrada == -99999) break;
            
            if (!validarRango(entrada)) {
                cout << "*** Error: Valor fuera del rango permitido (-99999 a +99999). Intente de nuevo. ***\n";
                continue;
            }
            memory[ubicacion] = entrada;
            ubicacion++;
        }
        cout << "*** Carga del programa terminada ***\n";
    }
}

void ejecutarPrograma(double memory[], double& accumulator, int& instructionCounter, double& instructionRegister, int& operationCode, int& operand) {
    cout << "*** Comienza la ejecucion del programa ***\n\n";
    bool keepRunning = true;
    
    while (keepRunning) {
        instructionRegister = memory[instructionCounter];
        
        operationCode = static_cast<int>(instructionRegister) / 1000;
        operand = static_cast<int>(instructionRegister) % 1000;

        switch (operationCode) {
            case READ: {
                double valorEntrada = 0.0;
                bool entradaValida = false;
                while (!entradaValida) {
                    cout << "Ingrese un numero (-99999 a +99999): ";
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
                if (operand < 0 || operand >= TAMANO_MEMORIA) {
                    cout << "\n*** ERROR: DIRECCION DE OPERANDO INVALIDA EN OPERACION WRITE ***\n";
                    keepRunning = false;
                } else {
                    cout << "Salida: " << memory[operand] << endl;
                    instructionCounter++;
                }
                break;

            case NEWLINE:
                cout << "\n";
                instructionCounter++;
                break;

            case READSTRING: {
                if (operand < 0 || operand >= TAMANO_MEMORIA) {
                    cout << "\n*** ERROR: DIRECCION DE BASE INVALIDA EN OPERACION READSTRING ***\n";
                    keepRunning = false;
                    break;
                }
                string entradaString;
                cout << "Ingrese una cadena de texto: ";
                cin.ignore();
                getline(cin, entradaString);
                int longitud = entradaString.length();
                int baseUbicacion = operand;
                if (baseUbicacion < TAMANO_MEMORIA) {
                    memory[baseUbicacion] = static_cast<double>(longitud * 1000);
                    baseUbicacion++;
                }
                for (int i = 0; i < longitud; i++) {
                    if (baseUbicacion < TAMANO_MEMORIA) {
                        int grupoSuperior = (i + 1) * 1000;
                        int grupoInferior = static_cast<int>(entradaString[i]);
                        memory[baseUbicacion] = static_cast<double>(grupoSuperior + grupoInferior);
                        baseUbicacion++;
                    }
                }
                instructionCounter++;
                break;
            }

            case WRITESTRING: {
                if (operand < 0 || operand >= TAMANO_MEMORIA) {
                    cout << "\n*** ERROR: DIRECCION DE BASE INVALIDA EN OPERACION WRITESTRING ***\n";
                    keepRunning = false;
                    break;
                }
                int baseUbicacion = operand;
                cout << "Salida de cadena: ";
                if (baseUbicacion < TAMANO_MEMORIA) {
                    int longitud = static_cast<int>(memory[baseUbicacion]) / 1000;
                    baseUbicacion++;
                    for (int i = 0; i < longitud; i++) {
                        if (baseUbicacion < TAMANO_MEMORIA) {
                            int asciiValor = static_cast<int>(memory[baseUbicacion]) % 1000;
                            cout << static_cast<char>(asciiValor);
                            baseUbicacion++;
                        } else {
                            cout << "\n*** ERROR: LECTURA DE CADENA EXGEDE EL LIMITE DE MEMORIA ***\n";
                            keepRunning = false;
                            break;
                        }
                    }
                }
                cout << endl;
                if (keepRunning) {
                    instructionCounter++;
                }
                break;
            }
                
            case LOAD:
                if (operand < 0 || operand >= TAMANO_MEMORIA) {
                    cout << "\n*** ERROR: DIRECCION DE OPERANDO INVALIDA EN OPERACION LOAD ***\n";
                    keepRunning = false;
                } else {
                    accumulator = memory[operand];
                    instructionCounter++;
                }
                break;
                
            case STORE:
                if (operand < 0 || operand >= TAMANO_MEMORIA) {
                    cout << "\n*** ERROR: DIRECCION DE OPERANDO INVALIDA EN OPERACION STORE ***\n";
                    keepRunning = false;
                } else {
                    memory[operand] = accumulator;
                    instructionCounter++;
                }
                break;
                
            case ADD:
                if (operand < 0 || operand >= TAMANO_MEMORIA) {
                    cout << "\n*** ERROR: DIRECCION DE OPERANDO INVALIDA EN OPERACION ADD ***\n";
                    keepRunning = false;
                } else {
                    accumulator += memory[operand];
                    if (!validarRango(accumulator)) {
                        cout << "\n*** ERROR: DESBORDAMIENTO DEL ACUMULADOR (EJECUCION ABORTADA) ***\n";
                        keepRunning = false;
                    } else {
                        instructionCounter++;
                    }
                }
                break;
                
            case SUBTRACT:
                if (operand < 0 || operand >= TAMANO_MEMORIA) {
                    cout << "\n*** ERROR: DIRECCION DE OPERANDO INVALIDA EN OPERACION SUBTRACT ***\n";
                    keepRunning = false;
                } else {
                    accumulator -= memory[operand];
                    if (!validarRango(accumulator)) {
                        cout << "\n*** ERROR: DESBORDAMIENTO DEL ACUMULADOR (EJECUCION ABORTADA) ***\n";
                        keepRunning = false;
                    } else {
                        instructionCounter++;
                    }
                }
                break;
                
            case MULTIPLY:
                if (operand < 0 || operand >= TAMANO_MEMORIA) {
                    cout << "\n*** ERROR: DIRECCION DE OPERANDO INVALIDA EN OPERACION MULTIPLY ***\n";
                    keepRunning = false;
                } else {
                    accumulator *= memory[operand];
                    if (!validarRango(accumulator)) {
                        cout << "\n*** ERROR: DESBORDAMIENTO DEL ACUMULADOR (EJECUCION ABORTADA) ***\n";
                        keepRunning = false;
                    } else {
                        instructionCounter++;
                    }
                }
                break;
                
            case DIVIDE:
                if (operand < 0 || operand >= TAMANO_MEMORIA) {
                    cout << "\n*** ERROR: DIRECCION DE OPERANDO INVALIDA EN OPERACION DIVIDE ***\n";
                    keepRunning = false;
                } else {
                    if (memory[operand] == 0.0) {
                        cout << "\n*** ERROR: DIVISION ENTRE CERO (EJECUCION ABORTADA) ***\n";
                        keepRunning = false;
                    } else {
                        accumulator /= memory[operand];
                        instructionCounter++;
                    }
                }
                break;

            case MODULUS:
                if (operand < 0 || operand >= TAMANO_MEMORIA) {
                    cout << "\n*** ERROR: DIRECCION DE OPERANDO INVALIDA EN OPERACION MODULO ***\n";
                    keepRunning = false;
                } else {
                    if (memory[operand] == 0.0) {
                        cout << "\n*** ERROR: DIVISION ENTRE CERO EN OPERACION MODULO (EJECUCION ABORTADA) ***\n";
                        keepRunning = false;
                    } else {
                        accumulator = fmod(accumulator, memory[operand]);
                        instructionCounter++;
                    }
                }
                break;

            case EXPONENT: {
                if (operand < 0 || operand >= TAMANO_MEMORIA) {
                    cout << "\n*** ERROR: DIRECCION DE OPERANDO INVALIDA EN OPERACION EXPONENT ***\n";
                    keepRunning = false;
                    break;
                }
                
                if (memory[operand] < 0) {
                    cout << "\n*** ERROR: EXPONENTE NEGATIVO NO SOPORTADO (EJECUCION ABORTADA) ***\n";
                    keepRunning = false;
                } else {
                    accumulator = pow(accumulator, memory[operand]);
                    if (!validarRango(accumulator)) {
                        cout << "\n*** ERROR: DESBORDAMIENTO DEL ACUMULADOR EN EXPONENCIACION (EJECUCION ABORTADA) ***\n";
                        keepRunning = false;
                    } else {
                        instructionCounter++;
                    }
                }
                break;
            }
                
            case BRANCH:
                instructionCounter = operand;
                break;
                
            case BRANCHNEG:
                if (accumulator < 0.0) instructionCounter = operand;
                else instructionCounter++;
                break;
                
            case BRANCHZERO:
                if (accumulator == 0.0) instructionCounter = operand;
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

        if (instructionCounter < 0 || instructionCounter >= TAMANO_MEMORIA) {
            cout << "\n*** ERROR: CONTADOR DE INSTRUCCIONES FUERA DE RANGO ***\n";
            keepRunning = false;
        }
    }
}

void dump(double accumulator, int instructionCounter, double instructionRegister, int operationCode, int operand, double memory[]) {
    cout << "\n*** REGISTROS ***\n";
    cout << "accumulator          " << setfill(' ') << setw(10) << internal << showpos << accumulator << endl;
    cout << "instructionCounter   " << setfill('0') << setw(3) << noshowpos << instructionCounter << endl;
    cout << "instructionRegister  " << setfill(' ') << setw(10) << internal << showpos << instructionRegister << endl;
    cout << "operationCode        " << setfill('0') << setw(2) << noshowpos << operationCode << endl;
    cout << "operand              " << setfill('0') << setw(3) << noshowpos << operand << endl;

    cout << "\n*** MEMORIA ***\n        ";
    for (int i = 0; i < 10; i++) cout << setw(10) << i << "  ";
    cout << "\n";

    for (int i = 0; i < TAMANO_MEMORIA; i += 10) {
        cout << setfill('0') << setw(3) << noshowpos << i << "  ";
        for (int j = 0; j < 10; j++) {
            cout << setfill(' ') << setw(10) << internal << showpos << memory[i + j] << "  ";
        }
        cout << "\n";
    }
}

int main() {
    double memory[TAMANO_MEMORIA] = {0.0}; 
    double accumulator = 0.0;
    int instructionCounter = 0;
    double instructionRegister = 0.0;
    int operationCode = 0;
    int operand = 0;
    
    cargarPrograma(memory);
    ejecutarPrograma(memory, accumulator, instructionCounter, instructionRegister, operationCode, operand);
    dump(accumulator, instructionCounter, instructionRegister, operationCode, operand, memory);

    return 0;
}