#pragma once

#include <boost/asio.hpp>
#include <string>
#include <memory>
#include "outputDevice.hpp"

using namespace boost::asio;

class TcpSocket: public OutputDevice
{
public:
	TcpSocket(io_context &ioContext): connectionSocket(ioContext)
	{}

	void Write(std::string_view msg)
	{
		connectionSocket.send(boost::asio::buffer(msg.data(), msg.length()));
	}

	bool Connect(const std::string &ip, uint32_t port)
	{
		ip::tcp::endpoint endpoint(ip::address::from_string(ip), port);
		boost::system::error_code ec;
		connectionSocket.connect(endpoint, ec);

		return !ec;
	}
	
private:
	ip::tcp::socket connectionSocket;
};