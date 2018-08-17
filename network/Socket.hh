#pragma once

#include "Address.hpp"

namespace network
{
  namespace udp
  {
    class Socket
    {
     public:
      bool open(const Address& listen_addr);
      void close();
      inline bool is_open() const { return socket_ != -1; };
      bool send(const Address& dest, const void* data, int len);
      int receive(Address& sender, void* data, int len);
      
     private:
      int socket_ = -1;
    };
  }
}
