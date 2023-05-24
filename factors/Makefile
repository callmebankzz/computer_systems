CC      = cc
CFLAGS  = -std=gnu11 -Wall -Wextra -Werror -g
LDLIBS  = -lm

PROG    = factors
OBJS    = factors.o

all: ${PROG}

${PROG}: ${OBJS}
	${CC} ${CFLAGS} -o ${PROG} ${OBJS} ${LDLIBS}

clean:
	${RM} *.o ${PROG} core.[1-9]*

.PHONY: clean
