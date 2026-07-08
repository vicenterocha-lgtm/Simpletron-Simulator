#include <iostream>
#include <string>
#include <stack>
#include <cctype>

using namespace std;


int obtenerPrecedencia(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    if (op == '^') return 3;
    return 0;
}


string convertirAPostfijo(const string& infija) {
    stack<char> pila;
    string postfija = "";

    for (size_t i = 0; i < infija.length(); ++i) {
        char c = infija[i];


        if (isspace(c)) continue;


        if (isalnum(c)) {
            postfija += c;
        }

        else if (c == '(') {
            pila.push(c);
        }

        else if (c == ')') {
            while (!pila.empty() && pila.top() != '(') {
                postfija += pila.top();
                pila.pop();
            }
            if (!pila.empty()) pila.pop(); // Quitar '('
        }

        else {
            while (!pila.empty() && obtenerPrecedencia(pila.top()) >= obtenerPrecedencia(c)) {
                postfija += pila.top();
                pila.pop();
            }
            pila.push(c);
        }
    }


    while (!pila.empty()) {
        postfija += pila.top();
        pila.pop();
    }

    return postfija;
}

int main() {
    string infija;
    cout << "--- Conversor de Expresiones a Postfijo ---\n";
    cout << "Introduce la expresion en formato Infijo (ej. A+B*C): ";
    getline(cin, infija);

    string postfija = convertirAPostfijo(infija);
    cout << "Expresion en formato Postfijo: " << postfija << "\n";

    return 0;
}