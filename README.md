# SP Exam Solution
1. ✅ Provide types and operator overloads to support the reaction rule typesetting directly in C++ code.

`./include/stochastic/reaction.hpp` and `./src/reaction.cpp`

2. ✅ Use visitor pattern to print the reaction network in a network graph format (e.g. Fig. 4).

`./include/stochastic/visitor.hpp`,`./include/stochastic/graph_visitor.hpp` and `./src/graph_visitor.cpp`

Example of dot can be found in `./figures/dot.png`

3. ✅ Implement a generic symbol table to store and lookup objects of user-defined key and value types. Support failure cases when a) the  able does not contain the looked up symbol, b) the table already contains the symbol being added. Demonstrate the usage with reactants. Constrain the supported types based on your data structures. 

`./include/stochastic/symbol_table.hpp`

4. ✅ Implement the stochastic systems simulation algorithm (Alg. 1) using the reaction rules.

`./include/stochastic/simulator.hpp` and `./src/simulator.cpp`

5. ✅ Demonstrate the application of the library on the three examples (shown in Fig. 1, 2, 3).

`./examples/*`

6. ✅ Display simulation plots of the amounts over time. Use external libraries (like Qt or ImGui) to visualize.

`./figures/*`

7. ✅ Implement a generic support for a user-supplied trajectory state observer (allowing deep optimizations) or provide a lazy trajectory generation interface (coroutine). The observer itself should not be part of the library, but part of the user/test program. Demonstrate usage by estimating the peak of hospitalized agents in Covid-19 example without storing an entire trajectory. Record the peak hospitalization values for populations NNJ, NDK.

`./include/stochastic/simulator.hpp` and `./src/simulator.cpp`

8. ✅ Use unit testing framework to provide tests (e.g. for symbol table methods, failure cases, printing).

`./tests/*` 

9. ✅ Support multi-core computation of many simulations at the same time while avoiding data races and oversubscription. Demonstrate by estimating the average value of the hospitalized peak over 100 simulations.

`./benchmark/bench.cpp`

10.✅  Use benchmark framework to measure and compare the stochastic simulation performance (e.g. the time it takes to compute 100 simulations a single core, multiple cores, or improved implementation). Record the timings and make your conclusions.

`./benchmark/bench.cpp`

For N = 100000
-------------------------------------------------------------------------------
Benchmark simulation
-------------------------------------------------------------------------------
/home/mertz/Coding/stochastic/benchmark/bench.cpp:174
...............................................................................

benchmark name                       samples       iterations    est run time
                                     mean          low mean      high mean
                                     std dev       low std dev   high std dev
-------------------------------------------------------------------------------
Simulation Covid 100                           100             1     13.5506 m
                                         4.73289 s     4.72998 s     4.73617 s
                                        15.7206 ms    13.5908 ms    19.0182 ms

Simulation Covid Parallel 100                  100             1     1.06732 m
                                        656.948 ms     650.92 ms    664.389 ms
                                        33.9444 ms    28.6823 ms    40.7639 ms

Circadian naive                                100             1     3.27638 s
                                        33.5481 ms    33.1589 ms    34.0244 ms
                                        2.19662 ms    1.81219 ms    2.76972 ms

Circadian fast                                 100             1     2.87539 s
                                        28.3956 ms    28.0836 ms    28.7866 ms
                                        1.77896 ms     1.4356 ms    2.48757 ms

Large circular chain simulation
naive                                          100             1     1.20375 s
                                        11.9742 ms    11.9184 ms    12.0718 ms
                                        368.171 us    237.042 us    522.589 us

Large circular chain simulation
fast                                           100             1    361.572 ms
                                        3.60077 ms    3.59346 ms     3.6091 ms
                                        39.9556 us    33.8626 us    52.4096 us
