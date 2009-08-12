#pragma once

#include <boost/asio.hpp>
#include <boost/enable_shared_from_this.hpp>
#include "request.h"

namespace asio = boost::asio;

class server;

class connection:
	public boost::enable_shared_from_this<connection>,
	private boost::noncopyable
{
public:
	connection(asio::io_service&, server&);
	void start();

	asio::ip::tcp::socket& socket()
	{
		return socket_;
	}
private:
	void handle_read0(const boost::system::error_code&);
	void handle_read1(const boost::system::error_code&);
	void handle_write(const boost::system::error_code&);

	typedef std::map<int, request> requests_t;

	boost::array<unsigned char, 257 << 10> buffer_;
	requests_t requests_;
	server& server_;
	asio::ip::tcp::socket socket_;
};
