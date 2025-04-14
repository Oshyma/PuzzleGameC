#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include "game.h"

void test_paint() {
    Game game = initializeGame();
    paint(&game, 'a', 1);
    CU_ASSERT_EQUAL(game.tabuleiro[0][0], 'A');
}

void test_crossout() {
    Game game = initializeGame();
    crossout(&game, 'b', 2);
    CU_ASSERT_EQUAL(game.tabuleiro[1][1], '#');
}

void test_loadGame() {
    Game game;
    FILE *f = fopen("test_input.txt", "w");
    fprintf(f, "2 2\nab\ncd\n");
    fclose(f);

    loadGame(&game, "test_input.txt");
    CU_ASSERT_EQUAL(game.linhas, 2);
    CU_ASSERT_EQUAL(game.colunas, 2);
    CU_ASSERT_EQUAL(game.tabuleiro[0][1], 'b');
    CU_ASSERT_EQUAL(game.tabuleiro[1][1], 'd');
}

int main() {
    CU_initialize_registry();
    CU_pSuite suite = CU_add_suite("GameTestSuite", 0, 0);

    CU_add_test(suite, "Test Paint", test_paint);
    CU_add_test(suite, "Test Crossout", test_crossout);
    CU_add_test(suite, "Test LoadGame", test_loadGame);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();

    return 0;
}
