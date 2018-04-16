#include "NetworkFullTcp.class.h"

using namespace std;
using namespace boost::asio;
using ip::tcp;





NetworkFullTcp::NetworkFullTcp(Fifo<Message> message, Fifo<Frame> frame): NetworkAbstract(Fifo<Message> message, Fifo<Frame> frame) {

}

NetworkFullTcp::listen(int port) {

}
