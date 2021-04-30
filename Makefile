INSTALL_DIR=install
SRC_DIR=src
STR_DIR=src/strategie

BIN=server alltests

ifeq ($(shell uname -s), Darwin)
    LIBGCOV=-fprofile-arcs -ftest-coverage
else
    LIBGCOV=-lgcov
endif

CC=cc
CPPFLAGS=-I${SRC_DIR} -I${GSL_PATH}/include
LDFLAGS= -L${GSL_PATH}/lib  -lgsl -lm -ldl -lgslcblas ${LIBGCOV}
CFLAGS=-Wall -Wextra -std=c99 --coverage -g -O0


.PHONY: clean install

all: build

build:server players

test:alltests

clean:
	rm -f *.so *.o *.gcno *.gcda */*~ ${SRC_DIR}/*.h.gch ${STR_DIR}/*.h.gch ${INSTALL_DIR}/* ${BIN}

install:server alltests players


# Players

libplayer1.so: ${SRC_DIR}/player1.c graph_function.o
	${CC} ${CPPFLAGS} ${CFLAGS} ${LDFLAGS} -fPIC -shared $^ -o install/$@

captainplayer.so: ${SRC_DIR}/captainplayer.c graph_function.o set_sentinel.o strategie.o methods.o
	${CC} ${CPPFLAGS} ${CFLAGS} ${LDFLAGS} -fPIC -shared $^ -o install/$@

eval_player.so:  ${SRC_DIR}/eval_player.c graph_function.o set_sentinel.o evaluation.o methods.o
	${CC} ${CPPFLAGS} ${CFLAGS} ${LDFLAGS} -fPIC -shared $^ -o install/$@

players:libplayer1.so captainplayer.so eval_player.so

# Server

graph_function.o: ${SRC_DIR}/graph_function.[ch]
	${CC} ${CPPFLAGS} ${CFLAGS} $^ -fPIC -c

server_func.o: ${SRC_DIR}/server_func.[ch]  ${SRC_DIR}/server.h ${SRC_DIR}/player.h
	${CC} ${CPPFLAGS} ${CFLAGS} $^ -c

server.o: ${SRC_DIR}/server.[ch]
	${CC} ${CPPFLAGS} ${CFLAGS} $^ -c

stack.o: ${SRC_DIR}/stack.c ${SRC_DIR}/stack.h ${SRC_DIR}/stack_struct.h
	${CC} ${CPPFLAGS} ${CFLAGS} $^ -c

server: server.o server_func.o graph_function.o stack.o
	$(CC)  $^ ${CFLAGS} -o install/$@ ${LDFLAGS}

# Tests
test_graph.o: ${SRC_DIR}/test_graph.c
	${CC} ${CPPFLAGS} ${CFLAGS} $^ -c

test_player%.o: ${SRC_DIR}/test_player%.c
	${CC} -L. ${CPPFLAGS} ${CFLAGS} $^ -c ${LDFLAGS}

test_captainplayer.o: ${SRC_DIR}/test_captainplayer.c
	${CC} -L. ${CPPFLAGS} ${CFLAGS} $^ -c ${LDFLAGS}

test_eval_player.o: ${SRC_DIR}/test_eval_player.c
	${CC} -L. ${CPPFLAGS} ${CFLAGS} $^ -c ${LDFLAGS}

test_stack.o: ${SRC_DIR}/test_stack.c ${SRC_DIR}/stack.h ${SRC_DIR}/stack_struct.h
	${CC} ${CPPFLAGS} ${CFLAGS} $^ -c

test_server.o: ${SRC_DIR}/test_server.c ${SRC_DIR}/server.h ${SRC_DIR}/graph_function.h ${SRC_DIR}/move.h ${SRC_DIR}/graph.h
	${CC} ${CPPFLAGS} ${CFLAGS} $^ -c

alltests:${SRC_DIR}/alltests.c server_func.o graph_function.o test_graph.o test_player1.o test_captainplayer.o test_eval_player.o test_stack.o stack.o test_server.o
	$(CC) ${CPPFLAGS} ${CFLAGS} $^ -o install/$@ ${LDFLAGS}

#strategie
set_sentinel.o: ${STR_DIR}/set_sentinel.c ${STR_DIR}/set_sentinel.h
	${CC} ${CPPFLAGS} ${CFLAGS} $^ -c -fPIC

strategie.o: ${STR_DIR}/strategie.c ${SRC_DIR}/graph_function.h ${STR_DIR}/set_sentinel.h
	${CC} ${CPPFLAGS} ${CFLAGS} $^ -fPIC -c ${LDFLAGS}

evaluation.o : ${STR_DIR}/evaluation.c ${SRC_DIR}/graph_function.h ${STR_DIR}/set_sentinel.h
	${CC} ${CPPFLAGS} ${CFLAGS} $^ -fPIC -c ${LDFLAGS}

methods.o : ${STR_DIR}/methods.c ${SRC_DIR}/graph_function.h ${STR_DIR}/set_sentinel.h
	${CC} ${CPPFLAGS} ${CFLAGS} $^ -fPIC -c ${LDFLAGS}
