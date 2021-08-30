#ifndef MY_SERVER_H_
#define MY_SERVER_H_

#define BOOST_ASIO_NO_DEPRECATED
#include <boost/asio.hpp>
#include <map> // pair
#include <string>
#include "connection.h"
#include "connection_manager.h"
#include "request_handler.h"
#include "stat.h"
#include "storage.h"

namespace io = boost::asio;
using tcp = io::ip::tcp;
using error_code = boost::system::error_code;

namespace my {

class DBServer {
public:

  explicit DBServer(const std::string &address, const std::string &port,
      const std::string &dbfile);
  DBServer(const DBServer &) = delete;
  DBServer(DBServer &&) = delete;
  DBServer& operator=(const DBServer &) = delete;
  DBServer& operator=(DBServer &&) = delete;

  void run();

private:
  void doAccept();
  void doAwaitStop();
  void timerFunc(boost::system::error_code ec);

private:
  static const unsigned timerInterval_;

private:
  io::io_context ioContext_;
  io::signal_set signals_;
  io::steady_timer timer_;
  tcp::acceptor acceptor_;

  ConnectionManager connectionManager_;
  RequestHandler requestHandler_;
  Storage storage_;
  Stat stat_;
};

} // namespace my

#endif // MY_SERVER_H_
