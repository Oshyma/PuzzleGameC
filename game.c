#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "game.h"


Game initializeGame() {
    Game game;
    game.linhas = 2; // Exemplo de inicialização
    game.colunas = 2; // Exemplo de inicialização
    game.tabuleiro[0][0] = 'a'; // Exemplo de inicialização
    game.tabuleiro[0][1] = 'b'; // Exemplo de inicialização
    game.tabuleiro[1][0] = 'c'; // Exemplo de inicialização
    game.tabuleiro[1][1] = 'd'; // Exemplo de inicialização
    return game;
}

void printTab(char tabuleiro[max][max], int linhas, int colunas) {
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            printf("%c ", tabuleiro[i][j]);
        }
        printf("\n");
    }
}

void saveGame(Game *game, const char *ficheiro) {
    FILE *f = fopen(ficheiro, "w");
    if (!f) {
        perror("Erro ao abrir ficheiro para escrita");
        return;
    }

    fprintf(f, "%d %d\n", game->linhas, game->colunas);
    for (int i = 0; i < game->linhas; i++) {
        fprintf(f, "%s\n", game->tabuleiro[i]);
    }

    fclose(f);
    printf("Jogo salvo com sucesso no ficheiro \"%s\".\n", ficheiro);
}

void loadGame(Game *game, const char *ficheiro) {
    FILE *f = fopen(ficheiro, "r");
    if (!f) {
        perror("Erro ao abrir ficheiro");
        return;
    }

    fscanf(f, "%d %d\n", &game->linhas, &game->colunas);
    for (int i = 0; i < game->linhas; i++) {
        fgets(game->tabuleiro[i], max, f);
        game->tabuleiro[i][strcspn(game->tabuleiro[i], "\n")] = '\0';
    }

    fclose(f);
    printf("Jogo carregado com sucesso do ficheiro \"%s\".\n", ficheiro);
}

void paint(Game *game, char coluna, int linha) {
    int col = coluna - 'a';
    if (linha > 0 && linha <= game->linhas && col >= 0 && col <= game->colunas) {

        int lin = linha - 1;
        char c = game->tabuleiro[lin][col];
        if(c == '#') {
            printf("Coordenadas inválidas!\n");
            return;
        }
        if (islower(c)) {
            game->tabuleiro[lin][col] = toupper(c);
            game->estado[lin][col] = BRANCA;
       }
       printf("Pinta na coluna %c, linha %d\n", coluna, linha);
    } else {
        printf("Coordenadas inválidas!\n");
    }
}


void crossout(Game *game, char coluna, int linha){
    // Implementação da função crossout
    int col = coluna - 'a';
    int lin = linha - 1;
    if (linha > 0 && linha <= game->linhas && col >= 0 && col <= game->colunas) {
        printf("Riscada na coluna %c, linha %d\n", coluna, linha);
            game->tabuleiro[lin][col] = '#';
            game->estado[lin][col] = RISCADA;
    } else {
        printf("Coordenadas inválidas!\n");
    }
}