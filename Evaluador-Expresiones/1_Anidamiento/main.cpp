#include <iostream>
#include <string>
#include <stack>
#include <algorithm>

using namespace std;


void calcularProfundidad(const string& expresion) {
    stack<char> pila;
    int profundidadActual = 0;
    int profundidadMaxima = 0;
    bool error = false;

    cout << "\nAnalisis de la expresion paso a paso:\n";
    cout << expresion << "\n";

    for (size_t i = 0; i < expresion.length(); ++i) {
        char c = expresion[i];
        
        // Si es un símbolo de apertura, incrementamos profundidad
        if (c == '(' || c == '[' || c == '{') {
            pila.push(c);
            profundidadActual++;
            profundidadMaxima = max(profundidadMaxima, profundidadActual);
            cout << profundidadActual << " ";
        }

        else if (c == ')' || c == ']' || c == '}') {
            if (pila.empty()) {
                error = true;
                cout << "X ";
                continue;
            }
            
            char tope = pila.top();
            if ((c == ')' && tope == '(') || 
                (c == ']' && tope == '[') || 
                (c == '}' && tope == '{')) {
                pila.pop();
                cout << profundidadActual << " ";
                profundidadActual--;
            } else {
                error = true;
                cout << "X ";
            }
        } else {

            cout << profundidadActual << " ";
        }
    }
    cout << "\n\n";

    if (error || !pila.empty()) {
        cout << "Resultado: La expresion NO esta balanceada correctamente.\n";
    } else {
        cout << "Resultado: Expresion balanceada correctamente.\n";
        cout << "Profundidad maxima de anidamiento: " << profundidadMaxima << "\n";
    }
}

int main() {
    string expresion;
    cout << "--- Evaluador de Expresiones: Profundidad de Anidamiento ---\n";
    cout << "Introduce la expresion a evaluar: ";
    getline(cin, expresion);

    calcularProfundidad(expresion);
    return 0;
}