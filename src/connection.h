#ifndef MY_CONNECTION_H_
#define MY_CONNECTION_H_

#include <array>
#include <memory>
#define BOOST_ASIO_NO_DEPRECATED
#include <boost/asio.hpp>
#include "reply.h"
#include "request.h"
#include "request_handler.h"
#include "request_parser.h"

namespace io = boost::asio;
using tcp = io::ip::tcp;
using error_code = boost::system::error_code;

namespace my {

class ConnectionManager;

class Connection
  : public std::enable_shared_from_this<Connection>
{
public:

  explicit Connection(tcp::socket socket, ConnectionManager &manager,
      RequestHandler &handler, io::io_context &ioContext);
  Connection(const Connection &) = delete;
  Connection(Connection &&) = delete;
  Connection &operator=(const Connection &) = delete;
  Connection &operator=(Connection &&) = delete;

  void start();
  void stop();

private:
  void doRead();
  void doWrite();

private:
  tcp::socket socket_;
  io::io_context::strand read_;
  io::io_context::strand write_;
  ConnectionManager &connectionManager_;
  RequestHandler &requestHandler_;
  std::array<char, 1024*1024> buffer_;
  Request request_;
  RequestParser requestParser_;
  Reply reply_;
};

typedef std::shared_ptr<Connection> ConnectionPtr;

} // namespace my

#endif // MY_CONNECTION_H_
