#include <cstdlib>
#include "dblimits.h"
#include "request_parser.h"

namespace my {

RequestParser::RequestParser()
  : state_(CmdStart)
{
}

void RequestParser::reset()
{
  state_ = CmdStart;
}

RequestParser::result_type RequestParser::consume(Request& req, char input)
{
  switch (state_)
  {
  case CmdStart:
    if (!is_char(input) || is_ctl(input) || is_tspecial(input))
    {
      req.err = Error::E_INVALID_SYNTAX;
      return bad;
    }
    else
    {
      state_ = Cmd;
      req.cmd.push_back(input);
      return indeterminate;
    }
  case Cmd:
    if (input == '\n')
    {
      validateCmd(req);
      if(req.cmdCode == Cmd::Invalid) {
        req.err = Error::E_INVALID_SYNTAX;
        return bad;
      }
      state_ = KeySize;
      return indeterminate;
    }
    else if(input == '\r') {
      return indeterminate;
    }
    else if (!is_char(input) || is_ctl(input) || is_tspecial(input))
    {
      req.err = Error::E_INVALID_SYNTAX;
      return bad;
    }
    else
    {
      req.cmd.push_back(input);
      return indeterminate;
    }
  case KeySize:
  case ValueSize:
    if(input == '\n') {
      size_t sz = std::atoi(req.sizeStr.c_str());
      size_t limit = (state_ == KeySize) ?
        static_cast<size_t>(Limits::MaxKeySize) :
        static_cast<size_t>(Limits::MaxValueSize);

      if(sz > limit) {
        req.sizeStr.clear();
        req.err = Error::E_TOO_LARGE;
        return bad;
      }

      if(state_ == KeySize) {
        req.keySize = sz;
      }
      else {
        req.valueSize = sz;
      }

      state_ = (state_ == KeySize) ? Key : Value;
      req.sizeStr.clear();
      return indeterminate;
    }
    else if(input == '\r') {
      return indeterminate;
    }
    else if(!is_digit(input)) {
      req.err = Error::E_INVALID_SYNTAX;
      return bad;
    }
    else {
      req.sizeStr.push_back(input);
      return indeterminate;
    }
  case Key:
  case Value:
    return indeterminate;
  default:
    req.err = Error::E_FAIL;
    return bad;
  }
}

bool RequestParser::is_char(int c)
{
  return c >= 0 && c <= 127;
}

bool RequestParser::is_ctl(int c)
{
  return (c >= 0 && c <= 31) || (c == 127);
}

bool RequestParser::is_tspecial(int c)
{
  switch (c)
  {
  case '(': case ')': case '<': case '>': case '@':
  case ',': case ';': case ':': case '\\': case '"':
  case '/': case '[': case ']': case '?': case '=':
  case '{': case '}': case ' ': case '\t':
    return true;
  default:
    return false;
  }
}

bool RequestParser::is_digit(int c)
{
  return c >= '0' && c <= '9';
}

void RequestParser::validateCmd(Request &req) {
  if(req.cmd == "INSERT") {
    req.cmdCode = Cmd::Insert;
  }
  else if(req.cmd == "UPDATE") {
    req.cmdCode = Cmd::Update;
  }
  else if(req.cmd == "DELETE") {
    req.cmdCode = Cmd::Delete;
  }
  else if(req.cmd == "GET") {
    req.cmdCode = Cmd::Get;
  }
  else {
    req.cmdCode = Cmd::Invalid;
  }
  req.cmd.clear();
  return;
}

} // namespace my
