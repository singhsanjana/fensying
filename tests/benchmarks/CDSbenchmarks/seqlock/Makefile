include ../benchmarks.mk


include ../benchmarks.mk

TESTNAME = $(patsubst %.cc, %.o, $(wildcard *.cc))

all: $(TESTNAME)

%.o: %.cc
	$(CXX) -o $@ $< $(CXXFLAGS) $(LDFLAGS)

clean:
	rm -f *_fixed.* *.o