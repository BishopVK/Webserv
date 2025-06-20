/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danjimen,isainz-r,serferna <webserv@stu    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 13:10:56 by danjimen,is       #+#    #+#             */
/*   Updated: 2025/06/20 13:25:11 by danjimen,is      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <cstring>      // memset
#include <cstdlib>      // exit
#include <unistd.h>     // close
#include <netinet/in.h> // sockaddr_in, htons
#include <sys/socket.h> // socket, setsockopt, bind, listen
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>

class Server
{
	private:
		int	_port;
		int	_server_fd;

	public:
		Server(int port);
		~Server();

		void start();
};

#endif