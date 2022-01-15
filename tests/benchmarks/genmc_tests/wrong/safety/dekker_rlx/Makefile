BASE := ../../../../../../model-checker/cds-checker

SUBDIRS := $(wildcard */.)

all: $(SUBDIRS)
	for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir; \
	done

clean:
	rm -f *.o *_fixed.cc 
	for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir clean; \
	done