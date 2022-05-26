#!/bin/bash

g++ -MMD -MF ./.dekker_simple.o.d -o dekker_simple.o dekker_simple.cc -Wall -g -O3 -I../../../model-checker/cds-checker -I../../../model-checker/cds-checker/include -fpermissive -L../../../model-checker/cds-checker -lmodel
g++ -MMD -MF ./.peterson.o.d -o peterson.o peterson.cc -Wall -g -O3 -I../../../model-checker/cds-checker -I../../../model-checker/cds-checker/include -fpermissive -L../../../model-checker/cds-checker -lmodel