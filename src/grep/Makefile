FILES = grep_functions.c s21_grep.c
WAYS = -Wall -Werror -Wextra

all: s21_grep

s21_grep:
	gcc $(WAYS) $(FILES) -o s21_grep

work: clean
	gcc $(FILES) -o s21_grep

clean:
	rm -rf *.out s21_grep *.dSYM

rebuild: clean all

debug: clean
	gcc $(WAYS) $(FILES) -o s21_grep -fsanitize=address -static-libgcc -g

test: clean all
	bash test_func_grep.sh