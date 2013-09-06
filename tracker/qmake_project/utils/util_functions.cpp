#include "util_functions.h"

#include <algorithm>
#include <time.h>
#include <iostream>
#include <boost/array.hpp>

namespace sp2p{
namespace tracker{
namespace utils{

std::string getRandomString(int len) {
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

long long currTimeInMS()
{
    return time_t(0)*1000;
}

} // namespace utils
} // namespace tracker
} // namespace sp2p
