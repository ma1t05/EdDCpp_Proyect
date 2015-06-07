
# Directorys
OUTDIR	= ../out
SRCDIR = src
INCDIR = include

# Compiler
CC = gcc -O0

# Compiler options
COPT = -m32 -fPIC -fno-strict-aliasing

# Link options
CLNFLAGS = -m32 -lm -lpthread
CFLAGS = $(COPT) $(CLNFLAGS) -I$(INCDIR)

OBJSET = \
	$(OUTDIR)/main.o \
	$(OUTDIR)/mapa.o \
	$(OUTDIR)/arrdin.o \
	$(OUTDIR)/point.o \
	$(OUTDIR)/uniform.o \
	$(OUTDIR)/B-tree.o \
	$(OUTDIR)/random_selection.o \
	$(OUTDIR)/grid.o \
	$(OUTDIR)/arrpoint.o
# ------------------------------------------------------------

$(OUTDIR)/main.out: $(OBJSET)
	$(CC) $(CFLAGS) -o $(OUTDIR)/main.out $(OBJSET)
$(OUTDIR)/main.o: $(SRCDIR)/main.c
	$(CC) -c $(CFLAGS) $(SRCDIR)/main.c -o $(OUTDIR)/main.o

$(OUTDIR)/mapa.o: $(SRCDIR)/mapa.c
	$(CC) -c $(CFLAGS) $(SRCDIR)/mapa.c -o $(OUTDIR)/mapa.o

$(OUTDIR)/arrdin.o: $(SRCDIR)/arrdin.c
	$(CC) -c $(CFLAGS) $(SRCDIR)/arrdin.c -o $(OUTDIR)/arrdin.o

$(OUTDIR)/point.o: $(SRCDIR)/point.c
	$(CC) -c $(CFLAGS) $(SRCDIR)/point.c -o $(OUTDIR)/point.o

$(OUTDIR)/uniform.o: $(SRCDIR)/uniform.c
	$(CC) -c $(CFLAGS) $(SRCDIR)/uniform.c -o $(OUTDIR)/uniform.o

$(OUTDIR)/B-tree.o: $(SRCDIR)/B-tree.c
	$(CC) -c $(CFLAGS) $(SRCDIR)/B-tree.c -o $(OUTDIR)/B-tree.o

$(OUTDIR)/random_selection.o: $(SRCDIR)/random_selection.c
	$(CC) -c $(CFLAGS) $(SRCDIR)/random_selection.c -o $(OUTDIR)/random_selection.o

$(OUTDIR)/grid.o: $(SRCDIR)/grid.c
	$(CC) -c $(CFLAGS) $(SRCDIR)/grid.c -o $(OUTDIR)/grid.o

$(OUTDIR)/arrpoint.o: $(SRCDIR)/arrpoint.c
	$(CC) -c $(CFLAGS) $(SRCDIR)/arrpoint.c -o $(OUTDIR)/arrpoint.o

.PHONY: clean

clean:
	rm -f $(OUTDIR)/*.o