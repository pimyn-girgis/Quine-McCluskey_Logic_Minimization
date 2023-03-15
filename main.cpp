#include <iostream>
#include "bool_func.h"

int main() {
    std::ios_base::sync_with_stdio(false);

    std::cout << "Enter the function as an SoP, only accepted values are [a-j], + (OR) , \' (Negation)\n"
                 "Number of variables is calculated automatically.\n"
                 "It is assumed that the variables sequentially take lowercase letters \'a\' through \'j\'\n"
                 "To specify a custom size, add a term that contains the last variable and its negation!\n"
                 "For example, ab + dd' will be evaluated as ab but will include variables c & d\n"
                 "Please note that it is not possible to specify a variable count less than the one generated by the "
                 "code or more than 10.\n"
                 "Enter 0 to quit the program\n";

    bool_func func;
    while (std::cin >> func) {
        std::cout << func << '\n';

        auto minterms = func.get_minterms();
        std::cout << "Minterms: ";
        for (auto& u : minterms) {
            std::cout << u << ((&u == &minterms.back()) ? "" : " ");
        }
        std::cout << '\n';

        auto maxterms = func.get_maxterms();
        std::cout << "Maxterms: ";
        for (auto& u : maxterms) {
            std::cout << u << ((&u == &maxterms.back()) ? "" : " ");
        }
        std::cout << '\n';

        auto sop = func.get_canonical_sop();
        std::cout << "Canonical SoP: ";
        for (auto& u : sop) {
            std::cout << u << ((&u == &sop.back()) ? "" : " + ");
        }
        std::cout << '\n';

        auto pos = func.get_canonical_pos();
        std::cout << "Canonical PoS: ";
        for (auto& u : pos) {
            std::cout << u << ((&u == &pos.back()) ? "" : " * ");
        }
        std::cout << '\n';

        func.print_truth_table();
    }
    return 0;
}
