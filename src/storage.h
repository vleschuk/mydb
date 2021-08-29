#ifndef MY_STORAGE_H_
#define MY_STORAGE_H_

#if !defined(NDEBUG)
#define BOOST_MULTI_INDEX_ENABLE_INVARIANT_CHECKING
#define BOOST_MULTI_INDEX_ENABLE_SAFE_MODE
#endif

#include <boost/config.hpp> /* keep it first to prevent nasty warns in MSVC */
#include <algorithm>
#include <boost/interprocess/sync/named_mutex.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/member.hpp>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include "record.h"

using boost::multi_index_container;
using namespace boost::multi_index;

namespace my {

class Storage {
private:
/* Define a multi_index_container of records with index on key.
 * This container can be placed in shared memory because:
 *   * Record can be placed in shared memory.
 *   * We are using a Boost.Interprocess specific allocator.
 */

/* see Compiler specifics: Use of member_offset for info on
 * BOOST_MULTI_INDEX_MEMBER
 */

  typedef multi_index_container<
    Record,
    indexed_by<
      ordered_hashed_unique<
      BOOST_MULTI_INDEX_MEMBER(Record, SharedString, key)
    >>,
    bip::allocator<book, bip::managed_mapped_file::segment_manager>
  > RecordContainer;

public:
  explicit Storage(const std::string &DBFile);
  Storage(const Storage &) = delete;
  Storage(Storage &&) = delete;
  Storage &operator=(const Storage &) = delete;
  Storage &operator=(Storage &&) = delete;
  ~Storage() = default;

private:
  static const char *MtxUUID_;

private:
  bip::managed_mapped_file DBFile_;
  bip::named_mutex RecMtx_;
  RecordContaiter *Records_ = nullptr;
};

} // namespace my
#endif // MY_STORAGE_H


