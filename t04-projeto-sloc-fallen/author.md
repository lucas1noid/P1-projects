# Introduction

Este projeto é uma ferramenta de linha de comando que analisa arquivos de código-fonte C/C++. O programa calcula e reporta o número de linhas de código, de comentários (incluindo documentação Doxygen) e de linhas em branco além de processar tanto arquivos individuais quanto diretórios inteiros, possuindo uma opção para buscar recursivamente por arquivos em todas as subpastas e ordenar os resultados, tudo de acordo com as preferências do usuário.

# Author(s)

Lucas Gabriel da Silva Lopes (lucas1noid@gmail.com)
Samuel Jacome de Oliveira (s4mueljacome@outlook.com)

# Problems found or limitations

Vamos relatar mais feedbacks do que realmente problemas a seguir:

Pequeno deficit no conhecimento de bibliotecas de leitura de arquivos e como trabalhar corretamente com tais dados, precisando recorrer algumas vezes a ajuda de colegas mais experientes ou pesquisas na internet.

Tivemos certa dificuldade com a função iomanip e seus recursos, visto que era quase impossível não utilizar ela para a saida do programa, mas após estudar-la deu certo.
A maquina de estados foi de longe a parte mais problemática do código, pois a cada feature apareciam novos bugskkkk, mas acreditamos que o sanamos a grande maioria deles.

Lista aqui os problemas encontrados (se houve algum) e as possíveis limitações do programa.

# Grading

Preencha os itens que você implementou, indicando qual a nota esperada para cada item.

| Item                                                | Valor máximo | Valor esperado |
| --------------------------------------------------- | :----------: | :------------: |
| Read one or multiple source file                    |      10      |       10       |
| Read a list of files from a directory               |      10      |       10       |
| Read a list of files recursively from a directory   |      8       |        8       |
| Show help as requested `-h`                         |      4       |        4       |
| Treat error situations                              |      10      |       10       |
| Show correct number of blank lines                  |      5       |        5       |
| Show correct number of comment lines                |      10      |       10       |
| Show correct number of doc comment lines            |      10      |       10       |
| Show correct number of code lines                   |      10      |       10       |
| Show correct number of total lines                  |      5       |        5       |
| Show the type of source file                        |      5       |        5       |
| Show percentages as requested                       |      10      |       10       |
| Show consolidation at the bottom of a table         |      5       |        5       |
| Table is formatted as requested                     |      8       |        8       |
| The project contains at least one struct or classes |      5       |        5       |
| Overall evaluation of the program                   |      5       |        5       |

# Compiling and Running

Para compilar, utilize: com Cmake: cmake -S . -B build (!4.0 ou superior!) cmake --build build

com g++: g++ validate.cpp main.cpp sloc.cpp funcs.cpp -o sloc

com Clang: Clang -Wall validate.cpp main.cpp sloc.cpp funcs.cpp -o sloc

Para rodar: com Cmake: ./build/sloc (COMANDOS PARA O CODIGO)

com g++ ou Clang: ./sloc (COMANDOS PARA O CODIGO)

atenciosamente, Lucas e Samuel.
