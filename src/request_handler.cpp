#include "request_handler.h"
#include <fstream>
#include <sstream>
#include <string>
#include "reply.h"
#include "request.h"

namespace my {

RequestHandler::RequestHandler(Storage &storage, Stat &stat)
  : storage_(storage), stat_(stat) {
}

void RequestHandler::handleRequest(const Request &req, Reply &rep) { 
  Error res;
  switch(req.cmdCode) {
  case Cmd::Insert:
    res = storage_.addRecord(req.key, req.value);
    if(res == Error::S_OK) {
      stat_.insertOps.first++;
    }
    else {
      stat_.insertOps.second++;
    }
    rep.status = res;
    return;
  case Cmd::Update:
    res = storage_.updateRecord(req.key, req.value);
    if(res == Error::S_OK) {
      stat_.updateOps.first++;
    }
    else {
      stat_.updateOps.second++;
    }
    rep.status = res;
    return;
  case Cmd::Delete:
    res = storage_.removeRecord(req.key);
    if(res == Error::S_OK) {
      stat_.deleteOps.first++;
    }
    else {
      stat_.deleteOps.second++;
    }
    rep.status = res;
    return;
  case Cmd::Get:
    res = storage_.getValue(req.key, rep.content);
    if(res == Error::S_OK) {
      stat_.getOps.first++;
    }
    else {
      stat_.getOps.second++;
    }
    rep.status = res;
    return;
  default:
    rep.status = Error::E_FAIL;
    return;
  }
}

} // namespace my
