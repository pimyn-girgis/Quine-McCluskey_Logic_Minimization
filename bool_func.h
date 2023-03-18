#ifndef QM_ALGORITHM_BOOL_FUNC_H
#define QM_ALGORITHM_BOOL_FUNC_H

#include <chrono>
#include <unordered_set>
#include "util.h"

struct implicant {
    implicant(int i, int d, bool b);
    int imp;
    int dash_location;
    bool is_combined;
    std::vector<int> covered_minterms;
    bool operator==(const implicant& other) const {
        return (imp == other.imp) && (dash_location == other.dash_location);
    }
};

struct pair_hash
{
    template <class T1, class T2>
    std::size_t operator() (const std::pair<T1, T2> &pair) const {
       return (static_cast<std::uint32_t>(pair.first) << 11) + pair.second << 4 ;
    }
};

class bool_func {
   private:
    explicit bool_func(std::string& str); //constructor from string
    int var_count;
    bool is_combined;

    std::vector<std::string> sop; //Vector Of products representing the Canonical SoP
    std::vector<std::string> pos; //Vector of sums representing the Canonical PoS
    std::vector<std::string> func; //Vector of products representing the initial SoP
    std::vector<int> minterms;
    std::vector<int> maxterms;
    std::vector<char> truth_table; //Vector<char> was used because it is faster than using a vector<bool>
    std::vector<std::vector<implicant>> pi_table;
    std::vector<std::vector<implicant>> tmp_table;
    std::vector<implicant> prime_implicants;
    std::vector<implicant> essential_prime_implicants;
    std::vector<implicant> non_essential_prime_implicants;
    std::unordered_map<int,std::unordered_set<int>> coverage_chart;
    std::unordered_map<std::pair<int,int>,int,pair_hash> exists;

    void parse_func(std::string &str);
    void set_var_count(const std::string& input); //Given a function as input, calculate the number of variables
    void init_truth_table();
    void set_truth_table(char c = 'a', int product_index = 0, int i = 0, int value = 0); //Calculate the truth table
    void set_min_max_terms();
    void set_canonical_sop();
    void set_canonical_pos();

   public:
    bool_func(); //default constructor initializes var_count = 0

    friend std::istream& operator>>(std::istream& is, bool_func& obj);
    friend std::ostream& operator<<(std::ostream& os, const bool_func& obj);

    const std::vector<std::string>& get_canonical_sop();
    const std::vector<std::string>& get_canonical_pos();
    const std::vector<int>& get_minterms();
    const std::vector<int>& get_maxterms();
    const std::vector<char>& get_truth_table();
    const std::vector<implicant>& get_prime_implicants();
    const std::vector<implicant>& get_optimized_func();
    bool is_minterm(int i);
    static bool is_combinable(implicant * x, implicant * y);

    int get_var_count() const;

    void print_truth_table();

    bool test (implicant * j , implicant * k,int i);
};

#endif  // QM_ALGORITHM_BOOL_FUNC_H
