#include "stdafx.h"
#include "server.h"

#include <boost/bind.hpp>
#include "connection.h"

server::server(asio::io_service& io_service, request_handler& req_handler, int fcgi_port):
	acceptor_(io_service, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), fcgi_port)),
	io_service_(io_service),
	new_connection_(new connection(io_service, *this)),
	request_handler_(req_handler)
{
#ifdef TCP_DEFER_ACCEPT
	int v = true;
	setsockopt(acceptor_.native(), IPPROTO_TCP, TCP_DEFER_ACCEPT, &v, sizeof(v));
#endif
	acceptor_.async_accept(new_connection_->socket(), boost::bind(&server::handle_accept, this, asio::placeholders::error));
}

void server::handle_accept(const boost::system::error_code& e)
{
	if (e)
		return;
	new_connection_->start();
	new_connection_.reset(new connection(io_service_, *this));
	acceptor_.async_accept(new_connection_->socket(), boost::bind(&server::handle_accept, this, asio::placeholders::error));
}

void server::handle_request(request& req)
{
	request_handler_.handle(req);
}
