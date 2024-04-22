OUT:=main
SRCS:=$(wildcard *.cc)
OBJS:=$(patsubst %.cc,%.o,$(SRCS))
CC:=g++
$(OUT):$(OBJS)
	$(CC) $^ -o $@ -g -Wall -llog4cpp -lpthread
%.o:%.cc
	$(CC) -c $^ -o $@ -g -Wall
.PHONY:clean rebuild
clean:
	$(RM) $(OUT) $(OBJS)
rebuild:clean main
