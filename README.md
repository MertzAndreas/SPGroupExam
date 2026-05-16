# SP Exam Solution
1. ✅ Provide types and operator overloads to support the reaction rule typesetting directly in C++ code.
2. Use visitor pattern to print the reaction network in a network graph format (e.g. Fig. 4).
3. ✅ Implement a generic symbol table to store and lookup objects of user-defined key and value types. Support failure cases when a) the  able does not contain the looked up symbol, b) the table already contains the symbol being added. Demonstrate the usage with reactants. Constrain the supported types based on your data structures. 
4. Implement the stochastic systems simulation algorithm (Alg. 1) using the reaction rules.
5. Demonstrate the application of the library on the three examples (shown in Fig. 1, 2, 3).
6. Display simulation plots of the amounts over time. Use external libraries (like Qt or ImGui) to visualize.
7. Implement a generic support for a user-supplied trajectory state observer (allowing deep optimizations) or provide a lazy trajectory generation interface (coroutine). The observer itself should not be part of the library, but part of the user/test program. Demonstrate usage by estimating the peak of hospitalized agents in Covid-19 example without storing an entire trajectory. Record the peak hospitalization values for populations NNJ, NDK.
8. Use unit testing framework to provide tests (e.g. for symbol table methods, failure cases, printing).
9. Support multi-core computation of many simulations at the same time while avoiding data races and oversubscription. Demonstrate by estimating the average value of the hospitalized peak over 100 simulations.
10. Use benchmark framework to measure and compare the stochastic simulation performance (e.g. the time it takes to compute 100 simulations a single core, multiple cores, or improved implementation). Record the timings and make your conclusions.
