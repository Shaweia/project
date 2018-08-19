.PHONY:all

all:epoll_LT epoll_ET

epoll_LT:epoll_LT.c
	gcc -o $@ $^

epoll_ET:epoll_ET.c
	gcc -o $@ $^

.PHONY:clean

clean:
	rm -rf epoll_LT epoll_ET
