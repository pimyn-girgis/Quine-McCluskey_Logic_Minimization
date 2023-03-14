#include "util.h"

char util::flip_case(char c) {
    return char(c + ('a' <= c && c <= 'z' ? 'A' - 'a' : 'a' - 'A'));
}
bool util::is_valid_input(const std::string& func) {
    const int len = int(func.size());

    for (int i = 0; i < len; ++i) {
        char c = func[i];

        if ('a' <= c && c < 'a' + 10 || isspace(c))
            continue;

        //Otherwise, char is an operator
        if ((i == 0)                    //The first character in the function cannot be an operator
            || (c != '\'' && c != '+')  //Check if operator is valid operator
            || (func[i - 1] == '+')) {  //operator + cannot be followed by another operator
            std::cout << "Invalid input at character: " << i + 1 << '\n' << func << '\n' << std::string(i, '_') << '|' << '\n';
            return false;
        }
    }

    return true;
}

std::string util::remove_spaces(const std::string& str) {
    const int len = int(str.size());
    std::string temp;
    temp.reserve(len);

    for (int i = 0; i < len; ++i) {
        if (!isspace(str[i]))
            temp.push_back(str[i]);
    }

    return temp;
}
bool util::is_valid_func(const std::string& str) {
    int taken = 0;

    for (char c : str) {
        if ( (taken & (1 << (tolower(c) - 'a'))) && isupper(c)) {
            return false;
        }

        taken |= (1 << (tolower(c) - 'a'));
    }

    return true;
}
bool util::alphabetical_sort(const char a, const char b) {
    char loa = tolower(a, std::locale());
    char lob = tolower(b, std::locale());
    return loa == lob ? islower(a) : loa < lob;
}
void util::ensure_str_unique(std::string& str) {
    std::sort(str.begin(), str.end(), util::alphabetical_sort);
    auto it = std::unique(str.begin(), str.end());
    str.resize(it - str.begin());
}
void util::ensure_vec_unique(std::vector<std::string>& vec) {
    std::sort(vec.begin(), vec.end());
    auto it = std::unique(vec.begin(), vec.end());
    vec.resize(it - vec.begin());
}
