include ../benchmarks.mk

TESTNAME = dekker-fences

all: $(TESTNAME).o

$(TESTNAME).o: $(TESTNAME).cc
	$(CXX) -o $@ $< $(CXXFLAGS) $(LDFLAGS)

clean:
	rm -f $(TESTNAME) *.o
