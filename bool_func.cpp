#include "bool_func.h"

std::istream& operator>>(std::istream& is, bool_func& obj) {
    obj.func.clear();
    std::string input;
    do {
        getline(is, input);

        if (input[0] == '0') {
            is.setstate(1);
            return is;
        }

        input = util::remove_spaces(input);
    } while (!util::is_valid_input(input));

    obj = std::move(bool_func(input));

    return is;
}

std::ostream& operator<<(std::ostream& os, const bool_func& obj) {
    os << "Function: ";

    const int len = obj.func.size();
    for (int i = 0; i < len; ++i) {
        os << obj.func[i];
        if (i != len - 1)
            os << '+';
    }

    return os;
}

//
const int bool_func::get_var_count() const {
    return var_count;
}

void bool_func::set_var_count(const std::string& input) {
    var_count = *std::max_element(input.begin(), input.end()) - 'a' + 1;
    truth_table.resize(1 << var_count);
    memset(&truth_table.front(), 0, 1 << var_count);
}

std::vector<std::string> bool_func::get_canonical_sop() {
    return sop;
}

std::vector<std::string> bool_func::get_canonical_pos() {
    return pos;
}

void bool_func::set_canonical_sop() {

    char product[11]; int len = 0;
    sop.reserve(minterms.size());

    for (auto u : minterms) {
        len = 0;
        for (int i = 0; i < get_var_count(); ++i) {
            product[len++] = i + ((u & (1 << i))? 'a' : 'A');
        }
        product[len] = 0;
        sop.emplace_back(product);
    }

}

void bool_func::set_canonical_pos() {
    char sum[22] = "("; int len = 0;
    pos.reserve(maxterms.size());

    for (auto u : maxterms) {
        len = 1;

        for (int i = 0; i < get_var_count(); ++i) {
            sum[len++] = i + ((u & (1 << i))? 'A' : 'a');
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
        set_truth_table(c + 1, product_index, i + 1, value);
    } else if (cur == util::flip_case(c)) {
        set_truth_table(c + 1, product_index, i + 1, value);
    } else {
        set_truth_table(c + 1, product_index, i, value);
        set_truth_table(c + 1, product_index, i, value | (1 << (c - 'a')));
    }
}

bool_func::bool_func(std::string& str) {
    set_var_count(str);

    std::string product;
    product.reserve(10);
    str += '+';

    const char* temp;
    for (temp = &str.front(); *temp; ++temp) {
        if (*temp == '\'') {
            product.back() = util::flip_case(product.back());
        } else if (*temp == '+' && !product.empty()) {

            std::sort(product.begin(), product.end(), util::alphabetical_sort);
            auto it = std::unique(product.begin(), product.end());
            product.resize(it - product.begin());

            if (util::is_valid_func(product))
                func.push_back(product);
            else
                std::cout << "Product " << product << " omitted; expression always evaluates to false!\n";

            product.clear();
        } else {
            product.push_back(*temp);
        }
    }

    std::sort(func.begin(), func.end());
    auto it = std::unique(func.begin(), func.end());
    func.resize(it - func.begin());

    minterms.clear();
    set_truth_table();
    set_min_max_terms();
    set_canonical_sop();
    set_canonical_pos();
}

bool_func::bool_func() {
    var_count = 0;
}

const bool bool_func::is_minterm(int i) {
    return truth_table[i];
}

void bool_func::set_min_max_terms() {
    for (int i = 0; i < 1 << get_var_count(); ++i) {
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
    for (int i = 0; i < var_count; ++i) { \
        std::cout << "+---"; \
    } std::cout << "+-----+\n";

    print_line;
    for (int i = var_count - 1; i >= 0; --i) {
        std::cout << "| " << char('a' + i) << " ";
    } std::cout << "| exp |\n";
    print_line;

    std::stack<char> vars;
    for (int i = 0; i < 1 << var_count; ++i) {

        for (int j = 0; j < var_count; ++j) {
            vars.push(i & (1 << j)? '1' : '0');
        }
        for (int j = 0; j < var_count; ++j) {
            std::cout << "| " << vars.top() << " ";
            vars.pop();
        } std::cout << "|  " << is_minterm(i) << "  |\n";

        print_line;
    }

}
