#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include "game.h"

// Função que inicializa uma linha do tabuleiro com um caractere especificado
char* inicializaLinhaTabuleiro(int colunas, char caractere) {
    char* linha = malloc((colunas + 1) * sizeof(char));  // Aloca memória para a linha do tabuleiro
    if (!linha) return NULL;  // Verifica se a alocação falhou
    for (int j = 0; j < colunas; j++) {
        linha[j] = caractere;  // Preenche a linha com o caractere especificado
    }
    linha[colunas] = '\0';  // Adiciona o terminador de string
    return linha;
}

// Função que inicializa uma linha do estado do tabuleiro com o valor NORMAL
State* inicializaLinhaEstado(int colunas) {
    State* linha = malloc(colunas * sizeof(State));  // Aloca memória para a linha de estados
    if (!linha) return NULL;  // Verifica se a alocação falhou
    for (int j = 0; j < colunas; j++) {
        linha[j] = NORMAL;  // Inicializa o estado de cada célula como NORMAL
    }
    return linha;
}

// Função que inicializa o jogo, criando o tabuleiro e os estados
Game* initializeGame(int linhas, int colunas) {
    Game *game = malloc(sizeof(Game));  // Aloca memória para o jogo
    if (!game) return NULL;  // Verifica se a alocação falhou

    game->linhas = linhas;
    game->colunas = colunas;

    game->tabuleiro = malloc(linhas * sizeof(char *));  // Aloca memória para o tabuleiro
    game->estado = malloc(linhas * sizeof(State *));  // Aloca memória para os estados

    for (int i = 0; i < linhas; i++) {
        game->tabuleiro[i] = inicializaLinhaTabuleiro(colunas, '#');  // Inicializa cada linha do tabuleiro com '#'
        game->estado[i] = inicializaLinhaEstado(colunas);  // Inicializa o estado de cada linha
    }

    return game;
}

// Função para liberar a memória alocada para o jogo
void freeGame(Game *game) {
    for (int i = 0; i < game->linhas; i++) {
        free(game->tabuleiro[i]);  // Libera memória do tabuleiro
        free(game->estado[i]);  // Libera memória do estado
    }
    free(game->tabuleiro);  // Libera memória do array de tabuleiro
    free(game->estado);  // Libera memória do array de estados
    free(game);  // Libera memória do objeto jogo
}

// Função para imprimir o tabuleiro
void printTab(Game *game) {
    int linhas = game->linhas;
    int colunas = game->colunas;
    char **tabuleiro = game->tabuleiro;
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            printf("%c ", tabuleiro[i][j]);  // Imprime o caractere de cada célula
        }
        printf("\n");
    }
}

// Função para salvar o estado do jogo em um arquivo
void saveGame(Game *game, const char *ficheiro) {
    FILE *f = fopen(ficheiro, "w");  // Abre o arquivo para escrita
    if (!f) {
        perror("Erro ao abrir ficheiro para escrita");  // Trata erro ao abrir o arquivo
        return;
    }

    fprintf(f, "%d %d\n", game->linhas, game->colunas);  // Escreve as dimensões do tabuleiro
    for (int i = 0; i < game->linhas; i++) {
        fprintf(f, "%s\n", game->tabuleiro[i]);  // Escreve o estado de cada linha do tabuleiro
    }

    fclose(f);  // Fecha o arquivo
    printf("Jogo salvo com sucesso no ficheiro \"%s\".\n", ficheiro);  // Mensagem de sucesso
}

