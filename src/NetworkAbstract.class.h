


#ifndef NETWORK_ABSTRACT_H
#define NETWORK_ABSTRACT_H

typedef boost::shared_ptr<tcp::socket> socket_ptr;

class NetworkAbstract {
	public:
		NetworkAbstract(Fifo<Message> message, Fifo<Frame> frame) {
			this->inputMessageQueue = message;
			this->outputFrameQueue = frame;
		}
		~NetworkAbstract() {
		
		}
		virtual void listen(int port) = 0;

	private:
		Fifo<Message> inputMessageQueue;
		Fifo<Frame> outputFrameQueue;


};
#endif NETWORK_ABSTRACT_H
