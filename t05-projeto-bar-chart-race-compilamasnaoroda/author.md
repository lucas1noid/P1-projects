# Introduction

Este programa gera um gráfico animado no terminal que mostra a evolução temporal de um fenômeno, como a expectativa de vida nos países(nosso caso), além de qualquer outro banco de dados cedido ao programa, como o crescimento populacional, temperatura média, vendas mensais ou outro dado, portanto que varie ao longo do tempo. O programa lê uma base de dados com registros por ano ou/e mês e cria uma animação interativa que permite visualizar claramente as mudanças ao longo dos períodos. A animação destaca picos e variações, tornando a análise mais intuitiva e atrativa para o usuário.

# Author(s)

Lucas Gabriel da Silva Lopes lucas1noid@gmail.com
Osli Fernandes Pereira de Oliveira osli.pereira.106@ufrn.edu.br


# Problems found or limitations

Tivemos dificuldade de printar o eixo X da forma correta, cobrindo possíveis exceções. Tivemos que buscar ajuda com amigos, 
inteligência artificial e pesquisas em sites como cplusplus.com.

Tornar o programa genérico também foi difícil, tendo em vista que, inicialmente, queríamos utilizar um arquivo .csv que era
muito diferente dos arquivos disponibilizados pelo professor. Todos os nossos testes foram baseados no arquivo cities.txt, então 
pode-se garantir que o programa funciona para arquivos similares.

Quando a cidade são paulo aparece no gráfico ele fica 1 espaço recuado em comparação com o restante das cidades, creio que devido ao acento em 'ã', ja que a função analisa os bytes reservados e não o tamanho printado da string. não afeta necessariamente o programa, porém é um erro chatinho.

# Grading


Preencha os itens que você implementou, indicando qual a nota esperada para cada item.


Item     | Valor máximo   | Valor esperado
-------- | :-----: | :-----:
Read, validate, and process command line arguments | 5 | 5
Program runs with default values, if no options are provided | 5 | 5
Read the input data file and store the information in memory    | 10 | 10
Display a summary of the data read from the input file _before_ the animation starts | 5 | 5
Display a summary of the running options (set by the user or default values) _before_ the animation starts | 5 | 5
The project contains at least two classes | 10 | 10
Create individual bar charts containing all data items for a single date | 10 | 10
Run the bar chart animation at the correct speed | 10  | 10
The bar chart anatomy is complete and correct | 30 | 27
Program handles errors properly | 10 | 10

# Compiling and Runnig

para compilar o programa usando cmake:
cmake --build ./source/build/ 

para rodar o programa da pasta principal:
./source/build/bcr -b <* quantidade de barras *> -f <* quantidade de fps *> ./source/libs/cities.txt 

caso altere o libs, também alterar o parameters.ini com os respectivos dados.

