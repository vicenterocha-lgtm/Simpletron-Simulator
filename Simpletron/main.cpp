#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>

using namespace std;

const int TAMANO_MEMORIA = 1000;

const int READ = 10;
const int WRITE = 11;
const int READSTRING = 12;
const int WRITESTRING = 13;
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

            case READSTRING: {
                string entradaString;
                cout << "Ingrese una cadena de texto: ";
                cin.ignore();
                getline(cin, entradaString);
                int longitud = entradaString.length();
                int baseUbicacion = operand;
                if (baseUbicacion < TAMANO_MEMORIA) {
                    memory[baseUbicacion] = longitud * 1000;
                    baseUbicacion++;
                }
                for (int i = 0; i < longitud; i++) {
                    if (baseUbicacion < TAMANO_MEMORIA) {
                        int grupoSuperior = (i + 1) * 1000;
                        int grupoInferior = static_cast<int>(entradaString[i]);
                        memory[baseUbicacion] = grupoSuperior + grupoInferior;
                        baseUbicacion++;
                    }
                }
                instructionCounter++;
                break;
            }

            case WRITESTRING: {
                int baseUbicacion = operand;
                cout << "Salida de cadena: ";
                if (baseUbicacion < TAMANO_MEMORIA) {
                    int longitud = memory[baseUbicacion] / 1000;
                    baseUbicacion++;
                    for (int i = 0; i < longitud; i++) {
                        if (baseUbicacion < TAMANO_MEMORIA) {
                            int asciiValor = memory[baseUbicacion] % 1000;
                            cout << static_cast<char>(asciiValor);
                            baseUbicacion++;
                        }
                    }
                }
                cout << endl;
                instructionCounter++;
                break;
            }
                
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
                if (!validarRango(accumulator)) {
                    cout << "\n*** ERROR: DESBORDAMIENTO DEL ACUMULADOR (EJECUCION ABORTADA) ***\n";
                    keepRunning = false;
                } else {
                    instructionCounter++;
                }
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

            case MODULUS:
                if (memory[operand] == 0) {
                    cout << "\n*** ERROR: DIVISION ENTRE CERO EN OPERACION MODULO (EJECUCION ABORTADA) ***\n";
                    keepRunning = false;
                } else {
                    accumulator %= memory[operand];
                    instructionCounter++;
                }
                break;

            case EXPONENT: {
                int base = accumulator;
                int exponente = memory[operand];
                
                if (exponente < 0) {
                    cout << "\n*** ERROR: EXPONENTE NEGATIVO NO SOPORTADO (EJECUCION ABORTADA) ***\n";
                    keepRunning = false;
                } else {
                    long long resultado = 1;
                    bool overflow = false;
                    
                    for (int i = 0; i < exponente; i++) {
                        resultado *= base;
                        if (resultado < -9999 || resultado > 9999) {
                            overflow = true;
                            break;
                        }
                    }
                    
                    if (overflow) {
                        cout << "\n*** ERROR: DESBORDAMIENTO DEL ACUMULADOR EN EXPONENCIACION (EJECUCION ABORTADA) ***\n";
                        keepRunning = false;
                    } else {
                        accumulator = static_cast<int>(resultado);
                        instructionCounter++;
                    }
                }
                break;
            }
                
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

        if (instructionCounter < 0 || instructionCounter >= TAMANO_MEMORIA) {
            cout << "\n*** ERROR: CONTADOR DE INSTRUCCIONES FUERA DE RANGO ***\n";
            keepRunning = false;
        }
    }
}

void dump(int accumulator, int instructionCounter, int instructionRegister, int operationCode, int operand, int memory[]) {
    cout << "\n*** REGISTROS ***\n";
    cout << "accumulator          " << setfill(' ') << setw(5) << internal << showpos << accumulator << endl;
    cout << "instructionCounter   " << setfill('0') << setw(3) << noshowpos << instructionCounter << endl;
    cout << "instructionRegister  " << setfill(' ') << setw(5) << internal << showpos << instructionRegister << endl;
    cout << "operationCode        " << setfill('0') << setw(2) << noshowpos << operationCode << endl;
    cout << "operand              " << setfill('0') << setw(3) << noshowpos << operand << endl;

    cout << "\n*** MEMORIA ***\n        ";
    for (int i = 0; i < 10; i++) cout << setw(5) << i << "  ";
    cout << "\n";

    for (int i = 0; i < TAMANO_MEMORIA; i += 10) {
        cout << setfill('0') << setw(3) << i << "  ";
        for (int j = 0; j < 10; j++) {
            cout << setfill(' ') << setw(5) << internal << showpos << memory[i + j] << "  ";
        }
        cout << "\n";
    }
}

int main() {
    int memory[TAMANO_MEMORIA] = {0}; 
    int accumulator = 0;
    int instructionCounter = 0;
    int instructionRegister = 0;
    int operationCode = 0;
    int operand = 0;
    
    cargarPrograma(memory);
    ejecutarPrograma(memory, accumulator, instructionCounter, instructionRegister, operationCode, operand);
    dump(accumulator, instructionCounter, instructionRegister, operationCode, operand, memory);

    return 0;
}