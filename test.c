#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include <stdbool.h>
#include "game.h"

void test_paint() {
    Game* game = initializeGame(2, 2);
    game->tabuleiro[0][0] = 'a';
    game->tabuleiro[0][1] = 'b';
    game->tabuleiro[1][0] = 'c';
    game->tabuleiro[1][1] = 'd';
    paint(game, 'a', 1);
    CU_ASSERT_EQUAL(game->tabuleiro[0][0], 'A');
    freeGame(game); 
}

void test_crossout() {
    Game* game = initializeGame(2, 2);
    game->tabuleiro[0][0] = 'a';
    game->tabuleiro[0][1] = 'b';
    game->tabuleiro[1][0] = 'c';
    game->tabuleiro[1][1] = 'd';
    crossout(game, 'b', 2);
    CU_ASSERT_EQUAL(game->tabuleiro[1][1], '#');
    freeGame(game); 
}

void test_loadGame() {
    Game* game = initializeGame(2, 2);
    FILE *f = fopen("test_input.txt", "w");
    fprintf(f, "2 2\nab\ncd\n");
    fclose(f);

    loadGame(game, "test_input.txt");
    CU_ASSERT_EQUAL(game->linhas, 2);
    CU_ASSERT_EQUAL(game->colunas, 2);
    CU_ASSERT_EQUAL(game->tabuleiro[0][1], 'b');
    CU_ASSERT_EQUAL(game->tabuleiro[1][1], 'd');
    freeGame(game); 
}


void test_saveGame() {
    Game* game = initializeGame(2, 2);
    game->tabuleiro[0][0] = 'a';
    game->tabuleiro[0][1] = 'b';
    game->tabuleiro[1][0] = 'c';
    game->tabuleiro[1][1] = 'd';

    saveGame(game, "test_output.txt");

    FILE *f = fopen("test_output.txt", "r");
    char buffer[100];
    fgets(buffer, sizeof(buffer), f);
    CU_ASSERT_STRING_EQUAL(buffer, "2 2\n");
    fgets(buffer, sizeof(buffer), f);
    CU_ASSERT_STRING_EQUAL(buffer, "ab\n");
    fclose(f);
    remove("test_output.txt");
    freeGame(game);
}

int main() {
    CU_initialize_registry();
    CU_pSuite suite = CU_add_suite("GameTestSuite", 0, 0);

    CU_add_test(suite, "Test Paint", test_paint);
    CU_add_test(suite, "Test Crossout", test_crossout);
    CU_add_test(suite, "Test LoadGame", test_loadGame);
    CU_add_test(suite, "Test SaveGame", test_saveGame);


    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();

    return 0;
}
