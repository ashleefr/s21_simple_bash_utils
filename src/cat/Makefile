FILES = cat_functions.c s21_cat.c
WAYS = -Wall -Werror -Wextra

all: s21_cat

s21_cat:
	gcc $(WAYS) $(FILES) -o s21_cat

work:
	gcc $(FILES)

clean:
	rm -rf *.out s21_cat *.dSYM ./Tests/s21_cat

rebuild: clean all

test: clean all
	mv s21_cat Tests

debug: clean
	gcc $(WAYS) $(FILES) -o debug.out -fsanitize=address -static-libgcc -g

