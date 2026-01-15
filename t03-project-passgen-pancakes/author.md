# Introduction

Código que gera senhas aleatórias com ate nove grupos de caracteres, sendo os grupos:
lower (a-z), upper (A-Z), digits (0-9), logograms (#$%&@^`~), punctuation (.,;:), quotes ('"),
slashes (\/|_-), math (*+!?=), braces({}[]()). Alem disso, o codigo indica tambem a entropia
da senha, definindo-a de very weak ate Excellent.

# Author(s)

Andre Ferreira Lima (andreferlima19@gmail.com)
Lucas Gabriel da Silva Lopes (lucas1noid@gmail.com)

# Problems found or limitations
Nossa maior dificuldade foi na função 'generate_password' da generator.cpp, pois
primeiro tentamos fazer com ponteiros porém no meio do caminho davam muitas coisas
erradas e por fim decidimos mudar para uma lógica mais simples e simplificada por 
passos. No entanto nos sentimos muito satisfeitos com o restante do projeto que além
de divertido foi muito proveitoso por nossa parte, já que aprendemos muitas novas funcoes
da linguagem como um todo.

# Grading

<!-- TODO -->

Preencha os itens que você implementou, indicando qual a nota esperada para cada item.

| Item                                             | Valor máximo | Valor esperado |
| ------------------------------------------------ | :----------: | :------------: |
| Interface de acordo com o especificado           |      20      |     20         |
| Seleciona caracteres dos grupos adequadamente    |      15      |     15         |
| Apresenta a tela de ajuda quando solicitado      |      5       |     5          |
| Trata os erros de entrada corretamente           |      12      |     12         |
| Avalia a qualidade da senha com 5 categorias     |      10      |     10         |
| Implementa os 9 grupos de caracteres solicitados |      18      |     18         |
| Utiliza corretamente a aleatoriedade             |      10      |     10         |

# Compiling and Runnig

Para compilar, utilize:
com Cmake:
cmake -S . -B build (!4.0 ou superior!) 
cmake --build build

com g++:
g++ validate.cpp main.cpp entropy.cpp generator.cpp -o passgen

com Clang:
Clang -Wall validate.cpp main.cpp entropy.cpp generator.cpp -o passgen

Para rodar:
com Cmake:
./build/passgen (COMANDOS PARA O CODIGO) 

com g++ ou Clang:
./passgen (COMANDOS PARA O CODIGO)
