#ifndef MY_REQUEST_PARSER_H_
#define MY_REQUEST_PARSER_H_

#include <tuple>
#include "request.h"

namespace my {

struct Request;

class RequestParser
{
public:

  RequestParser();

  void reset();

  enum result_type {good, bad, indeterminate};

  /// Parse some data. The enum return value is good when a complete request has
  /// been parsed, bad if the data is invalid, indeterminate when more data is
  /// required. The InputIterator return value indicates how much of the input
  /// has been consumed.
  template <typename InputIterator>
  result_type parseKey(Request &req, InputIterator begin, InputIterator end) {
    auto bytes = end - begin;
    if(bytes < req.keySize) {
      req.err = Error::E_NOT_ENOUGH;
      return indeterminate;
    }
    req.key.assign(begin, begin + req.keySize);
    if(req.cmdCode == Cmd::Delete || req.cmdCode == Cmd::Get) {
      return good;
    }
    state_ = ValueSize;
    return indeterminate;
  }

  template <typename InputIterator>
  result_type parseValue(Request &req, InputIterator begin, InputIterator end) {
    auto bytes = end - begin;
    if(bytes < req.valueSize) {
      req.err = Error::E_NOT_ENOUGH;
      return indeterminate;
    }
    req.value.assign(begin, begin + req.valueSize);
    return good;
  }

  template <typename InputIterator>
  std::tuple<result_type, InputIterator> parse(Request &req,
      InputIterator begin, InputIterator end)
  {
    while (begin != end)
    {
      if(state_ == Key) {
        auto res = parseKey(req, begin, end);
        if(res == indeterminate && req.err == Error::E_NOT_ENOUGH) {
          return std::make_tuple(indeterminate, begin);
        }
        if(res == indeterminate) {
          begin += req.keySize + 1;
          if(begin != end && *begin == '\n') { // could be in case of CRLF
            ++begin;
          }
          continue;
        }
        return std::make_tuple(res, begin);
      }
      if(state_ == Value) {
        auto res = parseValue(req, begin, end);
        return std::make_tuple(res, begin);
      }
      auto res = consume(req, *begin++);
      if(res == good || res == bad) {
        return std::make_tuple(res, begin);
      }
    }
    return std::make_tuple(indeterminate, begin);
  }

private:
  result_type consume(Request& req, char input);
  static bool is_char(int c);
  static bool is_ctl(int c);
  static bool is_tspecial(int c);
  static bool is_digit(int c);
  static void validateCmd(Request& req);

private:
  enum state
  {
    CmdStart,
    Cmd,
    KeySize,
    Key,
    ValueSize,
    Value,
  } state_;
};

} // namespace my

#endif // MY_REQUEST_PARSER_H_
