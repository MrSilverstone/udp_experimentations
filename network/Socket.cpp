#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include "Socket.hh"


using namespace network::udp;

bool Socket::open(const Address& listen_addr)
{
  socket_ = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  if (socket_ <= 0)
  {
    std::cerr << "Error while creating socket" << std::endl;
    return false;
  }

  int re_use = 1;  
  if (setsockopt(socket_, SOL_SOCKET, SO_REUSEADDR, &re_use, sizeof(int)) < 0)
  {
    std::cerr << "SO_REUSEADDR error" << std::endl;
    return false;
  }
  
  if (bind(socket_, listen_addr.get_addr(), Address::size()) < 0)
  {
    std::cerr << "Error while binding socket" << std::endl;
    return false;
  }

  std::cout << "Listening on " << listen_addr << std::endl;
  
  return true;
}

void Socket::close()
{
  if (socket_ != -1)
  {
    ::close(socket_);
  }
}

bool Socket::send(const Address& dest, const void* data, int len)
{
  int sent_bytes = sendto(socket_, reinterpret_cast<const char*>(data), len, 0, dest.get_addr(), Address::size());

  if (sent_bytes != len)
  {
    std::cerr << "Error while sending packet" << std::endl;
    return false;
  }
  
  return true;
}

int Socket::receive(Address& sender, void* data, int len)
{
  sockaddr_in from;
  socklen_t fromLength = sizeof(from);

  int bytes = recvfrom(socket_, reinterpret_cast<char*>(data), len, 0, reinterpret_cast<sockaddr*>(&from), &fromLength);
  sender.set_from_sockaddr(from);
  
  return bytes;
}
