#pragma once
#ifndef BUFFER_HPP
#define BUFFER_HPP

#include <cstddef>
#include <string>
#include <sys/types.h>
#include <vector>

/* ************************************************************************** */
class Buffer
{
public:
  typedef std::vector<unsigned char> Container;
  typedef Container::iterator iterator;

  Buffer();
  explicit Buffer(std::size_t size);

  void add(const std::string& str);
  void add(const Container& buffer);
  void remove(ssize_t bytes);
  void remove(size_t bytes);
  std::size_t getSize() const;
  bool isEmpty() const;
  unsigned char* data();

  iterator begin();
  iterator end();
  iterator getIterAt(long offset);

  std::string getString(long fromIndex, long toIndex) const;

private:
  Container _buff;
};

#endif // BUFFER_HPP
