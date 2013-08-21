#include "utils.h"

#include <algorithm>

std::string db_utils::getRandomString(int len) {
  auto randchar = []() -> char
  {
      const char charset[] =
      "0123456789"
      "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
      "abcdefghijklmnopqrstuvwxyz";
      const size_t max_index = (sizeof(charset) - 1);
      return charset[ rand() % max_index ];
  };
  std::string str(len, 0);
  std::generate_n( str.begin(), len, randchar );
  return str;
}
