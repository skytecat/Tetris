CC=gcc
FLAGS  = -lncurses -Wall -Werror -Wextra -std=c11
SOURCE = brick_game/tetris/*.c gui/cli/front.c
TEST_SOURCE = brick_game/tetris/back.c gui/cli/front.c

ifeq ($(shell uname -s),Linux)
	LIBS=-lm -lrt -lpthread -lcheck -lsubunit
endif

ifeq ($(shell uname -s),Darwin)
	LIBS=-lcheck
endif

all: install

install:
	$(CC) $(FLAGS) $(SOURCE) -o tetris
	./tetris

uninstall:
	rm -rf tetris

clean:
	rm -rf s21_test
	rm -rf *.a
	rm -rf *.gcno
	rm -rf *.gcda *.o
	rm -rf *.html
	rm -rf *.info
	rm -rf *.css
	rm -rf report
	rm -rf report_files
	rm -rf s21_test.dSYM
	rm -rf a.out
	rm -rf *.gcov

dvi:
	open Readme.md

dist:clean
	mkdir tetris_game
	mkdir tetris_game/src
	cp -R brick_game tetris_game/src
	cp -R gui tetris_game/src
	cp -R diagram.png tetris_game/src
	cp -R Makefile tetris_game/src
	cp -R Readme.md tetris_game/src
	tar cvzf tetris_game.tgz tetris_game/
	rm -rf tetris_game

test: clean
	$(CC) $(FLAGS) --coverage tests/tests.c $(TEST_SOURCE) $(LIBS) -o s21_test
	./s21_test

gcov_report: clean
	$(CC) $(FLAGS) --coverage $(TEST_SOURCE) tests/tests.c -o s21_test $(LIBS)
	./s21_test
	lcov -t "coverage" -o s21_test.info -c -d .
	genhtml -o report s21_test.info
	open ./report/index.html

lcov:
	brew install lcov

brew:
	cd
	curl -fsSL https://rawgit.com/kube/42homebrew/master/install.sh | zsh

clang:
	clang-format -style=Google -i $(SOURCE) brick_game/tetris/*.h gui/cli/*.h
	clang-format -style=Google -i  tests/*.c tests/*.h
