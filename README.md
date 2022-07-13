### Pre-requisites
* Linux environment
* Python3.x, Pip
* pip / anaconda installer for python libraries
* g++ compiler
* Make

### Dependencies
* Python networkx, prettytable, tabulate  package
* Z3


## Directory Structure
* `run`: script to run a single program using FenSying
* `README.md`: readme file for the project
* `model-checker`: model checker used in the tool to generate buggy traces. This folder contains source code of CDSChecker twiced to support FenSying
* `src`: source code for FenSying. 
* `tests`: litmus tests and benchmarks. This folder also container test scripts to run entire set of benchmarks and litmus tests.


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
Alternatively, you can also use `main.py` in `src` folder as follows:
```
python3 main.py -f <filename containing main function including path>
```
The ouput file will be generated in the source path with a `_fixed` appended to its name.

Alternatively, you can use `run_` script in `tests/`


Note: Check sample files in `tests/litmus/` and `tests/benchmarks/` folders for the supported input file formats
