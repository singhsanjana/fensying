import os

BASE_DIR = 'tests/benchmarks/tracer_benchmarks/'
IGNORE_DIR = ['test_scripts']

def make_content():
    content  = '\n\n'
    content += 'SUBDIRS := $(wildcard */.)\n\n'
    content += 'OBJECTS := $(patsubst %.c, %.o, $(wildcard *.c))\n'
    content += 'OBJECTS += $(patsubst %.cc, %.o, $(wildcard *.cc))\n\n'
    content += 'include $(BASE)/common.mk\n\n'
    content += 'DEPS := $(join $(addsuffix ., $(dir $(OBJECTS))), $(addsuffix .d, $(notdir $(OBJECTS))))\n\n'
    content += 'CPPFLAGS += -I$(BASE) -I$(BASE)/include -fpermissive\n\n'
    content += 'all: $(OBJECTS)\n'
    content += '\tfor dir in $(SUBDIRS); do \\\n'
    content += '\t\t$(MAKE) -C $$dir; \\\n'
    content += '\tdone\n\n'
    content += '-include $(DEPS)\n\n'
    content += '%.o: %.c\n'
    content += '\t-$(CC) -MMD -MF $(@D)/.$(@F).d -o $@ $< $(CPPFLAGS) -L$(BASE) -l$(LIB_NAME)\n\n'
    content += '%.o: %.cc\n'
    content += '\t-$(CXX) -MMD -MF $(@D)/.$(@F).d -o $@ $< $(CPPFLAGS) -L$(BASE) -l$(LIB_NAME)\n\n'
    content += 'clean:\n'
    content += '\trm -f *.o *_fixed.cc *_fixed.c .*.o.d \n'
    content += '\tfor dir in $(SUBDIRS); do \\\n'
    content += '\t\t$(MAKE) -C $$dir clean; \\\n'
    content += '\tdone'

    return content

for root, subdirs, files in os.walk(BASE_DIR):
    for dir in subdirs:
        dir_path = os.path.join(root, dir)
        if 'test_scripts' in dir_path:
            continue
        
        cds_base_path = '../' * dir_path.count('/')
        f = open(dir_path + '/Makefile', 'w')
        f.write('BASE := ' + cds_base_path + '../model-checker/cds-checker')
        f.write(make_content())
        f.close()
