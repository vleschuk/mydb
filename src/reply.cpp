#include "reply.h"
#include <string>

namespace my {

std::vector<io::const_buffer> Reply::toBuffers()
{
  std::vector<io::const_buffer> buffers;
  buffers.push_back(io::buffer(strError(status)));
  buffers.push_back(io::buffer("\n"));
  buffers.push_back(io::buffer(content));
  return buffers;
}

Reply Reply::stockReply(Error status)
{
  Reply rep;
  rep.status = status;
  return rep;
}

} // namespace my
