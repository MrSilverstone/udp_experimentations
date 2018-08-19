#include "Address.hpp"

namespace network
{
class Connection
{
  public:
    Connection(const Address& addr, int id) : addr_(addr)
    {
    }

    private:
    Address addr_;
    int id;
    long last_packt_ = 0;
    long expiration_ = 0;
};
} // namespace network