#pragma once

#include <asio.hpp>
#include "request_handler.h"

class connection;

class server
{
public:
	server(asio::io_service&, request_handler&, int fcgi_port);
	void handle_accept(const asio::error_code&);
	void handle_request(request&);
private:
	asio::ip::tcp::acceptor acceptor_;
	asio::io_service& io_service_;
	boost::shared_ptr<connection> new_connection_;
	request_handler& request_handler_;
};
