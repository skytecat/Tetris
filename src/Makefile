CC=gcc
FLAGS  = -lncurses -Wall -Werror -Wextra -std=c11
SOURCE = backend/*.c frontend/front.c
TEST_SOURCE = backend/main.c frontend/front.c

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
	rm -rf tetris

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
