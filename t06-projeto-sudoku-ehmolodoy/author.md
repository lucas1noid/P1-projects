# Introduction

Nosso projeto é uma implementação do clássico jogo Sudoku rodando em interface de linha de comando. Ele foi desenvolvido com foco no jogo clássico, porém com algumas funções a mais, que tornam a experiência do usuário agradável.

# Author(s)

Lucas Gabriel da Silva Lopes lucas1noid@gmail.com

Guilherme Henriuqe Silva de Melo  guilhermehsdm@gmail.com

# Problems found or limitations

<!-- TODO -->
Lista aqui os problemas encontrados (se houve algum) e as
possíveis limitações do programa.

ao invés de tratar o tabuleiro como um array preferimos usar diretamente uma matriz da biblioteca padrão da linguagem.

toda a doxygen do projeto foi feita nos hpp's.

criamos o limite de erros para elevar a dificuldade dos jogos e ter como o jogador perder também, porém posteriormente poderiam ser criados outros modos de jogo, por exemplo: fácil, dificil, hardcore... e etc.

# Grading

<!-- TODO -->
Preencha os itens que você implementou, indicando qual a nota esperada para cada item.


Item     | Valor máximo   | Valor esperado
-------- | :-----: | :-----:
Trata corretamente os argumentos de linha de comando | 5 pts | 5
Lê e armazena os vários puzzles contidos em um arquivo de entrada |5 pts| 5
Exibe corretamente a tela principal com o menu de ações |5 pts| 5
Permite a escolha dos vários puzzles lidos do arquivo  |5 pts| 5
Executa corretamente a ação de posicionamento de dígitos no tabuleiro (comando posicionar) identificação jogadas inválidas |11 pts| 11
Executa corretamente a remoção de dígitos posicionados pelo jogador (comando remover) |10 pts| 10
Executa corretamente o comando de desfazer ação |12 pts| 12
Executa corretamente o comando de verificação do tabuleiro |12 pts| 12
Salva corretamente, em arquivo externo, uma partida em andamento | 7 pts | 7
Carrega corretamente, de um arquivo externo, uma partida em andamento previamente salva e consegue retormar a partida | 8 pts | 8
Exibe as regras do jogo quando solicitado |5 pts| 5
Indica corretamente se uma partida finalizada foi vencida ou perdida |5 pts| 5
Apresenta uma interface com usuário igual ou similar a que foi especificada nesse documento |5 pts| 5
Apresenta uma validação robusta dos comandos textuais de ação fornecidos pelo usuário |5 pts| 5
Exibe corretamente a lista de dígitos disponíveis, conforme o progresso do jogo |5 pts| 5
Apresenta as indicações com `setas` da coordenada da célula que foi alvo da última ação |5 pts| 5
Apresenta um fluxo correto entre telas, com opção de retomar uma partida suspensa e solicitação de confirmação de finalização de partida, caso exista uma partida em andamento |5 pts| 5

# Compiling and Runnig

<!-- TODO -->
Indique aqui como compilar e executar seu programa.

com Cmake: 
Dentro da pasta src, digite:( cmake -S . -B build ) para criar a pasta build
Logo em seguida, compile com: ( cmake --build build )
E então entre em build e execute o programa (./sudoku) com suas preferências.
