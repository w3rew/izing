all:
	clang++ -o izing.o izing.cpp -std=c++17 -march=native -Ofast -DMULTITHREAD -fopenmp

debug:
	clang++ -o izing.o izing.cpp -g -std=c++17 -fsanitize=address -fsanitize=alignment -fsanitize=bool -fsanitize=bounds -fsanitize=enum -fsanitize=float-cast-overflow -fsanitize=float-divide-by-zero -fsanitize=integer-divide-by-zero -fsanitize=leak -fsanitize=nonnull-attribute -fsanitize=null -fsanitize=object-size -fsanitize=return -fsanitize=returns-nonnull-attribute -fsanitize=shift -fsanitize=signed-integer-overflow -fsanitize=undefined -fsanitize=unreachable -fsanitize=vla-bound -fsanitize=vptr -lm -pie -DDEBUG -Wall -fopenmp

onethread:
	clang++ -o izing.o izing.cpp -std=c++17 -march=native -Ofast
