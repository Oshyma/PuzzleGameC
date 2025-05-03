#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "game.h"

// Função para terminar o jogo
int quit() {
    return 0;
}

// Função para inicializar o histórico de movimentos
Moves* initMoves() {
// Aloca memória para o histórico de movimentos
    Moves *moves = malloc(sizeof(Moves));
    moves->capacidade = 10; // Define a capacidade inicial
    moves->tamanho = 0; // Define o tamanho inicial
    moves->lista = malloc(moves->capacidade * sizeof(Move)); // Aloca memória para a lista de movimentos
    return moves;
}

void freeMoves(Moves *moves) {
    free(moves->lista); // Libera a memória da lista de movimentos
    free(moves); // Libera a memória do histórico
}


// Função para adicionar um movimento ao histórico
void adicionarMove(Moves *moves, char tipo, char coluna, int linha, char valorAntigo, State estadoAntigo) {
  // Verifica se a capacidade do histórico foi atingida
    if (moves->tamanho >= moves->capacidade) {
        moves->capacidade *= 2; // Dobra a capacidade
        moves->lista = realloc(moves->lista, moves->capacidade * sizeof(Move)); // Realloca memória
    }
    // Cria um novo movimento e adiciona à lista
    Move novo = {tipo, coluna, linha, valorAntigo, estadoAntigo};
    moves->lista[moves->tamanho++] = novo;
}

/*
void printHistorico(Moves *historico) {
    printf("\n--- Histórico de Jogadas ---\n");
    for (int i = 0; i < historico->tamanho; i++) {
        Move m = historico->lista[i];
        printf("%d. [%c] %c%d (valor antigo: '%c', estado antigo: %d)\n",
               i + 1,
               m.tipo,
               m.coluna,
               m.linha,
               m.valorAntigo,
               m.estadoAntigo);
    }
    if (historico->tamanho == 0) {
        printf("Nenhuma jogada feita ainda.\n");
    }
    printf("-----------------------------\n");
}
*/

// WRAPPERS

// Função para carregar o jogo a partir de um ficheiro
void loadGame_wrapper(Game *game, char *args, Moves *historico) {
    (void)historico; // Parâmetro historico não é utilizado
    
    // Verifica se o argumento para carregar o jogo é válido
    if (args != NULL && args[0] != '\0') {
        loadGame(game, args); // Chama a função loadGame
    } else {
        printf("Argumentos inválidos para load. Especifica o nome do ficheiro.\n");
    }
}

// Função para salvar o jogo num ficheiro
void saveGame_wrapper(Game *game, char *args, Moves *historico) {
    (void)historico; // Parâmetro historico não é utilizado
    // Verifica se o argumento para salvar o jogo é válido
    if (args != NULL && args[0] != '\0') {
        saveGame(game, args); // Chama a função saveGame
    } else {
        printf("Argumentos inválidos para save. Especifica o nome do ficheiro.\n");
    }
}

// Função para pintar uma célula no tabuleiro
void paint_wrapper(Game *game, char *args, Moves *historico) {
    char coluna;
    int linha;
    // Verifica se os argumentos para o comando paint são válidos
    if (sscanf(args, " %c%d", &coluna, &linha) == 2) {
        int lin = linha - 1; // Ajusta o índice da linha
        int col = coluna - 'a'; // Ajusta o índice da coluna
        // Verifica se as coordenadas estão dentro dos limites do tabuleiro
        if (lin >= 0 && lin < game->linhas && col >= 0 && col < game->colunas) {
            char valorAntigo = game->tabuleiro[lin][col]; // Guarda o valor antigo
            State estadoAntigo = game->estado[lin][col]; // Guarda o estado antigo

            paint(game, coluna, linha); // Chama a função paint
            
            // Adiciona o movimento ao histórico
            adicionarMove(historico, 'b', coluna, linha, valorAntigo, estadoAntigo);
        }
    } else {
        printf("Argumentos inválidos para paint.\n");
    }
}

