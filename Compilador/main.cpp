#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include <cctype>
#include <algorithm>
#include <stack>

using namespace std;

struct Token {
    string tipo;
    string valor;
};

struct EntradaTabla {
    string simbolo;
    char tipo; // 'L' = Linea, 'V' = Variable, 'C' = Constante
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

    int precedencia(const string& op) {
        if (op == "^") return 3;
        if (op == "*" || op == "/" || op == "%") return 2;
        if (op == "+" || op == "-") return 1;
        return 0;
    }

    vector<string> infijoAPostfijo(const vector<string>& tokensExpr) {
        vector<string> postfijo;
        stack<string> pila;

        for (const string& token : tokensExpr) {
            if (esNumero(token) || (isalpha(token[0]) && token.size() == 1)) {
                postfijo.push_back(token);
            } else if (token == "(") {
                pila.push(token);
            } else if (token == ")") {
                while (!pila.empty() && pila.top() != "(") {
                    postfijo.push_back(pila.top());
                    pila.pop();
                }
                if (!pila.empty()) pila.pop();
            } else { 
                while (!pila.empty() && precedencia(pila.top()) >= precedencia(token)) {
                    postfijo.push_back(pila.top());
                    pila.pop();
                }
                pila.push(token);
            }
        }
        while (!pila.empty()) {
            postfijo.push_back(pila.top());
            pila.pop();
        }
        return postfijo;
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
                           palabra == "<=" || palabra == ">=" || palabra == "<" || palabra == ">" ||
                           palabra == "(" || palabra == ")") {
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

    void primeraPasada(const string& codigoCompleto) {
        stringstream ss(codigoCompleto);
        string linea;
        int pcContador = 0;

        while (getline(ss, linea)) {
            if (linea.empty()) continue;
            stringstream streamLinea(linea);
            string numLinea, comando;
            if (!(streamLinea >> numLinea >> comando)) continue;

            agregarSimbolo(numLinea, 'L', pcContador);

            if (comando == "rem") {
                // REM no genera instrucciones SML
            } else if (comando == "input" || comando == "print" || comando == "goto" || comando == "end") {
                pcContador += 1;
            } else if (comando == "let") {
                string varDestino, igual;
                streamLinea >> varDestino >> igual;
                vector<string> exprTokens;
                string t;
                while (streamLinea >> t) exprTokens.push_back(t);

                if (exprTokens.size() == 1) {
                    pcContador += 2; // LOAD, STORE
                } else if (exprTokens.size() == 3) {
                    pcContador += 3; // LOAD, OP, STORE
                } else {
                    vector<string> post = infijoAPostfijo(exprTokens);
                    int ops = 0;
                    for (const auto& tk : post) {
                        if (tk == "+" || tk == "-" || tk == "*" || tk == "/" || tk == "%" || tk == "^") ops++;
                    }
                    pcContador += 1 + ops + 1;
                }
            } else if (comando == "if") {
                string op1, rel, op2, gotoCmd, dest;
                streamLinea >> op1 >> rel >> op2 >> gotoCmd >> dest;
                if (rel == "==" || rel == "<" || rel == ">") {
                    pcContador += 3; 
                } else if (rel == "<=" || rel == ">=") {
                    pcContador += 4; 
                }
            }

            string tokenAux;
            stringstream streamLinea2(linea);
            streamLinea2 >> numLinea;
            while (streamLinea2 >> tokenAux) {
                if (esNumero(tokenAux) && tokenAux != numLinea) {
                    if (buscarSimbolo(tokenAux, 'C') == -1) {
                        agregarSimbolo(tokenAux, 'C', dataCounter);
                        memoriaSML[dataCounter] = stoi(tokenAux);
                        dataCounter--;
                    }
                } else if (isalpha(tokenAux[0]) && tokenAux.size() == 1) {
                    if (buscarSimbolo(tokenAux, 'V') == -1) {
                        agregarSimbolo(tokenAux, 'V', dataCounter);
                        dataCounter--;
                    }
                }
            }
        }
        instruccionCounter = pcContador;
        cout << "[OK] Primera Pasada: Tabla de Simbolos calculada correctamente.\n";
    }

    void segundaPasada(const string& codigoCompleto) {
        stringstream ss(codigoCompleto);
        string linea;
        int pc = 0;

        while (getline(ss, linea)) {
            if (linea.empty()) continue;
            stringstream streamLinea(linea);
            string etiqueta, comando;
            if (!(streamLinea >> etiqueta >> comando)) continue;

            if (comando == "rem") {
                continue;
            }
            else if (comando == "input") {
                string var;
                streamLinea >> var;
                int dirVar = buscarSimbolo(var, 'V');
                memoriaSML[pc++] = 10 * 1000 + dirVar;
            }
            else if (comando == "print") {
                string var;
                streamLinea >> var;
                int dirVar = buscarSimbolo(var, 'V');
                if (dirVar == -1) dirVar = buscarSimbolo(var, 'C');
                memoriaSML[pc++] = 11 * 1000 + dirVar;
            }
            else if (comando == "goto") {
                string destino;
                streamLinea >> destino;
                int dirDestino = buscarSimbolo(destino, 'L');
                memoriaSML[pc++] = 40 * 1000 + dirDestino;
            }
            else if (comando == "end") {
                memoriaSML[pc++] = 43 * 1000;
            }
            else if (comando == "let") {
                string varDestino, igual;
                streamLinea >> varDestino >> igual;
                int dirDest = buscarSimbolo(varDestino, 'V');

                vector<string> exprTokens;
                string t;
                while (streamLinea >> t) exprTokens.push_back(t);

                if (exprTokens.size() == 1) {
                    int dirOp = buscarSimbolo(exprTokens[0], 'V');
                    if (dirOp == -1) dirOp = buscarSimbolo(exprTokens[0], 'C');
                    memoriaSML[pc++] = 20 * 1000 + dirOp; // LOAD
                    memoriaSML[pc++] = 21 * 1000 + dirDest; // STORE
                } 
                else if (exprTokens.size() == 3) {
                    int dirOp1 = buscarSimbolo(exprTokens[0], 'V');
                    if (dirOp1 == -1) dirOp1 = buscarSimbolo(exprTokens[0], 'C');
                    int dirOp2 = buscarSimbolo(exprTokens[2], 'V');
                    if (dirOp2 == -1) dirOp2 = buscarSimbolo(exprTokens[2], 'C');

                    memoriaSML[pc++] = 20 * 1000 + dirOp1; // LOAD

                    string op = exprTokens[1];
                    if (op == "+") memoriaSML[pc++] = 30 * 1000 + dirOp2;
                    else if (op == "-") memoriaSML[pc++] = 31 * 1000 + dirOp2;
                    else if (op == "*") memoriaSML[pc++] = 33 * 1000 + dirOp2;
                    else if (op == "/") memoriaSML[pc++] = 32 * 1000 + dirOp2;
                    else if (op == "%") memoriaSML[pc++] = 34 * 1000 + dirOp2;
                    else if (op == "^") memoriaSML[pc++] = 35 * 1000 + dirOp2;

                    memoriaSML[pc++] = 21 * 1000 + dirDest; // STORE
                }
            }
            else if (comando == "if") {
                string op1, rel, op2, gotoCmd, destino;
                streamLinea >> op1 >> rel >> op2 >> gotoCmd >> destino;

                int dirOp1 = buscarSimbolo(op1, 'V');
                if (dirOp1 == -1) dirOp1 = buscarSimbolo(op1, 'C');

                int dirOp2 = buscarSimbolo(op2, 'V');
                if (dirOp2 == -1) dirOp2 = buscarSimbolo(op2, 'C');

                int dirDest = buscarSimbolo(destino, 'L');

                memoriaSML[pc++] = 20 * 1000 + dirOp1; 
                memoriaSML[pc++] = 31 * 1000 + dirOp2; 

                if (rel == "==") {
                    memoriaSML[pc++] = 42 * 1000 + dirDest; 
                } else if (rel == "<") {
                    memoriaSML[pc++] = 41 * 1000 + dirDest; 
                } else if (rel == ">") {
                    pc--; 
                    memoriaSML[pc - 1] = 20 * 1000 + dirOp2; 
                    memoriaSML[pc++] = 31 * 1000 + dirOp1;   
                    memoriaSML[pc++] = 41 * 1000 + dirDest;  
                } else if (rel == "<=") {
                    memoriaSML[pc++] = 41 * 1000 + dirDest; 
                    memoriaSML[pc++] = 42 * 1000 + dirDest; 
                } else if (rel == ">=") {
                    pc--;
                    memoriaSML[pc - 1] = 20 * 1000 + dirOp2;
                    memoriaSML[pc++] = 31 * 1000 + dirOp1;
                    memoriaSML[pc++] = 41 * 1000 + dirDest;
                    memoriaSML[pc++] = 42 * 1000 + dirDest;
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
                           << setfill('0') << setw(4) << abs(memoriaSML[i]) << "\n";
                }
            }
            salida.close();
            cout << "[OK] Archivo SML generado: " << nombreArchivo << "\n";
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
        compiler.primeraPasada(codigoCompleto);
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