// Função para carregar o estado do jogo a partir de um arquivo
void loadGame(Game *game, const char *ficheiro) {
    FILE *f = fopen(ficheiro, "r");  // Abre o arquivo para leitura
    if (!f) {
        perror("Erro ao abrir ficheiro para leitura");  // Trata erro ao abrir o arquivo
        return;
    }

    int linhas, colunas;
    if (fscanf(f, "%d %d\n", &linhas, &colunas) != 2) {  // Lê as dimensões do tabuleiro
        printf("Erro ao ler dimensões do ficheiro.\n");
        fclose(f);
        return;
    }

    // Libera memória antiga do jogo
    for (int i = 0; i < game->linhas; i++) {
        free(game->tabuleiro[i]);
        free(game->estado[i]);
    }
    free(game->tabuleiro);
    free(game->estado);

    game->linhas = linhas;
    game->colunas = colunas;

    game->tabuleiro = malloc(linhas * sizeof(char *));  // Aloca nova memória para o tabuleiro
    game->estado = malloc(linhas * sizeof(State *));  // Aloca nova memória para os estados

    for (int i = 0; i < linhas; i++) {
        game->tabuleiro[i] = malloc((colunas + 1) * sizeof(char));  // Aloca memória para cada linha do tabuleiro
        game->estado[i] = malloc(colunas * sizeof(State));  // Aloca memória para o estado de cada linha

        if (fscanf(f, "%s\n", game->tabuleiro[i]) != 1) {  // Lê cada linha do tabuleiro
            printf("Erro ao ler linha %d do ficheiro.\n", i);
            fclose(f);
            return;
        }

        for (int j = 0; j < colunas; j++) {
            char c = game->tabuleiro[i][j];
            // Atualiza o estado de cada célula com base no caractere do tabuleiro
            if (c == '#') {
                game->estado[i][j] = RISCADA;
            } else if (isupper(c)) {
                game->estado[i][j] = BRANCA;
            } else {
                game->estado[i][j] = NORMAL;
            }
        }
    }

    fclose(f);  // Fecha o arquivo
    printf("Jogo carregado com sucesso.\n");  // Mensagem de sucesso
}

// Função para pintar uma célula no tabuleiro
void paint(Game *game, char coluna, int linha) {
    int col = coluna - 'a';  // Converte a letra da coluna para índice
    int lin = linha - 1;  // Converte o número da linha para índice

    if (linha > 0 && linha <= game->linhas && col >= 0 && col < game->colunas) {
        char c = game->tabuleiro[lin][col];
        if (c == '#') {
            printf("Coordenadas inválidas!\n");
            return;
        }
        if (islower(c)) {
            game->tabuleiro[lin][col] = toupper(c);  // Converte o caractere para maiúsculo
            game->estado[lin][col] = BRANCA;  // Atualiza o estado para BRANCA
        }
        printf("Pinta na coluna %c, linha %d\n", coluna, linha);
    } else {
        printf("Coordenadas inválidas!\n");
    }
}

// Função para riscar uma célula no tabuleiro
void crossout(Game *game, char coluna, int linha) {
    int col = coluna - 'a';  // Converte a letra da coluna para índice
    int lin = linha - 1;  // Converte o número da linha para índice

    if (linha > 0 && linha <= game->linhas && col >= 0 && col < game->colunas) {
        printf("Riscada na coluna %c, linha %d\n", coluna, linha);
        game->tabuleiro[lin][col] = '#';  // Atualiza o tabuleiro para '#'
        game->estado[lin][col] = RISCADA;  // Atualiza o estado para RISCADA
    } else {
        printf("Coordenadas inválidas!\n");
    }
}

// Função para restaurar o estado do jogo a partir de um histórico de movimentos
void restore(Game *game, Moves *historico) {
    if (historico->tamanho == 0) {
        printf("Erro: sem movimentos para desfazer.\n");
        return;
    }

    int i = (historico->tamanho) - 1;
    Move m = historico->lista[i];
    int linha = m.linha - 1;
    int col = m.coluna - 'a';  // Converte a letra da coluna para índice

    if (linha < 0 || linha >= game->linhas || col < 0 || col >= game->colunas) {
        printf("Erro: índices fora dos limites.\n");
        return;
    }

    game->tabuleiro[linha][col] = m.valorAntigo;  // Restaura o valor antigo da célula
    game->estado[linha][col] = m.estadoAntigo;  // Restaura o estado antigo da célula

    historico->tamanho--;  // Decrementa o tamanho do histórico
}

// Função para verificar se um símbolo é único na linha
bool isUniqueInRow(Game *game, int row, char symbol) {
    for (int j = 1; j < game->colunas; j++) {
        if (game->tabuleiro[row][j] == symbol && game->estado[row][j] == BRANCA) {
            return false;  // Retorna falso se o símbolo já existir na linha
        }
    }
    return true;
}

// Função para verificar se um símbolo é único na coluna
bool isUniqueInColumn(Game *game, int col, char symbol) {
    for (int i = 1; i < game->linhas; i++) {
        if (game->tabuleiro[i][col] == symbol && game->estado[i][col] == BRANCA) {
            return false;  // Retorna falso se o símbolo já existir na coluna
        }
    }
    return true;
}

