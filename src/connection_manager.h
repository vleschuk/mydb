#ifndef MY_CONNECTION_MANAGER_H_
#define MY_CONNECTION_MANAGER_H_

#include <set>
#include "connection.h"

namespace my {

class ConnectionManager
{
public:

  ConnectionManager();
  ConnectionManager(const ConnectionManager &) = delete;
  ConnectionManager &operator=(const ConnectionManager &) = delete;

  void start(ConnectionPtr c);
  void stop(ConnectionPtr c);
  void stopAll();

private:
  std::set<ConnectionPtr> connections_;
};

} // namespace my

#endif // MY_CONNECTION_MANAGER_H_
