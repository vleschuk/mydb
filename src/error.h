#ifndef MY_ERROR_H_
#define MY_ERROR_H_

#include <string>

namespace my {

enum class Error {
  S_OK,
  E_NOT_FOUND,
  E_NOT_CHANGED,
  E_EXISTS,
  E_FAIL,
};

const std::string &strError(Error);

} // namespace my
#endif // MY_ERROR_H
