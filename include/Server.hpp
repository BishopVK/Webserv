/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danjimen,isainz-r,serferna <webserv@stu    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 13:10:56 by danjimen,is       #+#    #+#             */
/*   Updated: 2025/07/04 12:12:07 by danjimen,is      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Config.hpp"
#include "Location.hpp"

class Server : public Config
{
	private:
		std::string						_ip;
		std::vector<int>				_ports;
		std::string						_server_name;
		std::vector<int>				_sockets;
		std::vector<Location>			_locations;
		bool							_is_running; // Flag to know if the server is running

	public:
		Server();
		Server(int port);
		Server(const Server &other);
		Server &operator=(const Server &other);
		~Server();

		/* GETERS and SETERS */
		// ip
		std::string	getIp() const;
		void		setIp(const std::string &ip);

		// ports
		std::vector<int>	getPorts() const;
		void				addPort(int port);
		bool				hasPort(int port) const;

		// server_name
		std::string	getServerName() const;
		void		setServerName(const std::string &server_name);

		// sockets
		std::vector<int>	getSockets() const;
		bool				hasSocket(int socket) const;

		// locations
		std::vector<Location>	getLocations() const;
		const Location*			getLocation(std::string route) const;
		void					addLocation(Location location);

		// is_running
		bool	isRunning() const;

		void start();
		void stop();

		/* TOOLS */
		void	print() const;
};