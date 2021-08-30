#ifndef MY_REPLY_H_
#define MY_REPLY_H_

#include <string>
#include <vector>
#define BOOST_ASIO_NO_DEPRECATED
#include <boost/asio.hpp>
#include "error.h"

namespace io = boost::asio;

namespace my {

struct Reply
{
  Error status;

  std::string content;

  /// Convert the reply into a vector of buffers. The buffers do not own the
  /// underlying memory blocks, therefore the reply object must remain valid and
  /// not be changed until the write operation has completed.
  std::vector<io::const_buffer> toBuffers();

  static Reply stockReply(Error status);
};

} // namespace my

#endif // MY_REPLY_H_
