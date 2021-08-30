#ifndef MY_REQUEST_H_
#define MY_REQUEST_H_

#include <string>
#include <vector>
#include "error.h"
#include "cmd.h"

namespace my {

struct Request
{
  Error err;

  std::string cmd;
  Cmd cmdCode;

  std::string sizeStr;
  size_t keySize;
  size_t valueSize;

  std::string key;
  std::string value;
};

} // namespace my

#endif // MY_REQUEST_H_
