LUA ?= lua5.1
LUA_PC ?= lua5.1
LUA_CFLAGS = $(shell pkg-config $(LUA_PC) --cflags)

CFLAGS ?= -O3 -Wall -Werror

all: stringy.so

%.o: %.c
	$(CC) -c $(CFLAGS) -fPIC $(LUA_CFLAGS) -o $@ $<

stringy.o: fastsearch.h

stringy.so: stringy.o stringy_test.lua
	$(CC) -shared stringy.o -o $@
	$(LUA) stringy_test.lua

clean:
	rm -f stringy.so stringy.o *.rock
