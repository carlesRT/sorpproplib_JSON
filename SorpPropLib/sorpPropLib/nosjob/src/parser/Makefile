all:
CPPFLAGS += -g
CFLAGS += -g
foo: main.o JSON_parser.o
	gcc -o $@ $^

all: foo

clean:
	-rm -f *.o foo
