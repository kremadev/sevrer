FLAGS=-Wall -Wextra -Werror
.PHONY: clean

build/sevrer: build/main.o build/args.o
	gcc $(FLAGS) $^ -o $@

build/main.o: src/main.c
	mkdir -p build
	gcc $(FLAGS) -c $^ -o $@

build/args.o: src/args.c src/args.h
	gcc $(FLAGS) -c $< -o $@

clean:
	rm -f build/*.o build/sevrer
