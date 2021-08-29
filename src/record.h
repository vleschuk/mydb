#ifndef MY_RECORD_H_
#define MY_RECORD_H_

#include <boost/interprocess/allocators/allocator.hpp>
#include <boost/interprocess/containers/string.hpp>
#include <boost/interprocess/managed_mapped_file.hpp>

namespace bip = boost::interprocess;

namespace my {

/* shared_string is a string type placeable in shared memory,
 * courtesy of Boost.Interprocess.
 */

typedef bip::basic_string<
  char,std::char_traits<char>,
  bip::allocator<char,bip::managed_mapped_file::segment_manager>
> SharedString;

/* DB record. All its members can be placed in shared memory,
 * hence the structure itself can too.
 */

struct Record
{
    SharedString key;
    SharedString value;

    Record(const SharedString::allocator_type& al):
        key(al), value(al)
    {}
};

} // namespace my
#endif // MY_RECORD_H_
