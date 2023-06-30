include ../benchmarks.mk

TESTNAME = $(patsubst %.cc, %.o, $(wildcard *.cc))

all: $(TESTNAME)

%.o: %.cc
	$(CC) -o $@ $< $(CFLAGS) $(LDFLAGS)

clean:
	rm -f *_fixed.* *.o
