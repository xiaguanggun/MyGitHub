OUT:=client
SRCS:=$(wildcard *.c)
OBJS:=$(patsubst %.c,%.o,$(SRCS))
CC:=gcc
$(OUT):$(OBJS)
	$(CC) $^ -o $@ -Wall -lpthread -lcrypt -lmysqlclient -lcrypto -lssl -g
%.o:%.c
	$(CC) -c $^ -o $@ -Wall -g
.PHONY:clean rebuild
clean:
	$(RM) $(OUT) $(OBJS)
rebuild:clean main
