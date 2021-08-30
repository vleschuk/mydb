#include "server.h"
#include <signal.h>
#include <thread>
#include <utility>

namespace my {

const unsigned DBServer::timerInterval_ = 60;

DBServer::DBServer(const std::string &address, const std::string &port,
    const std::string &dbfile)
  : ioContext_(1),
    signals_(ioContext_),
    timer_(ioContext_, std::chrono::seconds(timerInterval_)),
    acceptor_(ioContext_),
    connectionManager_(),
    storage_(dbfile),
    requestHandler_(storage_, stat_) {
  signals_.add(SIGINT);
  signals_.add(SIGTERM);
#if defined(SIGQUIT)
  signals_.add(SIGQUIT);
#endif // defined(SIGQUIT)

  doAwaitStop();

  // Open the acceptor with the option to reuse the address (i.e. SO_REUSEADDR).
  tcp::resolver resolver(ioContext_);
  tcp::endpoint endpoint = *resolver.resolve(address, port).begin();
  acceptor_.open(endpoint.protocol());
  acceptor_.set_option(tcp::acceptor::reuse_address(true));
  acceptor_.bind(endpoint);
  acceptor_.listen();

  doAccept();
}

void DBServer::run() {
  std::vector<std::thread> threads;
  auto count = std::thread::hardware_concurrency() * 2;

  for(size_t i = 0; i < count; ++i) {
    threads.emplace_back([&] {
        ioContext_.run();
    });
  }

  for(auto &t : threads) {
    t.join();
  }
}

void DBServer::doAccept() {
  acceptor_.async_accept(
      [this](boost::system::error_code ec, tcp::socket socket) {
        // Check whether the server was stopped by a signal before this
        // completion handler had a chance to run.
        if (!acceptor_.is_open())
        {
          return;
        }

        if (!ec)
        {
          connectionManager_.start(std::make_shared<Connection>(
              std::move(socket), connectionManager_, requestHandler_, ioContext_));
        }

        doAccept();
      });
}

void DBServer::doAwaitStop() {
  signals_.async_wait(
      [this](boost::system::error_code /*ec*/, int /*signo*/) {
        acceptor_.close();
        connectionManager_.stopAll();
        timer_.cancel();
      });
  timer_.async_wait([this](boost::system::error_code ec) {
      timerFunc(ec);
      });
}

void DBServer::timerFunc(boost::system::error_code ec) {
  if(ec == io::error::operation_aborted) {
    std::cerr << "The timer is cancelled" << std::endl;
    return;
  }
  std::cerr << "Stats:" << std::endl;
  std::cerr << "size: " << storage_.size() << std::endl;
  std::cerr << "operations successful/failed" << std::endl;
  std::cerr << "insert: " << stat_.insertOps.first << "/"
    << stat_.insertOps.second << std::endl;
  std::cerr << "update: " << stat_.updateOps.first << "/"
    << stat_.updateOps.second << std::endl;
  std::cerr << "delete: " << stat_.deleteOps.first << "/"
    << stat_.deleteOps.second << std::endl;
  std::cerr << "get: " << stat_.getOps.first << "/"
    << stat_.getOps.second << std::endl;
  timer_.expires_at(timer_.expiry() + std::chrono::seconds(timerInterval_));
  timer_.async_wait([this](boost::system::error_code ec) {
      timerFunc(ec);
      });
}

} // namespace my
