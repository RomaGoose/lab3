CC=gcc
TARGET=lab3.exe
CFLAGS=-g

SRCDIR=src
OBJDIR=build
INCLUDEDIR=includes

SRC=main.c container.c article.c selection_sort.c parse_args.c
OBJ=$(SRC:%.c=%.o)

SRCS=$(SRC:%=$(SRCDIR)/%)
OBJS=$(OBJ:%=$(OBJDIR)/%)

DELOBJS=OBJS
CLEANCMD=rm -f

ifeq ($(OS),Windows_NT)
 	DELOBJS=$(OBJ:%=$(OBJDIR)\\%)
	CLEANCMD=del /f
endif

$(TARGET): $(OBJS)
	$(CC) $^ -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -I$(INCLUDEDIR) -c $^ -o $@

.PHONY: clean cleanall printaaa

clean:
	$(CLEANCMD) $(TARGET)
	
cleanall: clean
	$(CLEANCMD) $(DELOBJS)
