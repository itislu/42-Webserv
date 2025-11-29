#include "IInBuffer.hpp"

#include <cstddef>
#include <ostream>

/* ************************************************************************** */
// PUBLIC

std::ostream& operator<<(std::ostream& out, IInBuffer& buffer)
{
  const std::size_t oldpos = buffer.pos();
  std::size_t pos = oldpos;

  out << "'";
  while (pos < buffer.size()) {
    const char chr = buffer.get();
    // printEscapedChar(out, chr);
    out << chr;
    ++pos;
  }
  out << "'\n";
  buffer.seek(oldpos);

  return out;
}
