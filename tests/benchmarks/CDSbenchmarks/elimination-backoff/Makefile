include ../benchmarks.mk

TESTNAME = stack

all: $(TESTNAME).o

$(TESTNAME).o: $(TESTNAME).cc
	$(CXX) -o $@ $< $(CXXFLAGS) $(LDFLAGS)

clean:
	rm -f $(TESTNAME) *.o
