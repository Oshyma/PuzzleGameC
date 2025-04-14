#ifndef GAME_H
#define GAME_H

#define max 100

typedef enum {
    NORMAL,
    BRANCA,
    RISCADA
} State;

typedef struct {
    int linhas;
    int colunas;
    char tabuleiro[max][max];
    State estado[max][max];
    } Game;

typedef void (*CommandFunction)(Game *game, char *args);

typedef struct {
    char letra;
    CommandFunction funcao;
    int needs_args;
} Command;


Game initializeGame();
void printTab(char tabuleiro[max][max], int linhas, int colunas);
void paint(Game *game, char coluna, int linha);
void crossout(Game *game, char coluna, int linha);
void loadGame(Game *game, const char *ficheiro);
void saveGame(Game *game, const char *ficheiro);

#endif
