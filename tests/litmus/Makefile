BASE := ../../model-checker/cds-checker

SUBDIRS := weak_fensying #$(wildcard */.)

OBJECTS := $(patsubst %.c, %.o, $(wildcard *.c))
OBJECTS += $(patsubst %.cc, %.o, $(wildcard *.cc))

include $(BASE)/common.mk

DEPS := $(join $(addsuffix ., $(dir $(OBJECTS))), $(addsuffix .d, $(notdir $(OBJECTS))))

CPPFLAGS += -I$(BASE) -I$(BASE)/include

all: $(OBJECTS)
	for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir; \
	done

-include $(DEPS)

%.o: %.c
	-$(CC) -MMD -MF $(@D)/.$(@F).d -o $@ $< $(CPPFLAGS) -L$(BASE) -l$(LIB_NAME)

%.o: %.cc
	-$(CXX) -MMD -MF $(@D)/.$(@F).d -o $@ $< $(CPPFLAGS) -L$(BASE) -l$(LIB_NAME)

clean:
	rm -f *.o *_fixed.cc  *_fixed.c
	for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir clean; \
	done
