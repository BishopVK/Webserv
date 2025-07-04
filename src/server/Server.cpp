/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danjimen,isainz-r,serferna <webserv@stu    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 12:09:45 by danjimen,is       #+#    #+#             */
/*   Updated: 2025/07/04 12:03:36 by danjimen,is      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/Server.hpp"

Server::Server() : Config(), _ip(IP_DEFAULT), _is_running(false)
{
	//Set the name to the server
	//_servers_count = 0;
	/* std::stringstream	ss;
	ss << ++_servers_count;
	_server_name = "Server " + ss.str(); */
	_server_name = "server_name";

	//_server_fd = -1;
	//_server_fds.clear();
	//_port = -1;
	_ports.push_back(-1);
	_sockets.clear();
	_locations.clear();
}

Server::Server(int port) : Config(), _ip(IP_DEFAULT), _is_running(false)
{
	//Set the name to the server
	//_servers_count = 0;
	/* std::stringstream	ss;
	ss << ++_servers_count;
	_server_name = "Server " + ss.str(); */
	_server_name = "server_name";

	//_server_fd = -1;
	//_server_fds.clear();
	//_port = port;
	_ports.push_back(port);
	_sockets.clear();
	_locations.clear();
}

Server::Server(const Server &other) : Config(other), _ip(other.getIp()), _is_running(other.isRunning())
{
	_root = other.getRoot();
	_index_files = other.getIndexFiles();
	_autoindex = other.getAutoindex();
	_client_max_body_size = other.getClientMaxBodySize();
	_error_pages = other.getErrorPages();
	_cgi = other.getCgis();
	_return_data = other.getReturnData();
	_methods = other.getMethods();
	_inherit_initizalized = other.getInheritInitialized();
	_ports = other.getPorts();
	_server_name = other.getServerName();
	//_server_fds = other.getServerFds();
	_sockets = other.getSockets();
	_locations = other.getLocations();
}

Server &Server::operator=(const Server &other)
{
	if (this != &other)
	{
		_root = other.getRoot();
		_index_files = other.getIndexFiles();
		_autoindex = other.getAutoindex();
		_client_max_body_size = other.getClientMaxBodySize();
		_error_pages = other.getErrorPages();
		_cgi = other.getCgis();
		_return_data = other.getReturnData();
		_methods = other.getMethods();
		_inherit_initizalized = other.getInheritInitialized();
		_ip = other.getIp();
		_ports = other.getPorts();
		_server_name = other.getServerName();
		//_server_fds = other.getServerFds();
		_sockets = other.getSockets();
		_locations = other.getLocations();
		_is_running = other.isRunning();
	}
	return (*this);
}

Server::~Server()
{
	_is_running = false;
	_locations.clear();

	/* std::vector<int>::iterator it;
	for (it = _server_fds.begin(); it != _server_fds.end(); ++it)
		close(*it); */
	/* if (_server_fd != -1)
		close(_server_fd); */
}

/* GETERS and SETERS */
// ip
std::string	Server::getIp() const { return _ip; }

void		Server::setIp(const std::string &ip)
{
	if (ip != "localhost")
	{
		std::vector<std::string>	tokens;
		size_t						start = 0;
		size_t						end;
		
		while ((end = ip.find('.', start)) != std::string::npos)
		{
			tokens.push_back(ip.substr(start, end - start));
			start = end + 1;
		}

		// Add last part
		tokens.push_back(ip.substr(start));
		if (tokens.size() != 4)
			throw ErrorException(ip + ": invalid ip format");
			
		for (std::vector<std::string>::iterator it = tokens.begin(); it != tokens.end(); ++it)
		{
			int nbr = std::atoi((*it).c_str());
			if ((*it).empty() || nbr < 0 || nbr > 255)
				throw ErrorException(ip + ": invalid ip format");
		}
	}
	_ip = ip;
}

// ports
std::vector<int>	Server::getPorts() const { return _ports; }

void				Server::addPort(int port)
{
	if (_ports.size() == 1 && _ports.at(0) == -1)
		_ports.clear();
	if (!hasPort(port) && port > 0 && port < MAX_PORT)
		_ports.push_back(port);
	else
	{
		std::stringstream ss;
		ss << port;
		std::string port_str = ss.str();
		throw ErrorException(port_str + ": invalid port");
	}
}

bool				Server::hasPort(int port) const
{
	return std::find(_ports.begin(), _ports.end(), port) != _ports.end();
}

// port
/* int	Server::getPort() const { return _port; }
void				Server::addPort(int port)
{
	if (_port == -1)
		_port = port;
}
bool				Server::hasPort(int port) const
{
	return _port == port;
} */

// server_name
std::string	Server::getServerName() const { return _server_name; }

void		Server::setServerName(const std::string &server_name) { _server_name = server_name; }

// sockets
std::vector<int>	Server::getSockets() const { return _sockets; }

bool				Server::hasSocket(int socket) const
{
	return std::find(_sockets.begin(), _sockets.end(), socket) != _sockets.end();
}

