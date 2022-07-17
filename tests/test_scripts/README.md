### Contents
* `run_litmus.py`: To run litmus tests other than Tracer's litmus tests
* `run_benchmarks.py`: Run all benchmarks and save result in results folder
* `tracer_lists`: Contains list of Tracer's buggy benchmarks. For convinience, we have split Tracer's buggy benchmarks into 3 lists.
* `run_tracer_litmus.py`: Run Tracer's litmus tests. This file takes a filename as input. The filename must contain space-seperated list of litmus tests to run. These lists can be found in `tracer_lists` folder
* `sanity_tracer_limus.py`: Runs sanity check over Tracer's litmus tests. This requires `_fixed` files for tracer litmus tests


