.PHONY:all

all: socks5 tranfer

socks5:socks5.cc epoll.cc
	g++ -o $@ $^

tranfer:tranfer.cc epoll.cc
	g++ -o $@ $^

.PHONY:clean

clean:
	rm -rf socks5 tranfer
