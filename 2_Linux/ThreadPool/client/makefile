SRCS:=$(wildcard *.c)
EXES:=$(patsubst %.c,%,$(SRCS))
CC:=gcc
all:$(EXES)
%:%.c
	$(CC) $^ -o $@ -g -Wall -lpthread
.PHONY:clean rebuild
clean:
	$(RM) $(EXES)
rebuild:clean all
