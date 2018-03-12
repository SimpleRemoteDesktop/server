#include "network.hpp"

boost::lockfree::queue<int> FrameToSockQueue(128);

void new_incoming_socket_handler(socket_ptr sock)
{
	try
	{
		sock->set_option(tcp::no_delay(true));
		char data[sizeof(Message)];
		Message* m;
		while(true)
		{
			size_t length = sock->read_some(buffer(data), error);
			if (error == error::eof)
				return; // Connection closed cleanly by peer.
			else if (error)
				throw boost::system::system_error(error); // Some other error.

			m = (Message*)data;
			handle_incoming_message(m);
		}

	}
	catch (exception& e)
	{
		cerr << "Exception in thread: " << e.what() << "\n";

	}

}

void socket_sender_thread_fn(socket_ptr sock)
{
	 // TODO send data thread to socket
}

void SRD_network_listen_tcp_socket(io_service& io_service, short port)
{
	// TODO only accept one time

	tcp::acceptor a(io_service, tcp::endpoint(tcp::v4(), port));
	for (;;)
	{
		socket_ptr sock(new tcp::socket(io_service));
		a.accept(*sock);
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
	catch (exception& e)
	{
		cerr << "Exception: " << e.what() << "\n";
	}

}



