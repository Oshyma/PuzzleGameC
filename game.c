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
void paint(Game *game, char coluna, int linha, Moves *historico) {
    int col = coluna - 'a';  // Converte a letra da coluna para índice
    int lin = linha - 1;  // Converte o número da linha para índice

    if (linha > 0 && linha <= game->linhas && col >= 0 && col < game->colunas) {
        char c = game->tabuleiro[lin][col];
        if (c == '#') {
            for(int i = 0; i < historico->tamanho; i++){
                Move m = historico->lista[i];
                if (m.coluna == coluna && m.linha == linha) {
                    char symbol = m.valorAntigo;  // Obtém o símbolo antigo
                    game->tabuleiro[lin][col] = toupper(symbol);  // Converte o caractere para maiúsculo
                    game->estado[lin][col] = BRANCA;  // Atualiza o estado para BRANCA
                    return;  // Retorna se a célula já foi riscada
                }
            }
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
bool isUniqueInRow(Game *game, int row, int col, char symbol) {
    for (int j = 1; j < game->colunas; j++) {
        if (game->tabuleiro[row][j] == tolower(symbol) && j != col) {
            return false;  // Retorna falso se o símbolo já existir na linha
        }
    }
    return true;
}

// Função para verificar se um símbolo é único na coluna
bool isUniqueInColumn(Game *game, int row, int col, char symbol) {
    for (int i = 1; i < game->linhas; i++) {
        if (game->tabuleiro[i][col] == tolower(symbol) && i != row) {
            return false;  // Retorna falso se o símbolo já existir na coluna
        }
    }
    return true;
}

// Função auxiliar para verificar se todas as casas vizinhas de uma célula riscada estão pintadas de branco
bool areCrossedOutNeighborsWhite(Game *game, int row, int col) {
    int directions[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
    for (int d = 0; d < 4; d++) {
        int newRow = row + directions[d][0];
        int newCol = col + directions[d][1];
        if (newRow >= 0 && newRow < game->linhas && newCol >= 0 && newCol < game->colunas) {
            if (game->estado[newRow][newCol] != BRANCA) {
                if (game->estado[newRow][newCol] == RISCADA) {
                    printf("Warning: Pelo menos Casa riscada tem vizinho(s) riscado(s).\n");
                } else {
                printf("Warning: Pelo menos Casa riscada tem vizinho(s) nao branco(s).\n");
                }
                return false; // Retorna falso se algum vizinho não for BRANCA
            }
        }
    }
    return true; // Retorna verdadeiro se todos os vizinhos forem BRANCA
}

// Função auxiliar para verificar conectividade de células brancas usando BFS
bool checkWhiteCellsConnectivity(Game *game, bool **visited, int totalWhiteCells) {
    int directions[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
    int queueSize = game->linhas * game->colunas;
    int *queueRow = malloc(queueSize * sizeof(int));
    int *queueCol = malloc(queueSize * sizeof(int));
    int front = 0, rear = 0;
    int visitedCount = 0;

    // Encontrar a primeira célula branca
    int startRow = -1, startCol = -1;
    for (int i = 0; i < game->linhas && startRow == -1; i++) {
        for (int j = 0; j < game->colunas; j++) {
            if (game->estado[i][j] != RISCADA) {
                startRow = i;
                startCol = j;
                j = game->colunas; // Sair do loop interno
            }
        }
    }

    // Adicionar a célula inicial à fila
    queueRow[rear] = startRow;
    queueCol[rear++] = startCol;
    visited[startRow][startCol] = true;

    while (front < rear) {
        int currentRow = queueRow[front];
        int currentCol = queueCol[front++];
        visitedCount++;

        // Verificar vizinhos ortogonais
        for (int d = 0; d < 4; d++) {
            int newRow = currentRow + directions[d][0];
            int newCol = currentCol + directions[d][1];
            if (newRow >= 0 && newRow < game->linhas && newCol >= 0 && newCol < game->colunas) {
                if (!visited[newRow][newCol] && game->estado[newRow][newCol] != RISCADA) {
                    visited[newRow][newCol] = true;
                    queueRow[rear] = newRow;
                    queueCol[rear++] = newCol;
                }
            }
        }
    }

    free(queueRow);
    free(queueCol);

    return visitedCount == totalWhiteCells;
}

// Função principal para verificar conectividade e vizinhos de casas riscadas
bool areAllWhiteCellsConnected(Game *game) {
    int totalWhiteCells = 0;

    // Alocar matriz para rastrear células visitadas
    bool **visited = malloc(game->linhas * sizeof(bool *));
    for (int i = 0; i < game->linhas; i++) {
        visited[i] = calloc(game->colunas, sizeof(bool));
    }

    // Contar células brancas e verificar vizinhos de casas riscadas
    for (int i = 0; i < game->linhas; i++) {
        for (int j = 0; j < game->colunas; j++) {
            if (game->estado[i][j] == RISCADA) {
                if (!areCrossedOutNeighborsWhite(game, i, j)) {
                    for (int k = 0; k < game->linhas; k++) free(visited[k]);
                    free(visited);
                    return false; // Retorna falso se alguma casa riscada tiver vizinhos não brancos
                }
            } else if (game->estado[i][j] != RISCADA) {
                totalWhiteCells++;
            }
        }
    }

    // Verificar conectividade das células brancas
    bool connected = checkWhiteCellsConnectivity(game, visited, totalWhiteCells);

    // Liberar memória da matriz visited
    for (int i = 0; i < game->linhas; i++) free(visited[i]);
    free(visited);

    return connected;
}
// Função que verifica se o estado do jogo é válido
bool verify(Game *game) {
    bool valid = true;  // Inicializa a variável de validade do jogo
    // Verificar unicidade de símbolos e vizinhos de células riscadas
    for (int i = 0; i < game->linhas; i++) {
        for (int j = 0; j < game->colunas; j++) {
            char symbol = game->tabuleiro[i][j];
            if (game->estado[i][j] == BRANCA) {
                if (!isUniqueInRow(game, i, j ,symbol) || !isUniqueInColumn(game,i, j, symbol)) {
                    printf("Warning: Pelo menos um símbolo não é único na sua linha ou coluna.\n");
                    valid = false;
                }
            }
        }
    }

    // Verificar conectividade de células brancas
    if (!areAllWhiteCellsConnected(game)) {
        printf("Warning: As casas brancas não estão conectadas ortogonalmente.\n");
        valid = false;
    }

    if (valid) {
        printf("O estado do jogo é válido.\n");
    }

    return valid;  
}

bool help(Game *game, Moves *historico) {
    bool changed = false; // Rastreia se houve alterações

    // 1. Riscar todas as letras iguais a uma letra branca na mesma linha e/ou coluna
    for (int i = 0; i < game->linhas; i++) {
        for (int j = 0; j < game->colunas; j++) {
            if (game->estado[i][j] == BRANCA) {
                char symbol = game->tabuleiro[i][j];
                if(!isUniqueInColumn(game,i, j, symbol)){
                    for (int k = 0; k < game->linhas; k++) {
                        if (game->tabuleiro[k][j] == tolower(symbol)) {
                            crossout(game, j + 'a', k + 1); // Risca a célula
                            changed = true; // Alteração feita
                        }
                    }
                }
                if(!isUniqueInRow(game, i, j, symbol)){
                    for (int k = 0; k < game->colunas; k++) {
                        if (game->tabuleiro[i][k] == tolower(symbol)) {
                            crossout(game, k + 'a', i + 1); // Risca a célula
                            changed = true; // Alteração feita
                        }
                    }
                }
            }
        }
    }

    // 2. Pintar de branco todas as casas vizinhas de uma casa riscada
    for (int i = 0; i < game->linhas; i++) {
        for (int j = 0; j < game->colunas; j++) {
            if (game->estado[i][j] == RISCADA) {
                int directions[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
                for (int d = 0; d < 4; d++) {
                    int newRow = i + directions[d][0];
                    int newCol = j + directions[d][1];
                    if (newRow >= 0 && newRow < game->linhas && newCol >= 0 && newCol < game->colunas && game->estado[newRow][newCol] == NORMAL) {
                        paint(game, newCol + 'a', newRow + 1, historico); // Pinta a célula vizinha
                        changed = true; // Alteração feita
                    }
                }
            }
        }
    }

    // 3. Pintar de branco uma casa quando seria impossível que esta fosse riscada por isolar casas brancas
    for (int i = 0; i < game->linhas; i++) {
        for (int j = 0; j < game->colunas; j++) {
            if (game->estado[i][j] == NORMAL) {
                // Verificar se riscar esta célula isolaria células brancas
                if (!areAllWhiteCellsConnected(game)) {
                    paint(game, j + 'a', i + 1, historico); // Pinta a célula
                    changed = true; // Alteração feita
                }
            }
        }
    }

    if (!changed) {
        printf("Nenhuma alteração foi feita pelo comando help.\n");
    }
    return changed; 
}
void autohelp(Game *game, Moves *historico) {
    bool changed;
    do { 
        changed = help(game, historico); // Executa o comando help

    } while (changed);
}


void solveGame(Game *game, Moves *historico) {
    while (historico->tamanho > 0) {
        restore(game, historico); // Restaura o jogo a partir do histórico
    }
    saveGame(game, "temp.txt"); // Salva o estado atual do jogo
    for(int i = 0; i < game->linhas; i++) {
        for (int j = 0; j < game->colunas; j++) {
            loadGame(game, "temp.txt"); // Carrega o estado do jogo
            crossout(game, j + 'a', i + 1); // Risca a célula
            autohelp(game, historico); // Executa o autoajuda
            if (verify(game)) {
                printf("O jogo foi resolvido com sucesso!\n");
                return; // Retorna se o jogo foi resolvido
            }
        }
    }
    printf("O jogo não pôde ser resolvido.\n"); // Mensagem de falha na resolução

}