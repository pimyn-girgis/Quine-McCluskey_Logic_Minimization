#ifndef QM_ALGORITHM_BOOL_FUNC_H
#define QM_ALGORITHM_BOOL_FUNC_H

#include "util.h"

class bool_func {
   private:
    int var_count;

    std::vector<std::string> sop;
    std::vector<std::string> pos;
    std::vector<std::string> func;
    std::vector<int> minterms;
    std::vector<int> maxterms;
    std::vector<char> truth_table;
    std::string func_as_str;

    void set_var_count(const std::string& input);
    void set_truth_table(char c = 'a', int product_index = 0, int i = 0, int value = 0);
    void set_min_max_terms();
    void set_canonical_sop();
    void set_canonical_pos();

   public:
    bool_func();
    bool_func(std::string &str);

    friend std::istream& operator>>(std::istream& is, bool_func& obj);
    friend std::ostream& operator<<(std::ostream& os, const bool_func& obj);

    std::vector<std::string> get_canonical_sop();
    std::vector<std::string> get_canonical_pos();
    std::vector<int> get_minterms();
    std::vector<int> get_maxterms();
    std::vector<char> get_truth_table();

    const bool is_minterm(int i);
    const int get_var_count() const;

    void print_truth_table();
};

#endif  // QM_ALGORITHM_BOOL_FUNC_H
