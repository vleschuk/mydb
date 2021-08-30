#include <unordered_map>
#include "error.h"

namespace my {

const std::string &strError(Error e) {
  static const std::unordered_map<Error, std::string> map{{
    {Error::S_OK, "S_OK"},
    {Error::E_INVALID_SYNTAX, "E_INVALID_SYNTAX"},
    {Error::E_TOO_LARGE, "E_TOO_LARGE"},
    {Error::E_NOT_ENOUGH, "E_NOT_ENOUGH"},
    {Error::E_NOT_FOUND, "E_NOT_FOUND"},
    {Error::E_NOT_CHANGED, "E_NOT_CHANGED"},
    {Error::E_EXISTS, "E_EXISTS"},
    {Error::E_FAIL, "E_FAIL"},
  }};
  static const std::string unknown("UNKNOWN");
  auto it = map.find(e);
  if(it == map.end()) {
    return unknown;
  }
  return it->second;
}

} // namespace my
