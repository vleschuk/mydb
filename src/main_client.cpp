#include <array>
#include <cstdlib>
#include <iostream>
#include <boost/asio.hpp>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>

namespace io = boost::asio;
namespace ip = io::ip;
using tcp = io::ip::tcp;
using error_code = boost::system::error_code;

bool validateCmd(const std::string &cmd) {
  if(cmd != "INSERT" && cmd != "UPDATE"
     && cmd != "DELETE" && cmd != "GET") {
    return false;
  }
  return true;
}

int main(int argc, char **argv)
{
  if(argc < 5) {
    std::cerr << "Usage: dbclient <address> <port> <command> <key> [value]\n";
    return 1;
  }

  std::string addr(argv[1]);
  auto port = std::atoi(argv[2]);
  std::string cmd(argv[3]);
  if(!validateCmd(cmd)) {
    std::cerr << "Invalid command: " << cmd
      << ", allowed commands: INSERT|UPDATE|DELETE|GET" << std::endl;
    return 1;
  }
  std::string key(argv[4]);
  std::string val;
  if(cmd == "INSERT" || cmd == "UPDATE") {
    if(argc == 5) {
      std::cerr << "For " << cmd << " command you must provide value" << std::endl;
      return 1;
    }
    val = argv[5];
  }

  io::io_context io_context;
  tcp::socket socket(io_context);
  ip::address address = ip::make_address(addr);
  tcp::endpoint endpoint(address, port);

  error_code error;
  try {
    socket.connect(endpoint, error);
    struct timeval tv;
    tv.tv_sec  = 5;
    tv.tv_usec = 0;
    setsockopt(socket.native_handle(), SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
    setsockopt(socket.native_handle(), SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv));
    if(error) {
      throw boost::system::system_error(error);
    }

    std::string msg(cmd);
    msg.append("\n");
    socket.write_some(io::buffer(msg), error);
    if(error) {
      throw boost::system::system_error(error);
    }

    msg = std::to_string(key.size());
    msg.append("\n");
    socket.write_some(io::buffer(msg), error);
    if(error) {
      throw boost::system::system_error(error);
    }

    msg = key;
    msg.append("\n");
    socket.write_some(io::buffer(msg), error);
    if(error) {
      throw boost::system::system_error(error);
    }

    if(cmd == "INSERT" || cmd == "UPDATE") {
      msg = std::to_string(val.size());
      msg.append("\n");
      socket.write_some(io::buffer(msg), error);
      if(error) {
        throw boost::system::system_error(error);
      }

      msg = val;
      msg.append("\n");
      socket.write_some(io::buffer(msg), error);
      if(error) {
        throw boost::system::system_error(error);
      }
    }

    for (;;) {
      std::array<char, 128> buf;
      boost::system::error_code error;

      size_t len = socket.read_some(io::buffer(buf), error);

      if (error == io::error::eof) {
        break;
      }
      else if(error) {
        throw boost::system::system_error(error);
      }

      std::cout.write(buf.data(), len);
    }
    std::cout << std::endl;
  }
  catch(const std::exception &e) {
    std::cerr << "Exception: " << e.what() << std::endl;
    return 1;
  }
}
