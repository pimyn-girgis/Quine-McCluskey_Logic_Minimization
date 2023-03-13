#ifndef QM_ALGORITHM_BOOL_FUNC_H
#define QM_ALGORITHM_BOOL_FUNC_H

#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <algorithm>

class bool_func {
private:
  char flip_case(const char c);
  static bool is_valid_func(const std::string &func);
  static std::string remove_spaces(const std::string &str);
  std::vector<std::string> sum_of_products;

public:
  friend std::istream &operator>>(std::istream &is, bool_func &obj);
  friend std::ostream &operator<<(std::ostream &os,
                                  const bool_func &obj);
};

#endif// QM_ALGORITHM_BOOL_FUNC_H
