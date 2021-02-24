build:
	gcc -o sudoku src/cJSON.c src/main.c src/utils.c -I -Wall -g -lm

run:
	./sudoku

clean:
	rm sudoku