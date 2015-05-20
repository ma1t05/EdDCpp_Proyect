
# Directorys
OUTDIR	= ../../../out

# Compiler
CC = gcc -O0

# Compiler options
COPT = -m32 -fPIC -fno-strict-aliasing

# Link options
CLNFLAGS = -m32 -lm -lpthread
CFLAGS = $(COPT) $(CLNFLAGS)

OBJSET = \
	$(OUTDIR)/main.o \
	$(OUTDIR)/mapa.o \
	$(OUTDIR)/arrdin.o \
	$(OUTDIR)/point.o \
	$(OUTDIR)/uniform.o
# ------------------------------------------------------------

main: $(OBJSET)
	$(CC) $(CFLAGS) -o $(OUTDIR)/main.out $(OBJSET)
$(OUTDIR)/main.o: main.c
	$(CC) -c $(CFLAGS) main.c -o $(OUTDIR)/main.o

$(OUTDIR)/mapa.o: mapa.c
	$(CC) -c $(CFLAGS) mapa.c -o $(OUTDIR)/mapa.o

$(OUTDIR)/arrdin.o: arrdin.c
	$(CC) -c $(CFLAGS) arrdin.c -o $(OUTDIR)/arrdin.o

$(OUTDIR)/point.o: point.c
	$(CC) -c $(CFLAGS) point.c -o $(OUTDIR)/point.o

$(OUTDIR)/uniform.o: uniform.c
	$(CC) -c $(CFLAGS) uniform.c -o $(OUTDIR)/uniform.o

.PHONY: clean

clean:
	rm -f $(OUTDIR)/*.o