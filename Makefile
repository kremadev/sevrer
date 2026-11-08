FLAGS=-Wall -Wextra -Werror
LIBS=-lssl -lcrypto
.PHONY: clean

build/sevrer: build/main.o build/args.o build/web.o build/ssl.o
	mkdir -p build
	gcc $(FLAGS) $^ -o $@ $(LIBS)

build/main.o: src/main.c src/args.h src/web.h src/ssl.h
	mkdir -p build
	gcc $(FLAGS) -c $< -o $@

build/args.o: src/args.c src/args.h
	mkdir -p build
	gcc $(FLAGS) -c $< -o $@

build/web.o: src/web.c src/web.h src/ssl.h
	mkdir -p build
	gcc $(FLAGS) -c $< -o $@

build/ssl.o: src/ssl.c src/ssl.h
	mkdir -p build
	gcc $(FLAGS) -c $< -o $@

clean:
	rm -f build/*.o build/sevrer
