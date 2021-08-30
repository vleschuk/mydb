#ifndef MY_REQUEST_HANDLER_H_
#define MY_REQUEST_HANDLER_H_

#include <string>
#include "stat.h"
#include "storage.h"

namespace my {

struct Reply;
struct Request;

class RequestHandler
{
public:

  explicit RequestHandler(Storage &storage, Stat &stat);
  RequestHandler(const RequestHandler &) = delete;
  RequestHandler(RequestHandler &&) = delete;
  RequestHandler& operator=(const RequestHandler &) = delete;
  RequestHandler& operator=(RequestHandler &&) = delete;

  void handleRequest(const Request& req, Reply& rep);

private:
  static bool urlDecode(const std::string &in, std::string &out);

private:
  Storage &storage_;
  Stat &stat_;
};

} // namespace my

#endif // MY_REQUEST_HANDLER_H_
