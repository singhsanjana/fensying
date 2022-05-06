include ../benchmarks.mk

TESTNAME = stack

all: $(TESTNAME)

$(TESTNAME): $(TESTNAME).c
	$(CXX) -o $@ $< $(CXXFLAGS) $(LDFLAGS)

clean:
	rm -f $(TESTNAME) *.o
