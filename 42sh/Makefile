# Add additional .c files here if you added any yourself.
ADDITIONAL_SOURCES =

# Add additional .h files here if you added any yourself.
ADDITIONAL_HEADERS =

# -- Do not modify below this point - will get replaced during testing --
TARGET = 42sh
SOURCES = front.c shell.c parser.c ast.c lex.yy.c mc.c $(ADDITIONAL_SOURCES)
HEADERS = parser.h lexer.h lex.yy.h ast.h xalloc.h shell.h mc.h front.h $(ADDITIONAL_HEADERS)
GENERATED = parser.c parser.h lex.yy.h lex.yy.c
META = parser.y lexer.l lemon.c lempar.c README.rst

CFLAGS = -W -Wall -std=c11 -g -D_POSIX_C_SOURCE=200809L
LDFLAGS =
LIBS = -lreadline -lhistory

CC = gcc
LEX = flex --header-file=lex.yy.h
RST2PDF = rst2pdf -s twelvepoint --footer='\#\#\#Page\#\#\#/\#\#\#Total\#\#\#' --real-footnotes

.PHONY: all tarball clean moreclean

all: $(TARGET)

tarball: 42sh.tar.gz

42sh.tar.gz: $(GENERATED) $(SOURCES) $(HEADERS) $(META) AUTHORS Makefile
	tar -czf $@ $^

check: 42sh.tar.gz
	curl --insecure -F "file=@42sh.tar.gz" http://51.15.56.180/bs2017/upload1/

clean:
	rm -f $(TARGET)
	rm -f *.o *~ *.out
	rm -f *.aux *.log *.ltx

moreclean: clean
	rm -f $(GENERATED)
	rm -f lemon

$(TARGET): $(SOURCES:.c=.o)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LIBS)

$(SOURCES:.c=.o): $(HEADERS)

LEMON = ./lemon

$(LEMON): lemon.c
	$(CC) $(CFLAGS) -o $@ $<

parser.h: parser.c
parser.c: parser.y $(LEMON)
	$(LEMON) $<


lex.yy.h lex.yy.c: lexer.l
	$(LEX) $<


.SUFFIXES: .pdf .rst
doc: README.pdf


.rst.pdf:
	$(RST2PDF) $<
