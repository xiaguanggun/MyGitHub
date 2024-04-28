OUT:=server
SRCS:=$(wildcard *.c)
OBJS:=$(patsubst %.c,%.o,$(SRCS))
CC:=gcc
$(OUT):$(OBJS)
	$(CC) $^ -o $@ -Wall -lpthread -lcrypt -lmysqlclient -lcrypto -lssl -ll8w8jwt -g
%.o:%.c
	$(CC) -c $^ -o $@ -Wall -g
.PHONY:clean rebuild
clean:
	$(RM) $(OUT) $(OBJS)
rebuild:clean main
