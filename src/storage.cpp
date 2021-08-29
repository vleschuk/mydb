#include "storage.h"

namespace my {

const char *Storage::MtxUUID_ = "ad8439d1-7699-4a56-95fe-f0eee321b741";
const size_t Storage::MaxFileSize_ = 65536;

Storage::Storage(const std::string &DBFile)
  : DBFile_(bip::open_or_create, DBFile.c_str(), MaxFileSize_)
  , RecMtx_(bip::open_or_create, MtxUUID_)
  , Records_(DBFile_.find_or_construct<RecordContainer>("record container")(
        RecordContainer::ctor_args_list(),
        RecordContainer::allocator_type(DBFile_.get_segment_manager())))
  {}


Error Storage::addRecord(const std::string &key, const std::string &value) {
  Record rec(SharedString::allocator_type(DBFile_.get_segment_manager()));
  rec.key = key.c_str();
  rec.value = value.c_str();
  auto &idx = Records_->get<Key>();
  bip::scoped_lock<bip::named_mutex> guard(RecMtx_);
  auto it = idx.find(rec.key);
  if(it != idx.end()) {
    return Error::E_EXISTS;
  }
  Records_->insert(rec);
  return Error::S_OK;
}

Error Storage::getValue(const std::string &key, std::string &value) {
  SharedString sharedKey(
      SharedString::allocator_type(DBFile_.get_segment_manager()));
  sharedKey = key.c_str();
  auto &idx = Records_->get<Key>();
  bip::scoped_lock<bip::named_mutex> guard(RecMtx_);
  auto it = idx.find(sharedKey);
  if(it == idx.end()) {
    return Error::E_NOT_FOUND;
  }
  value.assign(it->value.begin(), it->value.end());
  return Error::S_OK;
}

Error Storage::updateRecord(const std::string &key, const std::string &value) {
  SharedString sharedKey(
      SharedString::allocator_type(DBFile_.get_segment_manager()));
  sharedKey = key.c_str();
  auto &idx = Records_->get<Key>();
  bip::scoped_lock<bip::named_mutex> guard(RecMtx_);
  auto it = idx.find(sharedKey);
  if(it == idx.end()) {
    return Error::E_NOT_FOUND;
  }
  std::string tmp(it->value.begin(), it->value.end());
  if(tmp == value) {
    return Error::E_NOT_CHANGED;
  }
  idx.erase(it);
  Record rec(SharedString::allocator_type(DBFile_.get_segment_manager()));
  rec.key = key.c_str();
  rec.value = value.c_str();
  Records_->insert(rec);
  return Error::S_OK;
}

Error Storage::removeRecord(const std::string &key) {
  SharedString sharedKey(
      SharedString::allocator_type(DBFile_.get_segment_manager()));
  sharedKey = key.c_str();
  auto &idx = Records_->get<Key>();
  bip::scoped_lock<bip::named_mutex> guard(RecMtx_);
  auto it = idx.find(sharedKey);
  if(it == idx.end()) {
    return Error::E_NOT_FOUND;
  }
  idx.erase(it);
  return Error::S_OK;
}

} // namespace my
