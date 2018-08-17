#include <iostream>
#include <limits>
#include <stdexcept>
#include "Server.hh"

unsigned short get_port(const char* p)
{
  int port = std::stoi(p);

  if (port > std::numeric_limits<unsigned short>::max() || port <= 0)
  {
    throw std::invalid_argument("Invalid port");
  }
  
  return static_cast<unsigned short>(port);
}

int main(int argc, char** argv)
{
  if (argc < 2)
  {
    std::cout << "Usage: ./server PORT" << std::endl;
    return 1;    
  }

  unsigned short port = get_port(argv[1]);

  network::Server server("127.0.0.1", port);

  server.run();
  
  return 0;
}
