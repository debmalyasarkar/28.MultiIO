
MAKEFLAGS += -rR --no-print-directory
RM=@rm -rf a.out *~ *.o *.swp *.sow *~
GCC=@gcc -w 
ECHO=&& echo -n "  CC  " && echo "$P"

all:
	${GCC} mkPipe.c   -o mkPipe ${ECHO}mkPipe.c
	${GCC} wrPipe1.c  -o wrPipe1 ${ECHO}wrPipe1.c
	${GCC} wrPipe2.c  -o wrPipe2 ${ECHO}wrPipe2.c
	${GCC} poll.c -o poll  ${ECHO}poll

clean:
	${RM} *~ pone ptwo mkPipe wrPipe1 wrPipe2 poll

