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
./QM/QM_Algorithm < QM/in.txt > QM/out.txt
```
where in.txt is the input, and out.txt is the output.

## Generate and run test cases
compile using gcc
```
gcc tc_gen.c -o tc_gen.out
```
run the generator
```
.\tc_gen.out n
```
where n is the number of variables

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
## Getting the prime implicants using the canonical SoP representation of the function
- The function takes as input a set of minterms and the number of variables in the Boolean function.
- Implicant struct has an integer 'imp' that represenets the implicant itself where the dashes are always replaced with ones. The other integer is named dash location which sets the dash locations in the implicant to 1 so that we know their location. The struct has a boolean 'is_combined' that is used in the middle of the process to determine whether this implicant is combined with another one or not. Finally, the struct has a list of all covered minterms by this implicant.
- It initializes two vectors of implicants, pi_table and tmp_table, with size equal to the number of variables plus one.
- It populates pi_table with the minterms, using the '_builtin_popcount' function to count the number of ones in the binary representation of each minterm. Each minterm is stored in an implicant object along with its corresponding bitmask, dash location, and list of covered minterms.
- It then enters a loop that runs until no more implicants can be combined.
- The loop then iterates over each implicant in pi_table and tries to combine it with other implicants in pi_table that have one more one in their binary representation. If a combination is possible, it checks whether the resulting implicant already exists in exists, which is a hash table of implicants that have already been generated. If it does exist, the is_combined flag is set to true and the two implicants are marked as combined. If it does not exist, the new implicant is added to tmp_table along with its covered minterms, and the exists hash table is updated with the new implicant. The two implicants are then marked as combined, and the is_combined flag is set to true.
- Note that the hashing function hashes a pair of integers : the implicant itself and the dash location using a super fast modified 'bitwise encoding' hashing method. Using this method decreased the running time of the code by more than 40 times.
- After all possible combinations have been attempted, any implicants that were not marked as combined are added to the prime_implicants vector.
- The exists hash table is then cleared, and pi_table is replaced with tmp_table. tmp_table is resized to have size equal to the number of variables.
- The loop repeats until no more implicants can be combined.
## Getting essential and non essential prime implicants
- The code has four main steps:
- (1) Iterate through each prime implicant in prime_implicants and add its covered minterms to coverage_chart and uncovered_minterms.
- (2) Iterate through each entry in coverage_chart and check if there is only one prime implicant that covers the minterm. If so, add the corresponding prime implicant to essential_prime_implicants set.
- (3) Iterate through each essential prime implicant in essential_prime_implicants set and remove its covered minterms from uncovered_minterms set.
- (4) Iterate through each prime implicant in prime_implicants and check if it is not present in essential_prime_implicants set. If so, add it to non_essential_prime_implicants set.
- The purpose of the code is to identify essential and non-essential prime implicants in a logical expression. Essential prime implicants are those that are necessary to cover all minterms in the expression, while non-essential prime implicants can be removed without affecting the expression's truth table.
- The function uses sets to store prime implicants, essential prime implicants, and non-essential prime implicants. It also uses an unordered map to store coverage information for each minterm.
## Printing the minimized boolean expression
- At this point, we have all the prime implicants PIs, essential prime implicants EPIs, and the covered/uncovered minterms.
- By definition, the minimized function should include all the EPIs and, if there are uncovered minterms, the prime implicants covering the remaining uncovered minterms.
- Since we already store the EPIs, returning them is trivial.
- Then, we iterate over the prime implicants to choose the ones covering the maximum number of the uncovered minterms.
- Given all EPIs and the PIs covering the remaining minterms, we construct and print our minimized function. 

Project by:
- Bemen Girgis 900213066
- Ziad Hassan 900213728
- Salma Khalil 900213357
