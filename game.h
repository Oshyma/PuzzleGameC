#ifndef GAME_H
#define GAME_H

#include <stdbool.h>

typedef enum {
    NORMAL,
    BRANCA,
    RISCADA
} State;

typedef struct {
    int linhas;
    int colunas;
    char **tabuleiro;
    State **estado;
} Game;

typedef struct {
    char tipo;       
    char coluna;     
    int linha;       
    char valorAntigo; 
    State estadoAntigo; 
} Move;

typedef struct {
    Move *lista;    
    int tamanho;    
    int capacidade; 
} Moves;



typedef void (*CommandFunction)(Game *game, char *args, Moves *moves);

typedef struct {
    char letra;
    CommandFunction funcao;
    int needs_args;
} Command;

Game* initializeGame(int linhas, int colunas);
void freeGame(Game *game);
void printTab(Game *game);
void paint(Game *game, char coluna, int linha);
void crossout(Game *game, char coluna, int linha);
void restore(Game *game, Moves *historico);
void verify(Game *game);
void loadGame(Game *game, const char *ficheiro);
void saveGame(Game *game, const char *ficheiro);
bool areAllWhiteCellsConnected(Game *game);

#endif
