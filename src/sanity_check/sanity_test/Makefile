BASE := ../../../model-checker/cds-checker

OBJECTS := $(patsubst %.c, %.o, $(wildcard *.c))
OBJECTS += $(patsubst %.cc, %.o, $(wildcard *.cc))

include $(BASE)/common.mk

DEPS := $(join $(addsuffix ., $(dir $(OBJECTS))), $(addsuffix .d, $(notdir $(OBJECTS))))

CPPFLAGS += -I$(BASE) -I$(BASE)/include

all: $(OBJECTS)

-include $(DEPS)

%.o: %.c
	-$(CC) -MMD -MF $(@D)/.$(@F).d -o $@ $< $(CPPFLAGS) -L$(BASE) -l$(LIB_NAME)

%.o: %.cc
	-$(CXX) -MMD -MF $(@D)/.$(@F).d -o $@ $< $(CPPFLAGS) -L$(BASE) -l$(LIB_NAME)

clean:
	rm -f *.o *_fixed.cc 