#include "stdafx.h"
#include "connection.h"
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#include <algorithm>
#include "server.h"
#include "stream_int.h"

enum
{
	FCGI_BEGIN_REQUEST = 1,
	FCGI_ABORT_REQUEST,
	FCGI_END_REQUEST,
	FCGI_PARAMS,
	FCGI_STDIN,
	FCGI_STDOUT,
	FCGI_STDERR,
	FCGI_DATA,
	FCGI_GET_VALUES,
	FCGI_GET_VALUES_RESULT,
	FCGI_UNKNOWN_TYPE,
};

static std::string type2a(int v)
{
	switch (v)
	{
	case 1:
		return "FCGI_BEGIN_REQUEST";
	case 2:
		return "FCGI_ABORT_REQUEST";
	case 3:
		return "FCGI_END_REQUEST";
	case 4:
		return "FCGI_PARAMS";
	case 5:
		return "FCGI_STDIN";
	case 6:
		return "FCGI_STDOUT";
	case 7:
		return "FCGI_STDERR";
	case 8:
		return "FCGI_DATA";
	case 9:
		return "FCGI_GET_VALUES";
	case 10:
		return "FCGI_GET_VALUES_RESULT";
	case 11:
		return "FCGI_UNKNOWN_TYPE";
	}
	return boost::lexical_cast<std::string>(v);
}

connection::connection(asio::io_service& io_service, server& server):
	server_(server),
	socket_(io_service)
{
}

void connection::start()
{
	asio::async_read(socket_, asio::buffer(buffer_.begin(), 8), boost::bind(&connection::handle_read0, shared_from_this(), asio::placeholders::error));
}

void connection::handle_read0(const asio::error_code& e)
{
	if (e)
		return;
	asio::async_read(socket_, asio::buffer(&buffer_[8], read_int(2, &buffer_[4]) + buffer_[6]), boost::bind(&connection::handle_read1, shared_from_this(), asio::placeholders::error));
}

void connection::handle_read1(const asio::error_code& e)
{
	if (e)
		return;
	int request_id = read_int(2, &buffer_[2]);
	int content_length = read_int(2, &buffer_[4]);
	std::cout
		// << "version: " << static_cast<int>(buffer_[0]) << std::endl
		// << "type: " << type2a(buffer_[1]) << std::endl
		// << "request id: " << request_id << std::endl
		// << "content length: " << content_length << std::endl
		// << "padding length: " << static_cast<int>(buffer_[6]) << std::endl
		;
	const unsigned char* w = &buffer_[8] + content_length;
	if (buffer_[1] == FCGI_BEGIN_REQUEST)
	{
		if (content_length < 8)
			return;
		if (requests_.count(request_id))
			return;
		requests_[request_id];
		std::cout
			// << "role: " << read_int(2, &buffer_[8]) << std::endl
			// << "flags: " << static_cast<int>(buffer_[10]) << std::endl
			;
	}
	else if (buffer_[1] == FCGI_PARAMS && requests_.count(request_id))
	{
		request& req = requests_[request_id];
		for (const unsigned char* r = &buffer_[8]; r != w; )
		{
			int cb_name;
			int cb_value;
			if (r == w)
				return;
			if (*r & 0x80)
			{
				if (r + 4 > w)
					return;
				cb_name = read_int(4, r) & 0x7fffffff;
				r += 4;
			}
			else
				cb_name = *r++;
			if (r == w)
				return;
			if (*r & 0x80)
			{
				if (r + 4 > w)
					return;
				cb_value = read_int(4, r) & 0x7fffffff;
				r += 4;
			}
			else
				cb_value = *r++;
			if (r + cb_name + cb_value > w)
				return;
			// std::cout.write(reinterpret_cast<const char*>(r), cb_name);
			std::string& value = req.arguments_[std::string(reinterpret_cast<const char*>(r), cb_name)];
			r += cb_name;
			// std::cout << " = ";
			// std::cout.write(reinterpret_cast<const char*>(r), cb_value);
			value = std::string(reinterpret_cast<const char*>(r), cb_value);
			// std::cout << std::endl;
			r += cb_value;
		}
	}
	else if (buffer_[1] == FCGI_STDIN && requests_.count(request_id))
	{
		request& req = requests_[request_id];
		if (content_length)
			req.input_ += std::string(reinterpret_cast<const char*>(&buffer_[8]), content_length);
		else
		{
			req.handle();
			server_.handle_request(req);
			if (req.output_.size() > buffer_.size() - 256)
				return;
			// std::cout << std::endl;
			unsigned char* w = &buffer_[0];
			for (int i = 0; i < req.output_.size();)
			{
				int cb = std::min<int>(req.output_.size() - i, 0xffff);
				w[0] = 1;
				w[1] = FCGI_STDOUT;
				write_int(2, w + 2, request_id);
				write_int(2, w + 4, cb);
				w[6] = 0;
				w[7] = 0;
				w += 8;
				memcpy(w, req.output_.data() + i, cb);
				w += cb;
				i += cb;
			}
			w[0] = 1;
			w[1] = FCGI_STDOUT;
			write_int(2, w + 2, request_id);
			write_int(2, w + 4, 0);
			w[6] = 0;
			w[7] = 0;
			w += 8;
			w[0] = 1;
			w[1] = FCGI_END_REQUEST;
			write_int(2, w + 2, request_id);
			write_int(2, w + 4, 0);
			w[6] = 0;
			w[7] = 0;
			w[8] = 0;
			w[9] = 0;
			w[10] = 0;
			w[11] = 0;
			w[12] = 0; // FCGI_REQUEST_COMPLETE
			w[13] = 0;
			w[14] = 0;
			w[15] = 0;
			w += 16;
			asio::async_write(socket_, asio::buffer(buffer_.begin(), w - buffer_.begin()), boost::bind(&connection::handle_write, shared_from_this(), asio::placeholders::error));
			return;
		}
	}
	else
	{
		for (const unsigned char* r = &buffer_[8]; r != w; r++)
			std::cout << static_cast<int>(*r) << ' ';
		std::cout << std::endl;
	}
	// std::cout << std::endl;
	start();
}

void connection::handle_write(const asio::error_code& e)
{
	if (e)
		return;
	start();
}
