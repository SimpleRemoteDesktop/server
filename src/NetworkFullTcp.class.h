#include "NetworkAbstract.class.h"


class NetworkFullTcp: public NetworkAbstract {
	public:
	       NetworkFullTcp(Fifo<Message> message, Fifo<Frame>);
	       void listen(int port);

	private:


}
