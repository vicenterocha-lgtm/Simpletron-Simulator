#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include <cctype>
#include <algorithm>

using namespace std;

struct Token {
    string tipo;
    string valor;
};

struct EntradaTabla {
    string simbolo;
    char tipo;
    int ubicacion;
};

class Compilador {
private:
    vector<Token> tokens;
    vector<EntradaTabla> tablaSimbolos;
    vector<int> memoriaSML;
    int instruccionCounter;
    int dataCounter;

    int buscarSimbolo(const string& sim, char tipo) {
        for (size_t i = 0; i < tablaSimbolos.size(); i++) {
            if (tablaSimbolos[i].simbolo == sim && tablaSimbolos[i].tipo == tipo) {
                return tablaSimbolos[i].ubicacion;
            }
        }
        return -1;
    }

    void agregarSimbolo(const string& sim, char tipo, int ubicacion) {
        if (buscarSimbolo(sim, tipo) == -1) {
            EntradaTabla nueva = {sim, tipo, ubicacion};
            tablaSimbolos.push_back(nueva);
        }
    }

    bool esNumero(const string& s) {
        if (s.empty()) return false;
        size_t inicio = 0;
        if (s[0] == '-' || s[0] == '+') {
            if (s.size() == 1) return false;
            inicio = 1;
        }
        for (size_t i = inicio; i < s.size(); i++) {
            if (!isdigit(s[i]) && s[i] != '.') return false;
        }
        return true;
    }

public:
    Compilador() {
        instruccionCounter = 0;
        dataCounter = 999;
        memoriaSML.resize(1000, 0);
    }

    void tokenizar(const string& codigoCompleto) {
        stringstream ss(codigoCompleto);
        string linea;

        while (getline(ss, linea)) {
            if (linea.empty()) continue;
            stringstream streamLinea(linea);
            string palabra;
            bool esPrimerToken = true;

            while (streamLinea >> palabra) {
                Token t;
                if (esPrimerToken) {
                    t.tipo = "NUM_LINEA";
                    t.valor = palabra;
                    esPrimerToken = false;
                } else if (palabra == "input" || palabra == "print" || palabra == "let" || 
                           palabra == "goto" || palabra == "if" || palabra == "end" || palabra == "rem") {
                    t.tipo = "COMANDO";
                    t.valor = palabra;
                } else if (palabra == "+" || palabra == "-" || palabra == "*" || palabra == "/" || 
                           palabra == "%" || palabra == "^" || palabra == "=" || palabra == "==" || 
                           palabra == "<=" || palabra == ">=" || palabra == "<" || palabra == ">") {
                    t.tipo = "OPERADOR";
                    t.valor = palabra;
                } else if (esNumero(palabra)) {
                    t.tipo = "CONSTANTE";
                    t.valor = palabra;
                } else {
                    t.tipo = "VARIABLE";
                    t.valor = palabra;
                }
                tokens.push_back(t);
            }
        }
    }

    void primeraPasada() {
        for (size_t i = 0; i < tokens.size(); i++) {
            if (tokens[i].tipo == "NUM_LINEA") {
                agregarSimbolo(tokens[i].valor, 'L', instruccionCounter);
                instruccionCounter++; 
            } 
            else if (tokens[i].tipo == "VARIABLE") {
                if (buscarSimbolo(tokens[i].valor, 'V') == -1) {
                    agregarSimbolo(tokens[i].valor, 'V', dataCounter);
                    dataCounter--;
                }
            } 
            else if (tokens[i].tipo == "CONSTANTE") {
                if (buscarSimbolo(tokens[i].valor, 'C') == -1) {
                    agregarSimbolo(tokens[i].valor, 'C', dataCounter);
                    memoriaSML[dataCounter] = stoi(tokens[i].valor);
                    dataCounter--;
                }
            }
        }
        cout << "[OK] Primera Pasada: Tabla de Simbolos generada.\n";
    }

