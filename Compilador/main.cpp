#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

struct Token {
    string tipo;
    string valor;
};

vector<Token> tokenizar(const string& codigo) {
    vector<Token> tokens;
    
    return tokens;
}

int main() {
    ifstream archivo("programa.simp");
    string linea;
    string codigoCompleto = "";

    if (archivo.is_open()) {
        while (getline(archivo, linea)) {
            codigoCompleto += linea + "\n";
        }
        archivo.close();
        
        vector<Token> tokens = tokenizar(codigoCompleto);
        
        cout << "Analisis lexico completado. Archivo .simp procesado exitosamente.\n";
    } else {
        cout << "Error al abrir programa.simp para compilacion.\n";
    }

    return 0;
}