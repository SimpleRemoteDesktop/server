


#ifndef NETWORK_ABSTRACT_H
#define NETWORK_ABSTRACT_H

class NetworkAbstract {
	public:
		NetworkAbstract(Fifo<Message> message, Fifo<Frame> frame) {
			this->inputMessageQueue = message;
			this->outputFrameQueue = frame;
		}
		~NetworkAbstract() {

		}
	private:
		Fifo<Message> inputMessageQueue;
		Fifo<Frame> outputFrameQueue;


};
#endif NETWORK_ABSTRACT_H
