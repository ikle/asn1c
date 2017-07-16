TARGETS = asn1-test

CFLAGS += -O6

all: $(TARGETS)

clean:
	rm -f *.o *-lexer.[ch] *-parser.[ch] $(TARGETS)

PREFIX ?= /usr/local

install: $(TARGETS)
	install -D -d $(DESTDIR)/$(PREFIX)/bin
	install -s -m 0755 $^ $(DESTDIR)/$(PREFIX)/bin

%-lexer.c %-lexer.h: %-lexer.l
	flex -o $(patsubst %.l,%.c, $<) \
		--header-file=$(patsubst %.l,%.h, $<) $<

%-parser.c %-parser.h: %-parser.y
	bison -o $(patsubst %.y,%.c, $<) \
		--defines=$(patsubst %.y,%.h, $<) $<

asn1-lexer.o: asn1-parser.h
asn1-test: se.o asn1-lexer.o asn1-parser.o
