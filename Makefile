FLAGS=-Wall -Wextra -Werror
.PHONY: clean

build/sevrer: build/main.o build/args.o build/web.o
	mkdir -p build
	gcc $(FLAGS) $^ -o $@

build/main.o: src/main.c
	mkdir -p build
	gcc $(FLAGS) -c $^ -o $@

build/args.o: src/args.c src/args.h
	mkdir -p build
	gcc $(FLAGS) -c $< -o $@

build/web.o: src/web.c src/web.h
	mkdir -p build
	gcc $(FLAGS) -c $< -o $@

clean:
	rm -f build/*.o build/sevrer
