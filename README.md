# Quine-McCluskey_Logic_Minimization
Implementing the Quine-McCluskey Logic Minimization Algorithm for CSCE 2301 taught by Dr. Mohamed Shalan Spring 2023 at AUC.

## Build and Run
Create the make files
```
cmake -BQM
```
This will create a directory named QM

Now build the project
```
cmake --build QM --target all
```
Now to run an instance of the program just type in
```
./QM/QM_Algorithm
```
Large input may be difficult to display on the terminal, thus, we advise you save the output to a file using the following command.
```
./QM/QM_Algorithm < in.txt > out.txt
```
where in.txt is the input, and out.txt is the output.

## Input:
Input is implemented using C++ IO streams according to the following rules:
- Since the input should be in the form of SoP, it was assumed that no AND operator will be used and thus the validation only accepts two operators + (for OR) and ' (for negation)
- No operator can be at the start of the function
- Negation can be repeated multiple times
- The OR operator cannot be followed by another operator.
- An or operator at the end of the function will be dismissed.
- any spaces in the function is removed i.e. "a bc" is converted to "abc"
- Only allowed variable names are 'a' through 'j' (maximum 10 variables)
- User is prompted the position and value of the character where an error is discovered.
- To make the design easier, if a term in the input is always evaluated to false, it is omitted from the expression; e.g. aa' + ab will be translated to ab.
- A user can manually set the var count by adding a false term; e.g. the function ab + ee' will evaluate to a function with 5 variables, while ab will evaluate to a function with only two variables

## Data Storage and Representation:
- The main data structure used for storing a function is an std::vector\<std::string\>
- Internally, variables will be lowercase letters, while their negation will be uppercase letters.
- minterms and maxterms are kept inside their respective std::vector\<int\>.
- The truth table is represented using an std::vector\<char\>. An std::vector\<bool\> was considered but the performance issues resulting from it cannot be ignored.

## Initialization
- After the input is inserted and validated, the variable count and the size of the truth table are determined.
- After that, the function is parsed and each term is extracted individually and added to an std::vector\<std::string\>
- Characters of the each term are sorted alphabetically, while the terms of the SoP are sorted lexicographically.
- It is ensured that no characters are repeated inside a string, and no terms are repeated in the function.

## Setting the truth table
  - The truth table is then set by going through the products one by one.
  - For each product we go through the variables one by one (e.g. for the term $bc$ in a 4 variable expression, we go through $a, b, c,$ and $d$).
  - Since the terms are sorted, checking if a variable exists or not is trivial!
  - If a variable's truth value is specified, we determine it and move on to the next one. Otherwise, we branch our program to consider both cases when it's true and when it's false.
  
## Calculating min/max terms and SoP/PoS
- Calculating the min/max terms is trivial given the truth table.
- We simply go through the table element by element, if it is set to true, we add it to the vector of minterms otherwise, we add it to the vector of maxterms.
- Given the min/max terms, we simply translate the terms into characters and store them in the SoP/PoS.


## Printing the minimized boolean expression
- At this point, we have all the prime implicants PIs, essential prime implicants EPIs, and the covered/uncovered minterms.
- By definition, the minimized function should include all the EPIs and, if needed, the prime implicants covering the remaining uncovered minterms.
- Since we already store the EPIs, returning them is trivial.
- Then, we iterate over the prime implicants to choose the ones covering the maximum number of the uncovered minterms.
- Given all EPIs and the PIs covering the remaining minterms, we construct and print our minimized function. 
