#include "stdafx.h"
#include "inc\Output2Socket.h"
#include <array>
#include <boost/asio.hpp>

using namespace boost::asio;
using boost::asio::ip::udp;

void Output2Socket::Output(const std::string& msg)
{
	try
	{
		boost::asio::io_service io_service;

		//Querry host and port
		udp::resolver resolver(io_service);
		udp::resolver::query query(udp::v4(), "localhost", "514");
		udp::endpoint receiver_endpoint = *resolver.resolve(query);

		//Init udp socket
		udp::socket socket(io_service);
		socket.open(udp::v4());

		//send message
		size_t bytes_sent = socket.send_to(boost::asio::buffer(msg, msg.size()), receiver_endpoint);
		printf("bytes sent[%d]\n", bytes_sent);
	}
	catch (boost::system::system_error& error)
	{
		printf("error resolve[%s]", error.what());
	}

}