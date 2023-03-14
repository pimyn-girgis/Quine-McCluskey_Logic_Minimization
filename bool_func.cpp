#include "bool_func.h"

std::istream& operator>>(std::istream& is, bool_func& obj) {
    std::string input;
    do {
        getline(is, input);

        if (input[0] == '0') { //If first character is 0, istream is false.
            is.setstate(1);
            return is;
        }

        input = util::remove_spaces(input); //remove any whitespace in the input.
    } while (!util::is_valid_input(input)); //Check validity of input.

    obj = std::move(bool_func(input)); //create a new object of bool_func and _move_ it to current obj

    return is;
}

std::ostream& operator<<(std::ostream& os, const bool_func& obj) {
    os << "Function: ";

    const int len = int(obj.func.size());
    for (int i = 0; i < len; ++i) {
        os << obj.func[i];
        if (i != len - 1)
            os << '+';
    }

    return os;
}

//
int bool_func::get_var_count() const {
    return var_count;
}

void bool_func::set_var_count(const std::string& input) {
    var_count = *std::max_element(input.begin(), input.end()) - 'a' + 1;
}

std::vector<std::string> bool_func::get_canonical_sop() {
    return sop;
}

std::vector<std::string> bool_func::get_canonical_pos() {
    return pos;
}

void bool_func::set_canonical_sop() {

    sop.reserve(minterms.size());

    char product[11]; int len;
    for (auto u : minterms) {
        len = 0;
        for (int i = 0; i < get_var_count(); ++i) {
            product[len++] = char(i + ((u & (1 << i))? 'a' : 'A'));
        }
        product[len] = 0;
        sop.emplace_back(product);
    }

}

void bool_func::set_canonical_pos() {
    char sum[22] = "("; int len;
    pos.reserve(maxterms.size());

    for (auto u : maxterms) {
        len = 1;

        for (int i = 0; i < get_var_count(); ++i) {
            sum[len++] = char(i + ((u & (1 << i))? 'A' : 'a'));
            sum[len++] = '+';
        }

        sum[len - 1] = ')';
        sum[len] = 0;

        pos.emplace_back(sum);
    }
}

std::vector<int> bool_func::get_minterms() {
    return minterms;
}

void bool_func::set_truth_table(char c, int product_index, int i, int value) {

    if (product_index == func.size()) {
        return;
    }

    if (c == 'a' + var_count) {
        truth_table[value] = true;
        set_truth_table('a', product_index + 1, 0, 0);
        return;
    }

    char cur = func[product_index][i];
    if (cur == c) {
        value |= (1 << (c - 'a'));
        set_truth_table(char(c + 1), product_index, i + 1, value);
    } else if (cur == util::flip_case(c)) {
        set_truth_table(char(c + 1), product_index, i + 1, value);
    } else {
        set_truth_table(char(c + 1), product_index, i, value);
        set_truth_table(char(c + 1), product_index, i, value | (1 << (c - 'a')));
    }
}

bool_func::bool_func(const std::string& str) : var_count(0) {
    set_var_count(str);
    init_truth_table();

    parse_func(str + '+');
    set_truth_table();
    set_min_max_terms();
    set_canonical_sop();
    set_canonical_pos();
}

bool_func::bool_func() : var_count(0) { }

bool bool_func::is_minterm(int i) {
    return truth_table[i];
}

void bool_func::set_min_max_terms() {
    int len = 1 << get_var_count();
    for (int i = 0; i < len; ++i) {
        if (is_minterm(i)) {
            minterms.push_back(i);
        } else {
            maxterms.push_back(i);
        }
    }
}
std::vector<char> bool_func::get_truth_table() {
    return truth_table;
}
std::vector<int> bool_func::get_maxterms() {
    return maxterms;
}
void bool_func::print_truth_table() {
#define print_line \
    for (int VAR = 0; VAR < var_count; ++VAR) { \
        std::cout << "+---"; \
    } std::cout << "+-----+\n";

    print_line
    for (int i = var_count - 1; i >= 0; --i) {
        std::cout << "| " << char('a' + i) << " ";
    } std::cout << "| exp |\n";
    print_line

    std::stack<char> vars;
    for (int i = 0; i < 1 << var_count; ++i) {

        for (int j = 0; j < var_count; ++j) {
            vars.push(i & (1 << j)? '1' : '0');
        }
        for (int j = 0; j < var_count; ++j) {
            std::cout << "| " << vars.top() << " ";
            vars.pop();
        } std::cout << "|  " << is_minterm(i) << "  |\n";

        print_line
    }

}
void bool_func::init_truth_table() {
    truth_table.resize(1 << var_count);
    memset(&truth_table.front(), 0, 1 << var_count);
}
void bool_func::parse_func(const std::string &str) {

    std::string product; product.reserve(10);
    for (const char *temp = &str.front(); *temp; ++temp) {
        switch (*temp) {
            case '\'': //If char is negation, negate the last variable
                product.back() = util::flip_case(product.back());
                break;
            case '+': //If char is +, we are done with the current term, insert it.
                if (product.empty()) break;
                util::ensure_str_unique(product);
                if (util::is_valid_func(product)) func.push_back(product);
                else std::cout << "Product " << product << " omitted; expression always evaluates to false!\n";
                product.clear();
                break;
            default: //Otherwise it's a character and part of our term
                product.push_back(*temp);
        }
    }

    util::ensure_vec_unique(func);
}
