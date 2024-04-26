OUT:=main
SRCS:=$(wildcard *.cc)
OBJS:=$(patsubst %.cc,%.o,$(SRCS))
CC:=g++
$(OUT):$(OBJS)
	$(CC) $^ -o $@ -g -Wall -fno-elide-constructors -llog4cpp -lpthread
%.o:%.cc
	$(CC) -c $^ -o $@ -g -Wall -fno-elide-constructors
.PHONY:clean rebuild
clean:
	$(RM) $(OUT) $(OBJS)
rebuild:clean main
