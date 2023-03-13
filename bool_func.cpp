#include "bool_func.h"

std::istream &operator>>(std::istream &is, bool_func &obj)
{
    obj.sum_of_products.clear();
    std::string input;
    do {
        getline(is, input);
        input = bool_func::remove_spaces(input);
    } while(!bool_func::is_valid_func(input));

    const char *sep = "+";
    for(char *product = strtok(&input.front(), sep); product; product = strtok(nullptr, sep))
    {
        obj.sum_of_products.push_back(product);
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
           || (func[i - 1] == '+')) {   //operator + cannot be followed by another operator
            std::cout << "Invalid input at character " << i + 1 << ": " << c << '\n';
            return false;
        }
    }

    return true;
}

char bool_func::flip_case(const char c) {
    return c + ('a' <= c && c <= 'z' ? 'A' - 'a' : 'a' - 'A');
}

std::string bool_func::remove_spaces(const std::string &str)
{
    std::string temp; temp.reserve(str.size());

    const int len = str.size();
    for(int i = 0; i < len; ++i) {

        if (!isspace(str[i]))
            temp.push_back(str[i]);
    }

    return temp;
}
