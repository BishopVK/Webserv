/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danjimen,isainz-r,serferna <webserv@stu    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 13:10:56 by danjimen,is       #+#    #+#             */
/*   Updated: 2025/06/26 22:21:45 by danjimen,is      ###   ########.fr       */
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
		//std::vector<int>				_ports;
		std::string						_server_name;
		int								_server_fd;
		std::vector<int>				_sockets;
		int								_servers_count; // Number of servers created
		std::map<std::string, Location>	_locations; // List of the server locations
		bool							_is_running; // Flag to know if the server is running

	public:
		Server();
		Server(int port);
		~Server();

		/* GETERS and SETERS */
		// ip
		std::string	getIp() const;
		void		setIp(const std::string &ip);

		// ports
		//std::vector<int>	getPorts() const;
		int					getPorts() const;
		void				addPort(int port);
		bool				hasPort(int port) const;

		// server_name
		std::string	getServerName() const;
		void		setServerName(const std::string &server_name);

		// sockets
		std::vector<int>	getSockets() const;
		bool				hasSocket(int socket) const;

		// locations
		std::map<std::string, Location>	getLocations() const;
		const Location*					getLocation(std::string route) const;
		void							addLocation(const std::string &route, Location location);

		// is_running
		bool	isRunning() const;

		void start();
		void stop();
};