// Função para riscar uma célula no tabuleiro
void crossout_wrapper(Game *game, char *args, Moves *historico) {
    char coluna;
    int linha;
    // Verifica se os argumentos para o comando crossout são válidos
    if (sscanf(args, " %c%d", &coluna, &linha) == 2) {
        int lin = linha - 1; // Ajusta o índice da linha
        int col = coluna - 'a'; // Ajusta o índice da coluna
        // Verifica se as coordenadas estão dentro dos limites do tabuleiro
        if (lin >= 0 && lin < game->linhas && col >= 0 && col < game->colunas) {
            char valorAntigo = game->tabuleiro[lin][col]; // Guarda o valor antigo
            State estadoAntigo = game->estado[lin][col]; // Guarda o estado antigo

            crossout(game, coluna, linha); // Chama a função crossout
            
            // Adiciona o movimento ao histórico
            adicionarMove(historico, 'r', coluna, linha, valorAntigo, estadoAntigo);
        }
    } else {
        printf("Argumentos inválidos para crossout.\n");
    }
}

// Função para restaurar o jogo a partir do histórico
void restore_wrapper(Game *game, char *args, Moves *historico) {
    (void)args; // Parâmetro args não é utilizado
    restore(game, historico); // Chama a função restore
}

// Função para verificar o estado atual do jogo
void verify_wrapper(Game *game, char *args, Moves *historico) {
  (void)historico; // Parâmetro historico não é utilizado
  (void)args; // Parâmetro args não é utilizado
    verify(game); // Chama a função verify
}

void help_wrapper(Game *game, char *args, Moves *historico) {
    (void)args; // Parâmetro args não é utilizado
    (void)historico; // Parâmetro historico não é utilizado
    help(game); // Chama a função help
}

void autohelp_wrapper(Game *game, char *args, Moves *historico) {
    (void)args; // Parâmetro args não é utilizado
    (void)historico; // Parâmetro historico não é utilizado
    autohelp(game); // Chama a função autohelp
}

void solveGame_wrapper(Game *game, char *args, Moves *historico) {
    (void)args; // Parâmetro args não é utilizado
    solveGame(game, historico); // Chama a função solveGame
}
// MAIN FUNCS
// Função para processar os comandos do jogo
void commandProcess(Game *game, char *input, Moves *historico) {

    // Define os comandos disponíveis
    const Command comandos[] = {
        { 'b', paint_wrapper, 1 }, // Comando para pintar uma célula
        { 'r', crossout_wrapper, 1 }, // Comando para riscar uma célula
        { 'l', loadGame_wrapper, 1 }, // Comando para carregar um jogo
        { 'g', saveGame_wrapper, 1 }, // Comando para salvar o jogo
        { 'v', verify_wrapper, 0 }, // Comando para verificar o estado do jogo
        { 'a', help_wrapper, 0 }, // Comando para ajudar a resolver o jogo
        { 'A', autohelp_wrapper, 0 }, // Comando para ajudar automaticamente
        { 'R', solveGame_wrapper, 0 }, // Comando para resolver o jogo
        { 'd', restore_wrapper, 0 }, // Comando para restaurar o jogo
    };    

    int num_comandos = sizeof(comandos) / sizeof(Command); // Calcula o número de comandos disponíveis

    // Separa o comando e os argumentos da linha de entrada
    char letra;
    char args[100] = "";
    sscanf(input, " %c %[^\n]", &letra, args); // Lê o comando e os argumentos

    // Processa o comando
    for (int i = 0; i < num_comandos; i++) {
        if (comandos[i].letra == letra) {
        // Verifica se o comando requer argumentos
            if (comandos[i].needs_args && args[0] == '\0') {
                printf("O comando '%c' requer argumentos.\n", letra);
                return;
            }
            comandos[i].funcao(game, args[0] != '\0' ? args : NULL, historico); // Chama a função associada ao comando
            //printHistorico(historico);
            return;
        }
    }
    
    printf("Comando '%c' não reconhecido.\n", letra);
}

int main() {
    char input[100];
    int running = 1;
    // Inicializa o jogo com um tabuleiro 2x2
    Game* game = initializeGame(2, 2);
    Moves *historico = initMoves(); // Inicializa o histórico de movimentos
    printTab(game); // Imprime o tabuleiro inicial

    // Loop principal do jogo
    while (running) {
        printf("> ");
        fgets(input, sizeof(input), stdin); // Lê a entrada do utilizador
        
        // Verifica se o comando é para sair do jogo
        if (input[0] == 's') {
            running = quit(); // Sai do jogo
            continue;
        }

        commandProcess(game, input, historico); // Processa o comando
        printTab(game); // Imprime o estado atual do tabuleiro
    }


    freeGame(game); // Libera a memória do jogo
    freeMoves(historico); // Libera a memória do histórico
    return 0;
}