// server_fds
/* std::vector<int>	Server::getServerFds() const { return _server_fds; }

void				Server::setServerFds(int fd)
{
	std::vector<int>::iterator it;
	for (it = _server_fds.begin(); it != _server_fds.end(); ++it)
	{
		if (*it == fd)
			throw ErrorException("Trying to add repeated fd to _server_fds");
	}
	_server_fds.push_back(fd);
} */

// locations
std::vector<Location>	Server::getLocations() const { return _locations; }

const Location*	Server::getLocation(std::string route) const
{
	std::vector<Location>::const_iterator it;

	for (it = _locations.begin(); it != _locations.end(); ++it)
	{
		if (route == (*it).getRoute())
			return &(*it);
	}
	throw ErrorException("Location not found al getLocation()");
}

void	Server::addLocation(Location location)
{
	std::vector<Location>::iterator it;

	for (it = _locations.begin(); it != _locations.end(); ++it)
	{
		if (location.getRoute() == (*it).getRoute())
			throw ErrorException("Location already exists");
	}
	_locations.push_back(location);
}



/* std::map<std::string, Location>		Server::getLocations() const { return _locations; }
const Location*	Server::getLocation(std::string route) const
{
	std::map<std::string, Location>::const_iterator it;
	int index;

	while (true)
	{
		it = _locations.find(route);
		if (it != _locations.end())
			return &(it->second); // Se encontró coincidencia exacta. Devolvemos el puntero

		index = route.rfind("/");
		std::string last = route.substr(0, index);
		if (last.empty())
			last = "/";
		if (last == route)
			break;
		route = last;
	}

	return NULL; // No se encontró ninguna coincidencia
}

void	Server::addLocation(const std::string &route, Location location)
{
	if (_locations.find(route) == _locations.end())
	{
		location.inherit(*this);
		_locations.insert(std::pair<std::string, Location>(route, location));
	}
} */

// is_running
bool	Server::isRunning() const { return _is_running; }


// TESTING
/* Server::Server(int port) : _port(port), _server_fd(-1) {}

Server::~Server()
{
	if (_server_fd != -1)
		close(_server_fd);
} */

void Server::start()
{
	struct sockaddr_in address;

	//_server_fd = socket(AF_INET, SOCK_STREAM, 0);
	int _server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (_server_fd == -1)
	{
		perror("socket");
		exit(EXIT_FAILURE);
	}

	int opt = 1;
	if (setsockopt(_server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
	{
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}

	std::memset(&address, 0, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(_ports.at(0));

	if (bind(_server_fd, (struct sockaddr*)&address, sizeof(address)) < 0)
	{
		perror("bind");
		exit(EXIT_FAILURE);
	}

	if (listen(_server_fd, 10) < 0)
	{
		perror("listen");
		exit(EXIT_FAILURE);
	}

	std::cout << "Servidor escuchando en el puerto " << _ports.at(0) << std::endl;
}

/* TOOLS */
void	Server::print() const
{
	// root
	std::cout << "root = " << _root << std::endl;

	// index_files
	if (!_index_files.empty())
	{
		std::cout << "index_files:" << std::endl;
		std::vector<std::string>::const_iterator it;
		for (it = _index_files.begin(); it != _index_files.end(); ++it)
			std::cout << "\t- " << *it << std::endl;
	}

	// autoindex
	std::cout << "autoindex = " << (_autoindex ? std::string("true") : std::string("false")) << std::endl;

	// client_max_body_size
	std::cout << "client_max_body_size = " << _client_max_body_size << "M" << std::endl;

	// error_pages
	if (!_error_pages.empty())
	{
		std::cout << "error_pages:" << std::endl;
		std::map<int, std::string>::const_iterator it;
		for (it = _error_pages.begin(); it != _error_pages.end(); ++it)
			std::cout << "\t- " << it->first << " => " << it->second << std::endl;
	}

	// ip
	std::cout << "ip = " << _ip << std::endl;

	// ports
	if (!_ports.empty())
	{
		std::cout << "ports:" << std::endl;
		std::vector<int>::const_iterator it;
		for (it = _ports.begin(); it != _ports.end(); ++it)
			std::cout << "\t- " << *it << std::endl;
	}

	// server_name
	std::cout << "server_name = " << getServerName() << std::endl;

	// server_fd
	/* std::cout << "server_fds:" << std::endl;
	std::vector<int>::const_iterator it;
	for (it = _server_fds.begin(); it != _server_fds.end(); ++it)
		std::cout << "\t- " << *it << std::endl; */

	// sockets
	if (!_sockets.empty())
	{
		std::cout << "sockets:" << std::endl;
		std::vector<int>::const_iterator it;
		for (it = _sockets.begin(); it != _sockets.end(); ++it)
			std::cout << "\t- " << *it << std::endl;
	}

	// is_running
	std::cout << "is_running = " << (_is_running ? std::string("true") : std::string("false")) << std::endl;

	// locations
	if (!getLocations().empty())
	{
		std::cout << "locations:" << std::endl;
		std::vector<Location>::const_iterator it;
		for (it = _locations.begin(); it != _locations.end(); ++it)
		{
			std::cout << "--------------------" << std::endl;
			it->print();
			std::cout << "--------------------" << std::endl;
		}
	}
}