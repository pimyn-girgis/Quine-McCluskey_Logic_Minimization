//
// Created by Bemen on 13/03/2023.
//

#ifndef QM_ALGORITHM_BOOL_FUNC_H
#define QM_ALGORITHM_BOOL_FUNC_H

#include <iostream>
#include <vector>
#include <string>
#include <cstring>

class bool_func {
private:
  char flip_case(const char c);
  static bool is_valid_func(const std::string &func);
  std::vector<std::string> sum_of_products;

public:
  friend std::istream &operator>>(std::istream &is, bool_func &obj);
  friend std::ostream &operator<<(std::ostream &os,
                                  const bool_func &obj);
};

#endif// QM_ALGORITHM_BOOL_FUNC_H
