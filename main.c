#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "game.h"

int quit() {
    return 0;
}


// WRAPPERS
void loadGame_wrapper(Game *game, char *args) {
    if (args != NULL && args[0] != '\0') {
        loadGame(game, args);
    } else {
        printf("Argumentos inválidos para load. Especifica o nome do ficheiro.\n");
    }
}

void saveGame_wrapper(Game *game, char *args) {
    if (args != NULL && args[0] != '\0') {
        saveGame(game, args);
    } else {
        printf("Argumentos inválidos para save. Especifica o nome do ficheiro.\n");
    }
}

void paint_wrapper(Game *game, char *args) {
    char coluna;
    int linha;
    if (sscanf(args, " %c%d", &coluna, &linha) == 2) {
        paint(game, coluna, linha);
    } else {
        printf("Argumentos inválidos para paint.\n");
    }
}

void crossout_wrapper(Game *game, char *args) {
    char coluna;
    int linha;
    if (sscanf(args, " %c%d", &coluna, &linha) == 2) {
        crossout(game, coluna, linha);
    } else {
        printf("Argumentos inválidos para crossout.\n");
    }
}

//MAIN FUNCS
void commandProcess(Game *game,char *input) {

    // Array de todos os comandos disponíveis.
    const Command comandos[] = {
        { 'b', paint_wrapper, 1 },
        { 'r', crossout_wrapper, 1 },
        { 'l', loadGame_wrapper, 1 },
        { 'g', saveGame_wrapper, 1 },
        /*{ 'v', verify, 0 },
        { 'a', help, 0 },
        { 'A', enableHelp, 0 },
        { 'R', solve, 0 },
        { 'd', restore, 0 },*/
    };    

    int num_comandos = sizeof(comandos) / sizeof(Command);

    // Separar comando e argumentos
    char letra;
    char args[100] = "";
    sscanf(input, " %c %[^\n]", &letra, args);

    for (int i = 0; i < num_comandos; i++) {
        if (comandos[i].letra == letra) {
            if (comandos[i].needs_args && args[0] == '\0') {
                printf("O comando '%c' requer argumentos.\n", letra);
                return;
            }
            comandos[i].funcao(game, args[0] != '\0' ? args : NULL);
            return;
        }
    }
    printf("Comando '%c' não reconhecido.\n", letra);
}


int main() {
    char input[100];
    int running = 1;
    Game game = initializeGame();
    printTab(game.tabuleiro, game.linhas, game.colunas);
    while (running) {
        printf("> ");
        fgets(input, sizeof(input), stdin);

        if (input[0] == 's') {
            running = quit();
            continue;
        }

        commandProcess(&game, input);
        printTab(game.tabuleiro, game.linhas, game.colunas);
    }
    return 0;
}
