CC=gcc
TARGET=lab3.exe
CFLAGS=-c -g
LOGLVL=0

SRCDIR=src/
OBJDIR=build/
INCLUDEDIR=includes

SRC=app/main.c container/container.c core/article.c app/parse_args.c core/generate.c core/io.c
OBJ=$(SRC:%.c=$(OBJDIR)%.o)

$(TARGET): $(OBJ)
	$(CC) $^ -o $@

$(OBJDIR)%.o: $(SRCDIR)%.c | $(OBJDIR) 
	$(CC) $(CFLAGS) -I$(INCLUDEDIR) -DLOGLVL=$(LOGLVL) $< -o $@


WINOBJDIR=$(subst /,\,$(OBJDIR))
WINOBJ=$(subst /,\,$(OBJ))
ifeq ($(OS),Windows_NT)
	MKDIRCMD=if not exist $(WINOBJDIR) mkdir $(WINOBJDIR)app $(WINOBJDIR)container $(WINOBJDIR)core 
	CLEANCMD=del /f /q
else
	MKDIRCMD=mkdir -p $(OBJDIR)app $(OBJDIR)container $(OBJDIR)core 
    CLEANCMD=rm -f
endif
	

.PHONY: clean cleanall $(OBJDIR)

$(OBJDIR):
	$(MKDIRCMD)

clean:
	$(CLEANCMD) $(TARGET)
	
cleanall: clean
	$(CLEANCMD) $(WINOBJ)
