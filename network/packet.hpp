#pragma once

#include "Address.hpp"

namespace network
{
constexpr const int buff_size = 512;
using buff_t = std::array<char, buff_size>;

template <typename Iterator>
class packet
{
  public:
    packet(const Address &addr, Iterator begin, Iterator end) : addr(addr), data(begin, end)
    {
    }

    Address addr;
    std::vector<char> data;
};
} // namespace network