# Simulador Simpletron y Evaluador de Expresiones

Este repositorio contiene el desarrollo individual de dos proyectos clave para la materia: el Simulador de la Computadora Simpletron y el Evaluador de Expresiones Matematicas utilizando estructuras de datos (Pilas), junto con el Compilador del Lenguaje Simple a SML.

## Estructura del Repositorio

* **Simpletron/**: Contiene el codigo fuente del simulador en C++ (`simpletron.cpp`) y la bitacora de control de cambios.
* **Compilador/**: Contiene el codigo fuente del compilador (`main.cpp`) y su bitacora de desarrollo.
* **Evaluador-Expresiones/**: Contiene las tres fases del evaluador de expresiones (Anidamiento, Postfijo y Evaluacion).
* **programas 1-10 simple/**: Contiene los programas de prueba escritos en lenguaje Simple.

## Opcodes SML Soportados

| Opcode | Instruccion | Descripcion |
| :---: | :--- | :--- |
| **10** | `READ` | Lee una palabra desde la entrada hacia una ubicacion de memoria. |
| **11** | `WRITE` | Escribe una palabra desde la memoria hacia la pantalla. |
| **20** | `LOAD` | Carga una palabra desde la memoria hacia el acumulador. |
| **21** | `STORE` | Almacena una palabra desde el acumulador hacia la memoria. |
| **30** | `ADD` | Suma una palabra de la memoria al acumulador. |
| **31** | `SUBTRACT` | Resta una palabra de la memoria al acumulador. |
| **32** | `DIVIDE` | Divide el acumulador entre una palabra de la memoria. |
| **33** | `MULTIPLY` | Multiplica el acumulador por una palabra de la memoria. |
| **34** | `MODULUS` | Obtiene el residuo de la division en el acumulador. |
| **35** | `EXPONENT` | Eleva el acumulador a la potencia indicada. |
| **40** | `BRANCH` | Salto incondicional a una ubicacion especifica. |
| **41** | `BRANCHNEG` | Salto si el acumulador es negativo. |
| **42** | `BRANCHZERO` | Salto si el acumulador es cero. |
| **43** | `HALT` | Termina la ejecucion del programa. |

## Compilacion y Ejecucion

### 1. Compilar los programas con GCC (`g++`)

Abre la terminal en la raiz del proyecto y ejecuta:

```bash
g++ -o Compilador/compilador Compilador/main.cpp
g++ -o Simpletron/simpletron Simpletron/simpletron.cpp
