CC = gcc
CFLAGS = -Wall -Wextra -pedantic -O2 -fsanitize=address -fno-omit-frame-pointer -g
LDFLAGS = -lcunit

SRC = game.c
TEST_SRC = test.c
MAIN_SRC = main.c

all: jogo

jogo: $(MAIN_SRC) $(SRC)
	$(CC) $(CFLAGS) -o jogo $(MAIN_SRC) $(SRC)
 
testar: $(TEST_SRC) $(SRC)
	$(CC) $(CFLAGS) -fprofile-arcs -ftest-coverage -o testar $(TEST_SRC) $(SRC) $(LDFLAGS)
	./testar
	gcov game.c

clean:
	rm -f *.o jogo testar *.gcda *.gcno *.gcov test_input.txt
