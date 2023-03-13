//
// Created by Bemen on 13/03/2023.
//

#include "bool_func.h"

std::istream &operator>>(std::istream &is, bool_func &obj)
{
    std::string input;
    do {
        getline(is, input);
    } while(!bool_func::is_valid_func(input));

    const char *sep = "+ \t\n";
    for(char *product = strtok(&input.front(), sep); product; product = strtok(nullptr, sep))
    {
        obj.sum_of_products.emplace_back(product);
    }

    return is;
}

std::ostream &operator<<(std::ostream &os, const bool_func &obj)
{
    os << "Function: ";

    const int len = obj.sum_of_products.size();
    for(int i = 0; i < len; ++i)
    {
        std::cout << obj.sum_of_products[i] << (i == len - 1 ? '\n' : '+');
    }

    return os;
}
bool bool_func::is_valid_func(const std::string &func)
{
    const int len = func.size();

    for(int i = 0; i < len; ++i)
    {
        char c = func[i];

        if('a' <= c && c < 'a' + 10 || isspace(c))
            continue;

        //Otherwise, char is an operator
        if((i == 0)                     //The first character in the function cannot be an operator
           || (c != '\'' && c != '+')   //Check if operator is valid operator
           || (i == len - 1 && c == '+')//Last char in function cannot be a start of a new expression
           || (func[i - 1] == '+'))     //operator + cannot be followed by another operator
            return false;
    }

    return true;
}
char bool_func::flip_case(const char c)
{
    return c + ('a' <= c && c <= 'z' ? 'A' - 'a' : 'a' - 'A');
}
