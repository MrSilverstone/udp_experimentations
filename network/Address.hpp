#pragma once

#include <string>
#include <sstream>
#include <vector>
#include <stdexcept>
#include <arpa/inet.h>

namespace network
{
  class Address
  {
   public:
    Address() { }
    
    Address(const std::string& addr, unsigned port)
    {
      std::istringstream ss(addr);
      std::string token;      
      std::vector<unsigned int> tokens;
      
      while (std::getline(ss, token, '.'))
      {
        int i = std::stoi(token);
        if (i > 255 || i < 0)
        {
          throw std::invalid_argument("Bad address");
        }
        tokens.push_back(static_cast<unsigned int>(i));
      }

      if (tokens.size() != 4)
      {
        throw std::invalid_argument("Bad address");
      }
      
      unsigned address = (tokens[0] << 24) |
                         (tokens[1] << 16) |
                         (tokens[2] << 8) |
                         tokens[3];

      addr_.sin_family = AF_INET;
      addr_.sin_addr.s_addr = htonl(address);
      addr_.sin_port = htons(port);
      port_ = port;

      port_ = ntohs(addr_.sin_port);
      calc_addr(addr_.sin_addr.s_addr);
    }

    Address(int a, int b, int c, int d, unsigned short port)
    {
      unsigned address = (a << 24) |
                         (b << 16) |
                         (c << 8) |
                         d;

      addr_.sin_family = AF_INET;
      addr_.sin_addr.s_addr = htonl(address);
      addr_.sin_port = htons(port);
      port_ = port;

      port_ = ntohs(addr_.sin_port);
      calc_addr(addr_.sin_addr.s_addr);
    }
    
    const sockaddr* get_addr() const
    {
      return reinterpret_cast<const sockaddr*>(&addr_);
    }
    
    static constexpr socklen_t size(){ return sizeof(sockaddr_in); }

    void set_from_sockaddr(const sockaddr_in& addr)
    {
      addr_.sin_family = addr.sin_family;
      addr_.sin_addr.s_addr = addr.sin_addr.s_addr;
      addr_.sin_port = addr.sin_port;

      port_ = ntohs(addr.sin_port);

      calc_addr(addr_.sin_addr.s_addr);
    }

    std::string to_string() const
    {
      std::stringstream sstr;
      for (int i = 0; i < 4; i++)
      {
        sstr << (int)raw_addr_[i];
        if (i < 3)
          sstr << ".";        
      }

      sstr << ":" << port_;
      
      return sstr.str();
    }
    
   private:
    sockaddr_in addr_ = {};
    unsigned short port_;
    unsigned char raw_addr_[4];

   private:
    void calc_addr(int hAddr)
    {      
      raw_addr_[0] = hAddr & 0xff;
      raw_addr_[1] = hAddr >> 8 & 0xff;
      raw_addr_[2] = hAddr >> 16 & 0xff;
      raw_addr_[3] = hAddr >> 24 & 0xff;
    }
  };
};

inline std::ostream& operator<<(std::ostream& out, const network::Address& addr)
{
  out << addr.to_string();
  return out;
}
