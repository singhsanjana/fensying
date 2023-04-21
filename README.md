### Pre-requisites
* Linux environment
* Python3.x, Pip
* pip / anaconda installer for python libraries
* g++ compiler
* Make

### Dependencies
* Python packages: networkx, prettytable, tabulate
* Z3


## Directory Structure
* `run`: script to run a single program using FenSying.
* `README.md`: readme file for the project.
* `model-checker`: model checker to generate buggy traces. This folder contains the source code of CDSChecker (http://plrg.ics.uci.edu/software_page/42-2/) modified for FenSying.
* `src`: FenSying source code. 
* `tests`: litmus tests and benchmarks, and additionally test scripts to run entire set of benchmarks and litmus tests.


## Use FenSying Docker
Use `docker pull fensying/fensying:atva22` to pull and use FenSying docker

## Installation
* Install required python libraries:<br/>
```
pip3 install networkx prettytable tabulate
```
* Install Z3:<br/>
```
sudo apt update
sudo apt install z3
```
* We need to install CDSChecker to use it as buggy trace generator
```
cd model-checker/cds-checker/
make
```

## Usage
To run the FenSying tool over some benchmark, run:
```
run <filename containing main function including path>
```
An ouput file will be generated at the test path with an `_fixed` appended to the test name.

Alternatively, you can use `run_benchmarks.py` or `run_litmus.py` script in `tests/test_scripts`.


Note: Check sample files in `tests/litmus/` and `tests/benchmarks/` folders for the supported input file formats
