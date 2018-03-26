#include "network.hpp"
#include "main.hpp"

using namespace std;
using namespace boost::asio;
using ip::tcp;


typedef boost::shared_ptr<tcp::socket> socket_ptr;


void new_incoming_socket_handler(socket_ptr sock)
{
	boost::system::error_code error;
	bool socket_running = true;
	try
	{
		sock->set_option(tcp::no_delay(true));
		char data[sizeof(Message)];
		Message* m;
		while(socket_running)
		{
			size_t length = sock->read_some(buffer(data), error);
			if (error == error::eof) {
				std::cout << "connection close clearly must do action" << std::endl;
				return; // Connection closed cleanly by peer.

			}
			else if (error) {
				std::cout << "network error send stop command" << std::endl;
				throw boost::system::system_error(error); // Some other error.
			}
			//std::cout << "receiving new message" << std::endl;
			m = (Message*)data;
			handle_incoming_message(m);
		}

	}
	catch (exception& e)
	{
	socket_running = false;
	cerr << "Exception in thread: " << e.what() << "\n";

	}

}

void socket_sender_thread_fn(socket_ptr sock)
{
	int counter = 1;

	while(true) 
	{
		Frame* frame = queueToNetwork->get();
		if(frame) 
		{
			void* fullFrame = malloc(frame->size + 8);
			memcpy(fullFrame, (void *) &counter, 4);
			memcpy(fullFrame+4, (void *) &frame->size, 4);
			memcpy(fullFrame+8, (void *) frame->data, frame->size);

			boost::asio::write(*sock, buffer(fullFrame, frame->size+8));
			counter++;
		} else {
			boost::this_thread::sleep(boost::posix_time::milliseconds(1));
		}


	}
}

void SRD_network_listen_tcp_socket(io_service& io_service, short port)
{
	// TODO only accept one time

	tcp::acceptor a(io_service, tcp::endpoint(tcp::v4(), port));
	for (;;)
	{
		socket_ptr sock(new tcp::socket(io_service));
		a.accept(*sock);
		std::cout << "new socket connected" << std::endl;
		boost::thread socketReceiveThead(boost::bind(new_incoming_socket_handler, sock));
		boost::thread socketSendThread(boost::bind(socket_sender_thread_fn, sock));

	}

}


void SRD_server_init_listen()
{
	try
	{
		io_service io_service;
		SRD_network_listen_tcp_socket(io_service, 8001);
	}
	catch (std::exception& e)
	{
		//TODO
	}

}



