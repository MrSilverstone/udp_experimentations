#pragma once

#include <thread>
#include <string>
#include <array>
#include <atomic>
#include <vector>
#include <deque>
#include <iostream>
#include "Socket.hh"
#include "packet.hpp"

namespace network
{  
  class Server
  {
   public:
    Server(const std::string& listen_addr, unsigned short port);
    ~Server();
    void run();
    
   private:
    std::atomic<bool> running_;
    std::thread receive_thread_;
    std::thread send_thread_;
    udp::Socket sock_;
    buff_t buff_;
    std::deque<packet<buff_t::const_iterator>> received_packets_;
    std::deque<packet<buff_t::const_iterator>> packets_to_send_;
    
   private:
    void receive_handler(const std::string& listen_addr, unsigned short port);
    void send_handler();

    void send_packet(const packet<buff_t::const_iterator> & p);
  };
}
