#include "bool_func.h"

bool_func::bool_func() : var_count(0) {}
bool_func::bool_func(std::string& str) : var_count(0) {
    set_var_count(str);
    init_truth_table();
    parse_func(str);
    set_truth_table();
    set_min_max_terms();
    set_canonical_sop();
    set_canonical_pos();
    set_prime_implicants();
    set_essentials_and_non_essentials();
    set_optimized_func();
}
implicant::implicant(int i, int d, bool b) {
    imp=i;
    dash_location=d;
    is_combined=b;
}

std::istream& operator>>(std::istream& is, bool_func& obj) {
    std::string input;
    do {
        getline(is, input);

        if (input[0] == '0') {  //If first character is 0, istream is false.
            //is.setstate(1);
            exit(0);
            return is;
        }

        input = util::remove_spaces(input);  //remove any whitespace in the input.
    } while (!util::is_valid_input(input));  //Check validity of input.

    obj = std::move(bool_func(input));  //create a new object of bool_func and _move_ it to current obj

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

const std::vector<std::string>& bool_func::get_canonical_sop() {
    return sop;
}
const std::vector<std::string>& bool_func::get_canonical_pos() {
    return pos;
}
const std::vector<char>& bool_func::get_truth_table() {
    return truth_table;
}
const std::vector<int>& bool_func::get_minterms() {
    return minterms;
}
const std::vector<int>& bool_func::get_maxterms() {
    return maxterms;
}
int bool_func::get_var_count() const {
    return var_count;
}
void bool_func::print_truth_table() {
#define print_line                              \
    for (int VAR = 0; VAR < var_count; ++VAR) { \
        std::cout << "+---";                    \
    }                                           \
    std::cout << "+-----+\n";

    print_line for (int i = var_count - 1; i >= 0; --i) {
        std::cout << "| " << char('a' + i) << " ";
    }
    std::cout << "| exp |\n";
    print_line

        std::stack<char>
            vars;
    for (int i = 0; i < 1 << var_count; ++i) {

        for (int j = 0; j < var_count; ++j) {
            vars.push(i & (1 << j) ? '1' : '0');
        }
        for (int j = 0; j < var_count; ++j) {
            std::cout << "| " << vars.top() << " ";
            vars.pop();
        }
        std::cout << "|  " << is_minterm(i) << "  |\n";

        print_line
    }
}
bool bool_func::is_minterm(int i) {
    return truth_table[i];
}

void bool_func::set_canonical_sop() {

    sop.reserve(minterms.size());

    char product[11];
    int len;
    for (auto u : minterms) {
        len = 0;
        for (int i = 0; i < get_var_count(); ++i) {
            product[len++] = char(i + ((u & (1 << i)) ? 'a' : 'A'));
        }
        product[len] = 0;
        sop.emplace_back(product);
    }
}
void bool_func::set_canonical_pos() {
    char sum[22] = "(";
    int len;
    pos.reserve(maxterms.size());

    for (auto u : maxterms) {
        len = 1;

        for (int i = 0; i < get_var_count(); ++i) {
            sum[len++] = char(i + ((u & (1 << i)) ? 'A' : 'a'));
            sum[len++] = '+';
        }

        sum[len - 1] = ')';
        sum[len] = 0;

        pos.emplace_back(sum);
    }
}
void bool_func::set_var_count(const std::string& input) {
    var_count = *std::max_element(input.begin(), input.end()) - 'a' + 1;
}

void bool_func::set_truth_table() {
    for (int i = 0; i < func.size(); ++i) {
        set_truth_table('a', i, 0, 0);
    }
}
void bool_func::set_truth_table(char c, int product_index, int i, int value) {

    if (c == 'a' + var_count) {
        truth_table[value] = true;
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
void bool_func::init_truth_table() {
    truth_table.resize(1 << var_count);
}
void bool_func::parse_func(std::string& str) {
    std::string product;
    product.reserve(10);
    str += '+';
    for (const char* temp = &str.front(); *temp; ++temp) {
        switch (*temp) {
            case '\'':  //If char is negation, negate the last variable
                product.back() = util::flip_case(product.back());
                break;
            case '+':  //If char is +, we are done with the current term, insert it.
                if (product.empty())
                    break;
                util::ensure_str_unique(product);
                if (util::is_valid_func(product))
                    func.push_back(product);
                else
                    std::cout << "Product " << product << " omitted; expression always evaluates to false!\n";
                product.clear();
                break;
            default:  //Otherwise it's a character and part of our term
                product.push_back(*temp);
        }
    }

    util::ensure_vec_unique(func);
}
bool bool_func::is_combinable(implicant* x, implicant* y) {
    return x->dash_location == y->dash_location && util::is_power_of_two(x->imp^y->imp) == 1;
}
const std::vector<implicant>& bool_func::get_prime_implicants() {
    int i (1) ;
    std::cout<<"---------------------------------------\n";
    for(const auto& u : prime_implicants ) {
        std::cout<<"Prime Implicant " << i++ << '\n';
        std::cout<<binary_to_string(u.imp,u.dash_location,true)<<'\n';
        std::cout<<"Covered Minterms :\n";
        for(const auto& m : u.covered_minterms)
            std::cout<<m<<" ";
        std::cout<<"\n---------------------------------------\n";
    }
    return prime_implicants;
}
std::unordered_set<implicant,implicant_hash> bool_func::get_essentials(){
    int i(1);
    for(const auto& u : essential_prime_implicants ) {
        std::cout<<"Essential Prime Implicant " << i++ << '\n';
        std::cout<<binary_to_string(u.imp,u.dash_location,true)<<'\n';
        std::cout<<"Covered Minterms :\n";
        for(const auto& m : u.covered_minterms)
            std::cout<<m<<" ";
        std::cout<<"\n---------------------------------------\n";
    }
    std::cout<<"Uncovered Minterms :\n";
    for(const auto& u : uncovered_minterms)
        std::cout<<u<<" ";
    std::cout<<"\n---------------------------------------\n";
    return essential_prime_implicants;
}
const std::unordered_set<implicant,implicant_hash>& bool_func::get_optimized_func() {
    int len = int(ans.size()) - 1;
    std::cout<<"Final Optimized Function : \n";
    for(auto it = ans.begin(); it != ans.end(); ++it,--len)
        std::cout<<binary_to_string((*it).imp,(*it).dash_location,false) << (len? '+' : '\n' );
    if(!ans.empty()&&binary_to_string((ans.begin())->imp,(ans.begin()->dash_location),false)=="")
        std::cout<<"1\n";
    else if(ans.empty())
        std::cout<<"0\n";
    return ans;
}
void bool_func::set_optimized_func() {
    std::unordered_set<int> temp_uncovered_minterms = uncovered_minterms;

    for (const auto& u : essential_prime_implicants)
        for (const auto& m : u.covered_minterms) {
            temp_uncovered_minterms.erase(m);
        }

    implicant covers_max_uncovered (0,0,{});

    int cnt ,cur_max(0);

    ans = essential_prime_implicants;

    while(!temp_uncovered_minterms.empty()) {
        for (const auto& u : non_essential_prime_implicants) {
            cnt = 0 ;
            for (const auto& m : u.covered_minterms) {
                if (temp_uncovered_minterms.find(m) != temp_uncovered_minterms.end())
                    cnt++;
            }
            if (cnt >= cur_max) {
                cur_max = cnt;
                covers_max_uncovered = u;
            }
        }
        for(const auto& c : covers_max_uncovered.covered_minterms) {
            temp_uncovered_minterms.erase(c);
        }
        ans.emplace(covers_max_uncovered);
        cur_max = 0 ; cnt = 0 ;
    }
}
void bool_func::set_essentials_and_non_essentials() {
    for (int i(0);i<prime_implicants.size();++i) {
        for(int c : prime_implicants[i].covered_minterms) {
            coverage_chart[c].emplace(i);
            uncovered_minterms.emplace(c); // This is going to aid in get_optimized_func() instead of repeating code.
        }
    }
    for (const auto& u : coverage_chart){
        if(u.second.size() == 1)
            essential_prime_implicants.emplace(prime_implicants[*u.second.begin()]) ;
    }
    for(const auto& u : essential_prime_implicants)
        for(const auto& m : u.covered_minterms){
            uncovered_minterms.erase(m);
    }
    for(const auto& u : prime_implicants)
        if(essential_prime_implicants.find(u)==essential_prime_implicants.end())
            non_essential_prime_implicants.emplace(u);
}
void bool_func::set_prime_implicants() {
    auto start = std::chrono::high_resolution_clock::now();
    pi_table.resize(var_count + 1);
    tmp_table.resize(var_count + 1);

    for (int i: minterms) {
        pi_table[__builtin_popcount(i)].emplace_back(i, 0, false);
        pi_table[__builtin_popcount(i)].back().covered_minterms.push_back(i);
    }
    exists.reserve(10000);
    do {
        is_combined = false;
        for (int i(0); i < pi_table.size() ; ++i)
            for (implicant& j : pi_table[i]) {
                if (i < var_count && !pi_table[i+1].empty() ) {
                    for (implicant& k : pi_table[i + 1]){
                        if (is_combinable(&j, &k)) {
                            if(exists[{(j.imp ^ k.imp) | j.imp, (j.imp ^ k.imp) | j.dash_location}]) {
                                j.is_combined = true; k.is_combined = true; is_combined = true;
                            }
                            else {
                                tmp_table[i].emplace_back((j.imp ^ k.imp) | j.imp, (j.imp ^ k.imp) | j.dash_location,
                                                          false);
                                exists[{(j.imp ^ k.imp) | j.imp, (j.imp ^ k.imp) | j.dash_location}] = true;
                                for (auto m : j.covered_minterms)
                                    tmp_table[i].back().covered_minterms.push_back(m);
                                for (auto m : k.covered_minterms)
                                    tmp_table[i].back().covered_minterms.push_back(m);
                                j.is_combined = true; k.is_combined = true; is_combined = true;
                            }
                        }
                    }
                }
                if (!j.is_combined)
                    prime_implicants.push_back(j);
            }
        exists.clear();
        pi_table = std::move(tmp_table);
        tmp_table.resize(var_count);
    } while(is_combined);

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "Time taken by function: " << duration.count() << " microseconds" << "\n";
}
std::string bool_func::binary_to_string(int imp, int dash,bool b) const {
    char c = 'a' ; // add (+ var_count - 1) if u wanna invert the ouput again.
    std::string output;
    for(int i(0);i<var_count;++i,++c) { // add (--c) if u wanna invert the output again.
        if((dash&1))
            output = (!b ? "" : "-") + output;
        else if((imp&1))
            output = c + output;
        else if(!(imp&1))
            output = (char)toupper(c) + output;
        dash = dash>>1;
        imp = imp>>1;
    }
    return output;
}