    void segundaPasada(const string& codigoCompleto) {
        stringstream ss(codigoCompleto);
        string linea;
        int pc = 0;

        while (getline(ss, linea)) {
            if (linea.empty()) continue;
            stringstream streamLinea(linea);
            string etiqueta, comando;
            streamLinea >> etiqueta >> comando;

            if (comando == "rem") {
                memoriaSML[pc] = 0;
                pc++;
            }
            else if (comando == "input") {
                string var;
                streamLinea >> var;
                int dirVar = buscarSimbolo(var, 'V');
                memoriaSML[pc] = 10 * 1000 + dirVar;
                pc++;
            }
            else if (comando == "print") {
                string var;
                streamLinea >> var;
                int dirVar = buscarSimbolo(var, 'V');
                if (dirVar == -1) dirVar = buscarSimbolo(var, 'C');
                memoriaSML[pc] = 11 * 1000 + dirVar;
                pc++;
            }
            else if (comando == "goto") {
                string destino;
                streamLinea >> destino;
                int dirDestino = buscarSimbolo(destino, 'L');
                memoriaSML[pc] = 40 * 1000 + dirDestino;
                pc++;
            }
            else if (comando == "end") {
                memoriaSML[pc] = 43 * 1000;
                pc++;
            }
            else if (comando == "let") {
                string varDestino, igual, operando1, op, operando2;
                streamLinea >> varDestino >> igual >> operando1;
                
                int dirDest = buscarSimbolo(varDestino, 'V');
                int dirOp1 = buscarSimbolo(operando1, 'V');
                if (dirOp1 == -1) dirOp1 = buscarSimbolo(operando1, 'C');

                if (streamLinea >> op >> operando2) {
                    int dirOp2 = buscarSimbolo(operando2, 'V');
                    if (dirOp2 == -1) dirOp2 = buscarSimbolo(operando2, 'C');

                    memoriaSML[pc] = 20 * 1000 + dirOp1;
                    pc++;

                    if (op == "+") {
                        memoriaSML[pc] = 30 * 1000 + dirOp2;
                    } else if (op == "-") {
                        memoriaSML[pc] = 31 * 1000 + dirOp2;
                    } else if (op == "*") {
                        memoriaSML[pc] = 33 * 1000 + dirOp2;
                    } else if (op == "/") {
                        memoriaSML[pc] = 32 * 1000 + dirOp2;
                    }
                    pc++;

                    memoriaSML[pc] = 21 * 1000 + dirDest;
                    pc++;
                } else {
                    memoriaSML[pc] = 20 * 1000 + dirOp1;
                    pc++;
                    memoriaSML[pc] = 21 * 1000 + dirDest;
                    pc++;
                }
            }
            else if (comando == "if") {
                string op1, rel, op2, goto_cmd, destino;
                streamLinea >> op1 >> rel >> op2 >> goto_cmd >> destino;

                int dirOp1 = buscarSimbolo(op1, 'V');
                if (dirOp1 == -1) dirOp1 = buscarSimbolo(op1, 'C');

                int dirOp2 = buscarSimbolo(op2, 'V');
                if (dirOp2 == -1) dirOp2 = buscarSimbolo(op2, 'C');

                int dirDest = buscarSimbolo(destino, 'L');

                memoriaSML[pc] = 20 * 1000 + dirOp1;
                pc++;
                memoriaSML[pc] = 31 * 1000 + dirOp2;
                pc++;

                if (rel == "==") {
                    memoriaSML[pc] = 42 * 1000 + dirDest;
                    pc++;
                } else if (rel == "<") {
                    memoriaSML[pc] = 41 * 1000 + dirDest;
                    pc++;
                } else if (rel == "<=") {
                    memoriaSML[pc] = 41 * 1000 + dirDest;
                    pc++;
                    memoriaSML[pc] = 42 * 1000 + dirDest;
                    pc++;
                } else if (rel == ">") {
                    memoriaSML[pc] = 40 * 1000 + dirDest;
                    pc++;
                }
            }
        }
        cout << "[OK] Segunda Pasada: Codigo traducido exitosamente a SML.\n";
    }

    void exportarSML(const string& nombreArchivo) {
        ofstream salida(nombreArchivo);
        if (salida.is_open()) {
            for (int i = 0; i < 1000; i++) {
                if (i < instruccionCounter || i > dataCounter) {
                    salida << setfill('0') << setw(3) << i << " " 
                           << (memoriaSML[i] >= 0 ? "+" : "-") 
                           << setfill('0') << setw(5) << abs(memoriaSML[i]) << "\n";
                }
            }
            salida.close();
            cout << "[OK] Archivo binario/SML generado: " << nombreArchivo << "\n";
        }
    }
};

int main() {
    string nombreEntrada = "programa.simp";
    string nombreSalida = "programa.sml";

    ifstream archivo(nombreEntrada);
    string linea;
    string codigoCompleto = "";

    if (archivo.is_open()) {
        while (getline(archivo, linea)) {
            codigoCompleto += linea + "\n";
        }
        archivo.close();
        
        Compilador compiler;
        compiler.tokenizar(codigoCompleto);
        compiler.primeraPasada();
        compiler.segundaPasada(codigoCompleto);
        compiler.exportarSML(nombreSalida);
        
        cout << "\n=======================================================\n";
        cout << "¡COMPILACIÓN EXITOSA! '" << nombreEntrada << "' -> '" << nombreSalida << "'\n";
        cout << "=======================================================\n";
    } else {
        cout << "Error critico: No se pudo abrir " << nombreEntrada << " para compilacion.\n";
    }

    return 0;
}