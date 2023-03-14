#include <iostream>
#include "bool_func.h"

int main() {
    bool_func func;
    while (std::cin >> func) {
        std::cout << func << '\n';

        auto minterms = func.get_minterms();
        std::cout << "Minterms: ";
        for (auto &u : minterms) {
            std::cout << u << ((&u == &minterms.back())? '\n' : ' ');
        }

        auto maxterms = func.get_maxterms();
        std::cout << "Maxterms: ";
        for (auto &u : maxterms) {
            std::cout << u << ((&u == &maxterms.back())? '\n' : ' ');
        }


        auto sop = func.get_canonical_sop();
        std::cout << "Canonical SoP: ";
        for (auto &u : sop) {
            std::cout << u << ((&u == &sop.back())? "\n" : " + ");
        }

        auto pos = func.get_canonical_pos();
        std::cout << "Canonical SoP: ";
        for (auto &u : pos) {
            std::cout << u << ((&u == &pos.back())? "\n" : " * ");
        }

        func.print_truth_table();

    }

    return 0;
}
