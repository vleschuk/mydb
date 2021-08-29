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
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/member.hpp>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include "record.h"
#include "error.h"

using boost::multi_index_container;
using namespace boost::multi_index;

namespace my {

class Storage {
private:

  struct Key {};

  typedef multi_index_container<
    Record,
    indexed_by<
      boost::multi_index::hashed_unique<
      boost::multi_index::tag<Key>,
      BOOST_MULTI_INDEX_MEMBER(Record, SharedString, key)
    >>,
    bip::allocator<Record, bip::managed_mapped_file::segment_manager>
  > RecordContainer;

public:
  explicit Storage(const std::string &DBFile);
  Storage(const Storage &) = delete;
  Storage(Storage &&) = delete;
  Storage &operator=(const Storage &) = delete;
  Storage &operator=(Storage &&) = delete;
  ~Storage() = default;

  Error addRecord(const std::string &key, const std::string &value);
  Error getValue(const std::string &key, std::string &value);
  Error updateRecord(const std::string &key, const std::string &value);
  Error removeRecord(const std::string &key);

private:
  static const char *MtxUUID_;
  static const size_t MaxFileSize_;

private:
  bip::managed_mapped_file DBFile_;
  bip::named_mutex RecMtx_;
  RecordContainer *Records_ = nullptr;
};

} // namespace my
#endif // MY_STORAGE_H