// Função para verificar se todos os vizinhos de uma célula são brancos
bool areNeighborsWhite(Game *game, int row, int col) {
    int directions[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};  // Direções dos vizinhos (cima, baixo, esquerda, direita)
    for (int d = 0; d < 4; d++) {
        int newRow = row + directions[d][0];
        int newCol = col + directions[d][1];
        if (newRow >= 0 && newRow < game->linhas && newCol >= 0 && newCol < game->colunas) {
            if (game->estado[newRow][newCol] != BRANCA) {
                return false;  // Retorna falso se algum vizinho não for BRANCA
            }
        }
    }
    return true;  // Retorna verdadeiro se todos os vizinhos forem BRANCA
}

// Função para verificar se as células brancas estão conectadas ortogonalmente
bool isConnected(Game *game, int row, int col, bool **visited) {
    if (row < 0 || row >= game->linhas || col < 0 || col >= game->colunas || visited[row][col] || game->estado[row][col] != BRANCA) {
        return false;  // Retorna falso se a célula não for válida ou já visitada
    }
    visited[row][col] = true;

    int directions[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};  // Direções dos vizinhos
    for (int d = 0; d < 4; d++) {
        int newRow = row + directions[d][0];
        int newCol = col + directions[d][1];
        if (newRow >= 0 && newRow < game->linhas && newCol >= 0 && newCol < game->colunas) {
            isConnected(game, newRow, newCol, visited);  // Chama recursivamente para os vizinhos
        }
    }
    return true;  // Retorna verdadeiro se todas as células conectadas forem visitadas
}

// Função que verifica se todas as células brancas estão conectadas ortogonalmente
bool checkAllConnected(Game *game) {
    // Aloca uma matriz para rastrear as células visitadas
    bool **visited = malloc(game->linhas * sizeof(bool *));
    for (int i = 0; i < game->linhas; i++) {
        visited[i] = calloc(game->colunas, sizeof(bool));  // Inicializa as células como não visitadas
    }

    bool foundFirstWhite = false;  // Flag para indicar se a primeira célula branca foi encontrada
    // Itera sobre o tabuleiro para encontrar as células brancas
    for (int i = 0; i < game->linhas; i++) {
        for (int j = 0; j < game->colunas; j++) {
            if (game->estado[i][j] == BRANCA) {  // Verifica se a célula é branca
                if (!foundFirstWhite) {
                    isConnected(game, i, j, visited);  // Chama a função recursiva para marcar as células conectadas
                    foundFirstWhite = true;  // Marca que a primeira célula branca foi encontrada
                } else if (!visited[i][j]) {  // Se encontrar uma célula branca desconectada
                    // Libera a memória usada pela matriz visited
                    for (int k = 0; k < game->linhas; k++) free(visited[k]);
                    free(visited);
                    return false;  // Retorna falso se as células não estão todas conectadas
                }
            }
        }
    }

    // Libera a memória da matriz visited
    for (int i = 0; i < game->linhas; i++) free(visited[i]);
    free(visited);

    return true;  // Retorna verdadeiro se todas as células brancas estão conectadas
}

// Função que verifica se o estado do jogo é válido
void verify(Game *game) {
    bool valid = true;  // Inicializa a variável de validade do jogo

    /*
    bool unique = true;
    bool neighbors = true;
    bool allConnected = true;
    */
    
    // Verifica cada célula no tabuleiro
    for (int i = 0; i < game->linhas; i++) {
        for (int j = 0; j < game->colunas; j++) {
            char symbol = game->tabuleiro[i][j];  // Obtém o símbolo da célula
            if (game->estado[i][j] == BRANCA) {  // Se a célula for branca
                // Verifica se o símbolo é único na linha e na coluna
                if (!isUniqueInRow(game, i, symbol) || !isUniqueInColumn(game, j, symbol)) {
                    printf("Warning: símbolo '%c' repetido na linha %d ou coluna %c.\n", symbol, i + 1, j + 'a');
                    valid = false;  // Marca o jogo como inválido
                    //unique = false;
                }
            } else if (game->estado[i][j] == RISCADA) {  // Se a célula for riscada
                // Verifica se a célula riscada tem vizinhos não brancos
                if (!areNeighborsWhite(game, i, j)) {
                    printf("Warning: casa riscada (%c, %d) tem vizinhos não pintados de branco.\n", j + 'a', i + 1);
                    valid = false;  // Marca o jogo como inválido
                    //neighbors = false;
                }
            }
        }
    }

    // Verifica se todas as células brancas estão conectadas
    if (!checkAllConnected(game)) {
        printf("Warning: casas brancas não estão conectadas ortogonalmente.\n");
        valid = false;  // Marca o jogo como inválido
        // allConnected = false;
    }

    // Imprime a validade do jogo
    if (valid) {
        printf("O estado do jogo é válido.\n");
    }
}