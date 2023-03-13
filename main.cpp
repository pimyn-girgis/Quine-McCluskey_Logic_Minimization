#include "bool_func.h"
#include <iostream>

int main()
{
    bool_func func;
    while(std::cin >> func)
        std::cout << func << '\n';
    return 0;
}
