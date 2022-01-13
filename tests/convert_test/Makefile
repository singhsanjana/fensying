INDIR := not_converted
OUTDIR := converted


.PHONY: all

OBJECTS := $(patsubst $(INDIR)/%.cpp, $(OUTDIR)/%.cc, $(wildcard $(INDIR)/*/*.cpp))

all: $(OBJECTS)

$(filter %.cc,$(OBJECTS)): $(OUTDIR)/%.cc: $(INDIR)/%.cpp
	echo "target: $@ prereq: $<"
	./convert.py $< -o $@



