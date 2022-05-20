CXXFLAGS ?= -std=c++17 -march=native -Ofast
THREADING ?= yes
THREADING_FLAGS ?= -DMULTITHREAD -fopenmp
DEBUG_FLAGS ?= -g -std=c++17 -fsanitize=address -fsanitize=alignment -fsanitize=bool -fsanitize=bounds -fsanitize=enum -fsanitize=float-cast-overflow -fsanitize=float-divide-by-zero -fsanitize=integer-divide-by-zero -fsanitize=leak -fsanitize=nonnull-attribute -fsanitize=null -fsanitize=object-size -fsanitize=return -fsanitize=returns-nonnull-attribute -fsanitize=shift -fsanitize=signed-integer-overflow -fsanitize=undefined -fsanitize=unreachable -fsanitize=vla-bound -fsanitize=vptr -lm -pie -DDEBUG -Wall -fopenmp

ifeq (${THREADING}, yes)
	CXXFLAGS += ${THREADING_FLAGS}
endif

all:
	clang++ -o izing.o izing.cpp ${CXXFLAGS}

debug:
	clang++ -o izing.o izing.cpp ${DEBUG_FLAGS}
