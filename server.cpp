#include "server.hpp"

Server::Server(int port) : _port(port), _server_fd(-1) {}

Server::~Server()
{
	if (_server_fd != -1)
		close(_server_fd);
}

void Server::start()
{
	struct sockaddr_in address;

	_server_fd = socket(AF_INET, SOCK_STREAM, 0);
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
	address.sin_port = htons(_port);

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

	std::cout << "Servidor escuchando en el puerto " << _port << std::endl;
}