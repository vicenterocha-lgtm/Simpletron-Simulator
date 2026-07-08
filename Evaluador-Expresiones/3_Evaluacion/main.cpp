#include <iostream>
#include <string>
#include <stack>
#include <cctype>
#include <cmath>

using namespace std;


int evaluarPostfija(const string& expresion) {
    stack<int> pila;

    for (size_t i = 0; i < expresion.length(); ++i) {
        char c = expresion[i];

        if (isspace(c)) continue;


        if (isdigit(c)) {
            pila.push(c - '0');
        }

        else {
            if (pila.size() < 2) {
                cout << "Error: Expresion postfija invalida.\n";
                return 0;
            }
            int operando2 = pila.top(); pila.pop();
            int operando1 = pila.top(); pila.pop();
            int resultado = 0;

            switch (c) {
                case '+': resultado = operando1 + operando2; break;
                case '-': resultado = operando1 - operando2; break;
                case '*': resultado = operando1 * operando2; break;
                case '/': 
                    if (operando2 == 0) {
                        cout << "Error: Division por cero.\n";
                        return 0;
                    }
                    resultado = operando1 / operando2; 
                    break;
                case '^': resultado = pow(operando1, operando2); break;
            }
            pila.push(resultado);
        }
    }

    return pila.empty() ? 0 : pila.top();
}

int main() {
    string postfija;
    cout << "--- Evaluador Numerico de Expresiones Postfijas ---\n";
    cout << "Introduce una expresion postfija (ej. 234*+ para 2+3*4): ";
    getline(cin, postfija);

    int resultado = evaluarPostfija(postfija);
    cout << "El resultado de la evaluacion es: " << resultado << "\n";

    return 0;
}