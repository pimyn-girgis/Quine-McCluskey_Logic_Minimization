# Quine-McCluskey_Logic_Minimization
Implementing the Quine-McCluskey Logic Minimization Algorithm for CSCE 2301 taught by Dr. Mohamed Shalan Spring 2023 at AUC.

## Input:
Input is implemented using C++ IO streams according to the following rules:
- Since the input should be in the form of SoP, it was assumed that no AND operator will be used and thus the validation only accepts two operators + (for OR) and ' (for negation)
- No operator can be at the start of the function
- Negation can be repeated multiple times
- The OR operator cannot be followed by another operator.
- An or operator at the end of the function will be omitted
- any spaces in the function is removed i.e. "a b     c" is converted to "abc"
- Only allowed variable names are 'a' through 'j' (maximum 10 variables)
- User is prompted the position and value of the character where an error is discovered.

## Data Storage and Representation:
- The main data structure used for storing a function is a dynamic array (std::vector) of dynamic character arrays (std::string)
- Internally, variables will be lowercase letters, while their negation will be uppercase letters.
