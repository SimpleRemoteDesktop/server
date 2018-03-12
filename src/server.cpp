#include <fstream>
#include <algorithm>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <bounded_buffer.h>
#include <boost/circular_buffer.hpp>



enum type {TYPE_KEY_DOWN=1, TYPE_KEY_UP=2, TYPE_MOUSE_MOTION=3, TYPE_MOUSE_DOWN=4, TYPE_MOUSE_UP=5 , TYPE_ENCODER_START=6, TYPE_ENCODER_STOP=7 };

typedef Message Message;
struct Message
{
	int type;
	float x;
	float y;
	int button;
	int keycode;
	int codec_width;
	int codec_height;
	int bandwidth;
	int fps;
	int sdl;

};


boost::circular_buffer<Message> cb(5);

void new_incoming_socket_handler(socket_ptr sock)
{
	try
	{
		sock->set_option(tcp::no_delay(true));
		char data[max_length];

		boost::system::error_code error;
		size_t length = sock->read_some(buffer(data), error);

		//TODO network

		if (error == error::eof)
			return; // Connection closed cleanly by peer.
		else if (error)
			throw boost::system::system_error(error); // Some other error.

		if (data[0] == 'a'){
			sessionVideo(sock, monitorID, screenCoordinates);

		} else if (data[0] == 'b'){
			sessionKeystroke(sock, screenCoordinates);

		} else {
			cout << "Received a connection with a wrong identification buffer " << string(data, length) << endl;

		}

	}
	catch (exception& e)
	{
		cerr << "Exception in thread: " << e.what() << "\n";

	}

}

void SRD_network_listen_tcp_socket(io_service& io_service, short port)
{
	tcp::acceptor a(io_service, tcp::endpoint(tcp::v4(), port));
	for (;;)
	{
		socket_ptr sock(new tcp::socket(io_service));
		a.accept(*sock);
		boost::thread t(boost::bind(new_incoming_socket_handler, sock));

	}

}


int main(int argc, const char* argv[])
{
	cout << " Simple Remote desktop server version 0.2" << endl;


	try
	{
		io_service io_service;

		SRD_network_listen_tcp_socket(io_service, 8001);
	}
	catch (exception& e)
	{
		cerr << "Exception: " << e.what() << "\n";
	}
	return 0;
}
}
}
}
