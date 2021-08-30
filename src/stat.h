#ifndef MY_STAT_H_
#define MY_STAT_H_
#include <atomic>
#include <map>

namespace my {

typedef std::pair<std::atomic<uint32_t>, std::atomic<uint32_t>> OpCount;
struct Stat {
  OpCount insertOps;
  OpCount updateOps;
  OpCount deleteOps;
  OpCount getOps;
};

} // namespace my
#endif // MY_STAT_H_
