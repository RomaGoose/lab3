CC=gcc
TARGET=lab3.exe
CFLAGS=-c -g 

SRCDIR=src
OBJDIR=build
INCLUDEDIR=includes

SRC=main.c container.c article.c selection_sort.c parse_args.c generate.c
OBJ=$(SRC:%.c=%.o)

SRCS=$(SRC:%=$(SRCDIR)/%)
OBJS=$(OBJ:%=$(OBJDIR)/%)

DELOBJS=OBJS
CLEANCMD=rm -f
MKDIRCMD=mkdir -p $(OBJDIR)

ifeq ($(OS),Windows_NT)
 	DELOBJS=$(OBJ:%=$(OBJDIR)\\%)
	CLEANCMD=del /f
	MKDIRCMD=if not exist $(OBJDIR) mkdir $(OBJDIR)
endif

$(TARGET): $(OBJS)
	$(CC) $^ -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR) 
	$(CC) $(CFLAGS) -I$(INCLUDEDIR) $^ -o $@

.PHONY: clean cleanall printaaa

$(OBJDIR):
	$(MKDIRCMD)

clean:
	$(CLEANCMD) $(TARGET)
	
cleanall: clean
	$(CLEANCMD) $(DELOBJS)
