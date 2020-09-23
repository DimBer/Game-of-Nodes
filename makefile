# Makefile

IDIR = include
CC = g++
CFLAGS = -I$(IDIR) -std=c++14 -Wall  -g -O3 -fopenmp -lboost_program_options

SDIR = src
ODIR = obj

_SOURCES = creature.cpp evo.cpp utils.cpp
SOURCES = $(patsubst %,$(SDIR)/%,$(_SOURCES))

_OBJECTS = $(_SOURCES:.cpp=.o)
OBJECTS = $(patsubst %,$(ODIR)/%,$(_OBJECTS))

_DEPS = creature.h opts.h utils.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

EXE = evo

$(ODIR)/%.o: $(SDIR)/%.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(EXE): $(OBJECTS)
	$(CC) -o $@ $^ $(CFLAGS)
	rm -f $(ODIR)/*.o
