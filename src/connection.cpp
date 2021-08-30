#include "connection.h"
#include <utility>
#include <vector>
#include <iostream>
#include "connection_manager.h"
#include "request_handler.h"

namespace my {

Connection::Connection(tcp::socket socket, ConnectionManager &manager,
    RequestHandler &handler, io::io_context &ioContext)
  : socket_(std::move(socket)),
    read_(ioContext),
    write_(ioContext),
    connectionManager_(manager),
    requestHandler_(handler)
{
}

void Connection::start()
{
  doRead();
}

void Connection::stop()
{
  socket_.close();
}

void Connection::doRead() {
  auto self(shared_from_this());
  socket_.async_read_some(io::buffer(buffer_), io::bind_executor(read_,
      [this, self](boost::system::error_code ec, std::size_t bytesTransferred) {
        if(!ec) {
          RequestParser::result_type result;
          std::tie(result, std::ignore) = requestParser_.parse(
              request_, buffer_.data(), buffer_.data() + bytesTransferred);

          if(result == RequestParser::good) {
            requestHandler_.handleRequest(request_, reply_);
            doWrite();
          }
          else if(result == RequestParser::bad) {
            reply_ = Reply::stockReply(request_.err);
            doWrite();
          }
          else {
            doRead();
          }
        }
        else if(ec != io::error::operation_aborted) {
          connectionManager_.stop(shared_from_this());
        }
      }));
}

void Connection::doWrite()
{
  auto self(shared_from_this());
  io::async_write(socket_, reply_.toBuffers(), io::bind_executor(write_,
      [this, self](boost::system::error_code ec, std::size_t)
      {
        if(!ec)
        {
          // Initiate graceful connection closure.
          boost::system::error_code ignored;
          socket_.shutdown(io::ip::tcp::socket::shutdown_both, ignored);
        }

        if(ec != io::error::operation_aborted)
        {
          connectionManager_.stop(shared_from_this());
        }
      }));
}

} // namespace my
