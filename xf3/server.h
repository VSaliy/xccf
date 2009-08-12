#pragma once

#include <boost/asio.hpp>
#include "request_handler.h"

namespace asio = boost::asio;

class connection;

class server
{
public:
	server(asio::io_service&, request_handler&, int fcgi_port);
	void handle_accept(const boost::system::error_code&);
	void handle_request(request&);
private:
	asio::ip::tcp::acceptor acceptor_;
	asio::io_service& io_service_;
	boost::shared_ptr<connection> new_connection_;
	request_handler& request_handler_;
};
