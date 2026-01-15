[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/uGfXXTGv)
# Projeto Password Generator

<!--toc:start-->

- [1. Introdução](#1-introdução)
- [2. Funcionamento](#2-funcionamento)
  - [2.1 Considerações sobre o tamanho da senha](#21-considerações-sobre-o-tamanho-da-senha)
  - [2.2 Considerações sobre a escolha de vários caracteres de um mesmo grupo](#22-considerações-sobre-a-escolha-de-vários-caracteres-de-um-mesmo-grupo)
- [3. Entrada e Interface](#3-entrada-e-interface)
  - [3.1 Qualidade da senha](#31-qualidade-da-senha)
  - [3.2 Tratamento de erros](#32-tratamento-de-erros)
- [4. Modelagem do problema](#4-modelagem-do-problema)
- [5. Avaliação](#5-avaliação)
  - [5.1 Extras](#51-extras)
- [6. Boas práticas de programação](#6-boas-práticas-de-programação)
- [7. Autoria e Política de Colaboração](#7-autoria-e-política-de-colaboração)
- [8. Entrega](#8-entrega)
<!--toc:end-->

## 1. Introdução

Neste projeto de programação você deve desenvolver um programa gerador de senhas: `passgen`.
O objetivo do programa é produzir senhas de complexidade e tamanho variados, dependendo das escolhas de configuração do usuário.

O programa deverá oferecer uma interface de interação com o usuário no estilo CLI (_Command Line Interface_).

Nas próximas seções vamos descrever a especificação do projeto, detalhando sua interface
e _features_ que devem ser implementadas.

## 2. Funcionamento

O programa `passgen` recebe como entrada do usuário o tamanho da senha desejada junto com uma indicação de quais
**grupos** de caracteres o gerador deve usar para escolher os caracteres que vão compor a senha.
O gerador escolhe, pelo menos, um caractere de forma aleatória de cada grupo indicado.

Os grupos de caracteres são os seguintes:

| Grupo                       | Elementos       |
| --------------------------- | --------------- |
| letras minúsculas (_lower_) | a-z             |
| letras maiúsculas (_upper_) | A-Z             |
| dígitos (_digits_)          | 0-9             |
| _logograms_                 | # $ % & @ ^ ` ~ |
| pontuação (_punctuation_)   | . , ; :         |
| aspas (_quotes_)            | " '             |
| barras (_slashes_)          | \ / \| \_ -     |
| matemáticos (_math_)        | < > \* + ! ? =  |
| parênteses (_braces_)       | } { ] [ ) (     |

### 2.1 Considerações sobre o tamanho da senha

O tamanho _default_ de uma senha é 7, caso o usuário não indique um tamanho manualmente.

O programa deve procurar escolher, pelo menos, um caractere de cada grupo indicado.
Em outras palavras, todos os grupos indicados pelo usuário **devem** contribuir com algum caractere para a senha final.

Se o tamanho da senha solicitado for **maior** do que a quantidade de grupos
indicados, o gerador deve escolher mais de um caractere por grupo.
Por outro lado, se o tamanho da senha solicitado for **menor** do que a
quantidade de grupos indicada, o tamanho da senha deverá ser alterado de
maneira a escolher pelo menos um caractere de cada grupo.

Contudo, se o usuário ativar a opção de excução para _forçar_ o tamanho da senha desejado, o programa
deve sempre prover uma senha do tamanho desejado, mesmo que seja **menor** que a quantidade de grupos ativos.
Neste caso (tamanho da senha < quantidade de grupos selecionados), o programa deve escolher aleatoriamente quais grupos vão contribuir para a senha.

### 2.2 Considerações sobre a escolha de vários caracteres de um mesmo grupo

Suponha que o programa precisa selecionar `n` caracteres de um grupo com, digamos, tamanho `m`, com `m >= n`.
Seu programa **deve** garantir que os `n` caracteres escolhidos sejam distintos.

Por exemplo, se `n=3` e o grupo escolhido fosse o de "_parênteses_" (`--braces`), com tamanho `m=6`;
uma seleção aceitável de caracteres deste grupo seria `{ [ ]`, enquanto que `{ { )` não seria aceitável visto que `{` aparece 2 vezes neste subconjunto.

Por outro lado, se `n>m` é natural que ocorra repetições na seleção de caracteres deste grupo.
Mas mesmo neste caso, procure reduzir ao máximo a quantidade de repetições.
Considerando o exemplo anterior com `n=9`, um exemplo aceitável seria `) { ] ( [ { ( } [`, enquanto que `{ ) { { { { ] ) )` seria inaceitável, visto que o `{` é repetido múltiplas vezes (desnecessariamente).

## 3. Entrada e Interface

A seguir temos alguns exemplos de execução do `passgen` com algumas opções.

Na execução abaixo o programa exibe a tela de ajuda, que também pode ser gerada com a opção `--help`

```
$ ./passgen -h
-- Welcome to passgen v 1.0, © DIMAp 2025 --

This program generates passwords of specific length
based on the selection of groups of characters.

Usage: passgen [<options>]
  --len n            Size of the password (default is 7).
  -f, --force        Force program to preserve the requested password length,
                     even if the number of groups requested is greater than
                     the password length.
  -l, --lower        Add random characters from [a-z].
  -u, --upper        Add random characters from [A-Z].
  -d, --digits       Add random characters from [0-9].
  -o, --logograms    Add random characters from [#$%&@^`~].
  -p, --punctuation  Add random characters from [.,;:].
  -q, --quotes       Add random characters from ["'].
  -s, --slashes      Add random characters from [\/|_-].
  -m, --math         Add random characters from [*+!?=].
  -b, --braces       Add random characters from [{}[]()].
  -a, --all-groups   Add random characters from all the above groups.
  -v, --verbose      Prints extra information besides the password.
  -h, --help         Show this help screen.
```

A execução abaixo gera uma senha de 10 caracteres, utilizando apenas letras minúsculas e maiúsculas (modo _verbose_ ativo).

```
$ ./passgen --len 10 --lower --upper
-- Welcome to passgen v 1.0, © DIMAp 2025 --

>>> The generated password is:
IrTJcmMUnh

>>> Password quality: good.
    Entropy: 57 bits.
```

A execução abaixo gera uma senha de 20 caracteres, misturando vários grupos de
caracteres disponíveis, sem exibir qualquer outra informação além da própria
senha.
Note que deve ser possível especificar vários grupos combinando as iniciais
dos grupos, `-ludopqs`, ou indicando o nome completo do grupo `--math --braces`.

A indicação do mesmo grupo múltiplas vezes não deve ser um problema; tal grupo
deve ser considerado apenas uma vez.

```
$ ./passgen --len 20 -ludopqsm --math --braces
f`8G2v"*(@>,C{-{/.?'
```

A execução a seguir gera uma senha de 15 caracteres com todos os grupos presentes e exibe a qualidade da senha.

```
$ ./passgen --len 15 --all-groups --verbose
-- Welcome to passgen v 1.0, © DIMAp 2025 --

>>> The generated password is:
':t#<_4,"\=A{`)

>>> Password quality: strong.
    Entropy: 98 bits.
```

> [!important]
> Se o usuário não fornecer um grupo de caracteres o programa deve selecionar caracteres do grupo de letras minúsculas apenas.

### 3.1 Qualidade da senha

Seu programa deve avaliar a senha gerada com base na **entropia** da senha.
O resultado deve ser uma classificação qualitativa em 5 categorias, a saber:

- _Very Weak_: senhas com entropia até 25 bits.
- _Weak_: senhas com entropia até entre 26 e 45 bits.
- _Good_: senhas com entropia entre 46 e 80 bits.
- _Strong_: senhas com entropia entre 81 e 100 bits.
- _Very Strong_: senhas com entropia entre 101 e 120 bits.
- _Excellent_: senhas com entropia acima de 120 bits.

A **entropia** de uma senha pode ser calculada com a equação $E=\log_2(R^L)$, onde

- $E$: o valor da entropia em bits.
- $R$: a quantidade total de caracteres distintos, considerando cada grupo de caracters escolhido para compor a senha.
- $L$: o comprimento da senha.

Note que $R^L$ corresponde ao tamanho do espaço de todas as senhas possíveis que podem ser geradas com
o comprimento de senha solicitado, considerando a quantidade de caracteres distintos que podem ser escolhidos para a senha.

Cada dupla está livre para modificar esta classificação da forma que achar mais adequado, desde que seja um critério razoável.

> [!tip]
> Faça uma investigação sobre o assunto para saber como classificar a qualidade de uma senha. Em particular, investigue a definição do conceito de **entropia** em senhas.

### 3.2 Tratamento de erros

Seu programa deve tratar erros de entrada do usuário, indicando uma mensagem indicativa do problema, seguido da tela de ajuda padrão.

São exemplos (não exaustivos) de erros de entrada:

- Usar uma opção inválida.
- Usar a opção `--len` mas esquecer de indicar um inteiro (argumento da opção).
- Indicar um comprimento inválido, como por exemplo zero ou um número negativo.
- Indicar quando o usuário usou o `-` mas não incluiu a letra correta.

> [!important]
> Seu programa deve receber os argumentos em qualquer ordem.

## 4. Modelagem do problema

Seguem algumas dicas e sugestões de modelagem do problema, bem como algumas boas práticas de programação.

1. Crie um `struct` chamado de `RunningOptions` onde você deve salvar o resultado do processamento do [**argumentos de linha de comando**](https://www.learncpp.com/cpp-tutorial/command-line-arguments/) fornecidos ao programa.
2. Implemente um conjunto de funções ou uma classe que simule o comportamento de um [dado aleatório de `n` faces](https://www.dados-online.pt/).
   Tal componente pode ser muito útil, visto que selecionar elementos de um conjunto de caracteres pode ser interpretado como a rolagem de um dado aleatório com `n` faces, onde `n` é o número de componentes do conjunto.
3. Ao coletar os caracteres de cada grupo, guarde-os em uma _string_ do tamanho da senha e, por fim, aplique a operação
   _shuffle_ (embaralhamento) para garantir que os caracteres vindo de cada grupo serão misturados dentro da senha.
4. Procure criar funções pequenas e simples, que desempenham apenas uma única tarefa.
5. Use enumeração para representar os 9 grupos de caracteres. Isso torna seu código mais legível.
6. A utilização da estrutura de dados **dicionário** (ex. _hash table_ ou _map_) pode facilitar alguns aspectos da implementação.
7. Procure utilizar as funções da biblioteca [`<algoritm>`](https://en.cppreference.com/w/cpp/algorithm) do C++ sempre que possível.

## 5. Avaliação

Seu programa deve ser escrito em um bom estilo de programação.
Isto inclui (mas não fica limitado a) o uso de nomes significativos
para identificadores e funções, um cabeçalho de comentário no início
de cada arquivo, cabeçalho no formato Doxygen para cada função/método criado,
uso apropriado de linhas em branco e identação para ajudar na visualização
do código, comentários significativos nas linhas de código, etc.

<!-- Para a implementação deste projeto **é obrigatório** a utilização de pelo menos uma classe. -->

O programa completo deverá ser entregue sem erros de compilação, testado e totalmente
documentado.
O projeto é composto de **90 pontos** e será avaliado sob os seguintes critérios:-

| Item                                             | Pontos |
| ------------------------------------------------ | :----: |
| Interface de acordo com o especificado           |   20   |
| Seleciona caracteres dos grupos adequadamente    |   15   |
| Apresenta a tela de ajuda quando solicitado      |   5    |
| Trata os erros de entrada corretamente           |   12   |
| Avalia a qualidade da senha com 5 categorias     |   10   |
| Implementa os 9 grupos de caracteres solicitados |   18   |
| Utiliza corretamente a aleatoriedade             |   10   |

A pontuação acima não é definitiva e imutável.
Ela serve apenas como um guia de como o trabalho será avaliado em linhas gerais.
É possível a realização de ajustes nas pontuações indicadas visando adequar a pontuação
ao nível de dificuldade dos itens solicitados.

Os itens abaixo correspondem à descontos, ou seja, pontos que podem ser retirados
da pontuação total obtida com os itens anteriores:-

| Item                                                             | Value (points) |
| ---------------------------------------------------------------- | :------------: |
| Presença de erros de compilação                                  |    até -10     |
| Falta de documentação das funções e/ou classes no estilo Doxygen |    até -10     |
| Vazamento de memória                                             |      -10       |
| Não preencher o arquivo [`author.md`](./author.md)               |      -10       |
| O programa quebra ou falha durante a execução                    |       -5       |

O arquivo [`author.md`](docs/author.md) deve conter uma breve descrição do projeto e como compilar o projeto.
Também deve descrever eventuais limitações do projeto e dificuldades de aprendizagem enfrentadas.
Não esqueça de incluir os nomes dos componentes da dupla!

### 5.1 Extras

Os programas que **estiverem funcionando corretamente e completamente**, podem tentar obter ponto extra
implementando alguns dos elementos adicionais sugeridos a seguir:

1. Salvar em um arquivo criptografado informações como _local_ + _usuário_ + _senha_. Claro, é necessário pensar em uma maneira de recuperar as informações salvas.
2. Utilizar classes na modelagem do seu programa.
3. Criar um arquivo de configuração externa no formato [.ini](https://en.wikipedia.org/wiki/INI_file), de maneira a facilitar a configuração do gerador de senhas de forma persistente entre execuções.

Se você tiver alguma outra ideia interessante de extensão do projeto, compartilhe no servidor Discord.

## 6. Boas práticas de programação

Recomenda-se fortemente o uso das seguintes ferramentas:-

- Doxygen: para a documentação de código e das classes;
- Git: para o controle de versões e desenvolvimento colaborativo;
- Valgrind: para verificação de vazamento de memória;
- gdb: para depuração do código; e
- CMake/Makefile: para gerenciar o processo de compilação do projeto.

## 7. Autoria e Política de Colaboração

O trabalho pode ser realizado **individualmente** ou em **dupla**,
sendo que no último caso é importante, dentro do possível,
dividir as tarefas igualmente entre os componentes.
A divisão de tarefas deve ficar evidente através do histórico de _commit_ do
repositório git correspondente.

Após a entrega, qualquer equipe pode ser convocada para uma entrevista.
O objetivo da entrevista é duplo: confirmar a autoria do trabalho e
determinar a contribuição real de cada componente em relação ao trabalho.
Durante a entrevista os membros da equipe devem ser capazes de explicar,
com desenvoltura, qualquer trecho do trabalho, mesmo que o código tenha
sido desenvolvido pelo outro membro da equipe.
Portanto, é possível que, após a entrevista, ocorra redução da nota geral do trabalho ou
ajustes nas notas individuais, de maneira a refletir a verdadeira contribuição
de cada membro, conforme determinado na entrevista.

O trabalho em cooperação entre alunos da turma é estimulado.
É aceitável a discussão de ideias e estratégias.
Note, contudo, que esta interação **não** deve ser entendida como permissão
para utilização de código ou parte de código de outras equipes,
o que pode caracterizar a situação de plágio.
Em resumo, tenha o cuidado de escrever seus próprios programas.

Trabalhos plagiados ou realizados com IA generativa (ou qualquer variação) receberão nota **zero** automaticamente,
independente de quem seja o verdadeiro autor dos trabalhos infratores.
Fazer uso de qualquer assistência sem reconhecer os créditos apropriados
é considerado **plágio**.
Quando submeter seu trabalho, forneça a citação e reconhecimentos necessários.
Isso pode ser feito pontualmente nos comentários no início do código, ou,
de maneira mais abrangente, no arquivo texto `autor.md`.
Além disso, no caso de receber assistência, certifique-se de que ela lhe
é dada de maneira genérica, ou seja, de forma que não envolva alguém ou um serviço
tendo que escrever código por você.

## 8. Entrega

Você deve submeter seu trabalho de duas formas:
via GitHub Classroom (GHC) e via tarefa do SIGAA.

No SIGAA inclua apenas o link para o repositório no GHC.
No GHC você deve _subir_ seu projeto até antes do prazo final da tarefa.

Caso você opte por não usar o GHC por algum motivo, envie pelo SIGAA
um arquivo compactado com todo código fonte completo do seu projeto.

Em qualquer uma das duas opções descritas, lembre-se de remover todos
os arquivo executáveis do seu projeto (a pasta `build`) antes
de submeter seu trabalho.

> [!important]
> No caso de trabalhos implementados em duplas **os dois componente** devem
> submeter o trabalho via SIGAA. O aluno que não enviar o trabalho pelo
> SIGAA ficará sem nota!
