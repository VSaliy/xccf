#include "stdafx.h"
#include "database.h"
#include "server.h"
#include <boost/program_options.hpp>
#include <fstream>
#include <iostream>

int main(int argc, char* argv[])
{
	srand(time(NULL));
	namespace po = boost::program_options;
	po::variables_map vm;
	try
	{
		po::options_description desc;
		desc.add_options()
			("conf_file", po::value<std::string>()->default_value("xcc_forum.conf"))
			("daemon", po::value<bool>()->default_value(true))
			("fcgi_port", po::value<int>()->default_value(2711))
			("help", "")
			("mysql_database", po::value<std::string>()->default_value("xf3"))
			("mysql_host", po::value<std::string>()->default_value("localhost"))
			("mysql_password", po::value<std::string>()->default_value(""))
			("mysql_user", po::value<std::string>()->default_value("xf3"))
			("pid_file", po::value<std::string>()->default_value(""))
			("query_log", po::value<std::string>()->default_value(""))
			("templates_dir", po::value<std::string>()->default_value("."))
			;
		po::store(po::parse_command_line(argc, argv, desc), vm);
		std::ifstream is(vm["conf_file"].as<std::string>().c_str());
		po::store(po::parse_config_file(is, desc), vm);
		po::notify(vm);
		if (vm.count("help"))
		{
			std::cerr << desc;
			return 1;
		}
		database_c database;
		database.open(vm["mysql_host"].as<std::string>(), vm["mysql_user"].as<std::string>(), vm["mysql_password"].as<std::string>(), vm["mysql_database"].as<std::string>(), true);
		database.set_query_log(vm["query_log"].as<std::string>());
		database.read_config();
		database.repair_counts();
		asio::io_service io_service;
		request_handler req_handler(database, vm["templates_dir"].as<std::string>());
		server s(io_service, req_handler, vm["fcgi_port"].as<int>());
#ifndef WIN32
		if (vm["daemon"].as<bool>() && daemon(true, false))
			std::cerr << "daemon failed" << std::endl;
		std::ofstream(vm["pid_file"].as<std::string>().c_str()) << getpid() << std::endl;
#endif
		io_service.run();
		unlink(vm["pid_file"].as<std::string>().c_str());
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return 1;
	}
	return 0;
}
