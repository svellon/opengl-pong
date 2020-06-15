COMPILE_FLAGS = -Wall
STD_FLAGS = -std=c++17 -stdlib=libc++

default: start

debug: COMPILE_FLAGS += -g
debug: start

start: main.o glad.o polygon.o game.o
	clang++ -o bin/start $(COMPILE_FLAGS) -Iinclude $(STD_FLAGS)  -lglfw -framework CoreVideo -framework OpenGL -framework IOKit -framework Cocoa -framework Carbon obj/glad.o obj/main.o obj/polygon.o obj/game.o

main.o: src/main.cpp
	clang++ -c -o obj/main.o $(COMPILE_FLAGS) -Wno-deprecated-declarations -Iinclude $(STD_FLAGS) src/main.cpp

game.o: src/game.cpp
	clang++ -c -o obj/game.o $(COMPILE_FLAGS) -Wno-deprecated-declarations -IInclude $(STD_FLAGS) src/game.cpp

polygon.o: src/polygon.cpp
	clang++ -c -o obj/polygon.o $(COMPILE_FLAGS) -Wno-deprecated-declarations -Iinclude $(STD_FLAGS)  src/polygon.cpp

glad.o: src/glad/glad.c
	clang -c -o obj/glad.o $(COMPILE_FLAGS) -Iinclude -stdlib=libc++ src/glad/glad.c

clean:
	rm -f obj/*
	rm -f bin/*

