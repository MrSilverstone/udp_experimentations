#include <iostream>
#include <string>
#include <condition_variable>
#include <mutex>

#include "Server.hh"

using namespace network;

std::mutex receive_mutex_;
std::condition_variable receive_cond_;

std::mutex send_mutex_;
std::condition_variable send_cond_;

Server::Server(const std::string& listen_addr, unsigned short port) :
    running_(true),
    receive_thread_(&Server::receive_handler, this, listen_addr, port),
    send_thread_(&Server::send_handler, this)
{
}

Server::~Server()
{
  running_ = false;
  receive_cond_.notify_all();
  send_cond_.notify_all();
}

void Server::receive_handler(const std::string &listen_addr, unsigned short port)
{
  Address addr(listen_addr, port);
  sock_.open(addr);  
  
  while (running_.load())
  {
    Address sender;
    int len = sock_.receive(sender, buff_.data(), buff_.size() - 1);

    {
      std::unique_lock<std::mutex> lk(receive_mutex_);
      std::cout << "Received : [" << std::string(buff_.begin(), buff_.begin() + len) << "]" << std::endl;
      received_packets_.push_front(packet<buff_t::const_iterator>(sender, buff_.begin(), buff_.begin() + len));
      receive_cond_.notify_one();
    }
  }
}

void Server::send_handler()
{
  while (running_.load())
  {
    std::deque<packet<buff_t::const_iterator>> packets;
    
    {
      std::unique_lock<std::mutex> lk(send_mutex_);
      send_cond_.wait(lk);

      packets = packets_to_send_;
      packets_to_send_.clear();
    }

    while (!packets.empty())
    {
      auto p = packets.back();
      packets.pop_back();
      std::cout << "Sending : [" << std::string(p.data.begin(), p.data.end()) << "]" << std::endl;
      sock_.send(p.addr, p.data.data(), p.data.size());
    }
  }
}

#include <chrono>

using namespace std::chrono_literals;

void Server::run()
{

  
  while (running_.load())
  {
    std::deque<packet<buff_t::const_iterator>> packets;
    
    {
      std::unique_lock<std::mutex> lk(receive_mutex_);
      receive_cond_.wait(lk);
      packets = received_packets_;
      received_packets_.clear();
    }

    while (!packets.empty())
    {
      auto p = packets.back();
      std::cout << "Processing : [" << std::string(p.data.begin(), p.data.end()) << "]" << std::endl;
      //      std::this_thread::sleep_for(2s);
      
      {
        std::unique_lock<std::mutex> lk(send_mutex_);
        packets_to_send_.push_front(p);
        packets.pop_back();
      }
      send_cond_.notify_one();
    }
  }
}
