/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danjimen,isainz-r,serferna <webserv@stu    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 13:10:56 by danjimen,is       #+#    #+#             */
/*   Updated: 2025/06/24 17:51:25 by danjimen,is      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Config.hpp"
#include "Location.hpp"

class Server : public Config
{
	private:
		std::string						_ip;
		int								_port;
		std::vector<int>				_ports;
		std::string						_server_name;
		//int								_server_fd;
		std::vector<int>				_sockets;
		static int						_servers_count; /* Number of servers created */
		std::map<std::string, Location>	_locations; /* List of the server locations */

	public:
		Server(int port);
		~Server();

		void start();
};