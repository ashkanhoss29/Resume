Ashkan Hossein
CS 375
03/30/2014


-- Files --
README        - this file
main.cpp      - program
SampleOutput1 - the output for one run of the program
SampleOutput2 - output for another run of the program
SampleOutput3 - output for yet another run of the program


-- Running --
Compile and run using a c++ compiler
This has only been tested on Visual Studio 2012


-- Notes --
/** Asymptotic Analysis Comparison **/

The asysmptotic analysis of the complexity states about O(m^3) for DFA creation
and O(n) for search. Together we should get O(n + m^3).

Since the length of the search string (m) stays the same, and only n changes, we 
expect the difference to be about twice the runtime for 200,000 compared to 100,00.

Running both shows roughly linear difference.
However I couldn't find a multi-platform way to find time in milliseconds 
for accurate results


-- Please check source code for more information --

