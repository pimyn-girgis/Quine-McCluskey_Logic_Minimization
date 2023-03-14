#ifndef QM_ALGORITHM_UTIL_H
#define QM_ALGORITHM_UTIL_H

#include <string>
#include <iostream>
#include <algorithm>
#include <cstring>
#include <iostream>
#include <set>
#include <stack>
#include <vector>

class util {
   public:
    static char flip_case(char c);
    static bool is_valid_input(const std::string& func);
    static std::string remove_spaces(const std::string& str);
    static bool is_valid_func(const std::string &str);
    static bool alphabetical_sort(char a, char c);
    static void ensure_str_unique(std::string &str);
    static void ensure_vec_unique(std::vector<std::string>& vec);
};

#endif  //QM_ALGORITHM_UTIL_H
