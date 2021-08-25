# primes-parallel
### Description:
This program generates all prime numbers from 2 to N (inclusive) and is written in MPI, a parallel programming language.

The number of processes generated (to increase paralellization) can be adjusted in the compilation statement.

### Input and Output:
The input to be entered into the standard console is the number you would like to generate primes up to.

The ouptut program is a text file N.txt, where N is the number entered as an argument to 
the program. This file contains all the primes from 2 to N (inclusive).

ex.
- Input: 20
- Output: 2 3 5 7 11 13 17

The program will also output the number of primes that were generated and the time the parallel code took to run, in seconds.



