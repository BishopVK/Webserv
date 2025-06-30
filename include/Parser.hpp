/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danjimen,isainz-r,serferna <webserv@stu    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 20:53:50 by danjimen,is       #+#    #+#             */
/*   Updated: 2025/06/30 23:31:07 by danjimen,is      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "definitions.hpp"
#include "Server.hpp"

class Parser
{
	private:
		int					_server_count;
		std::vector<Server>	_parsed_servers;
		std::string			_config_file;
	
		Parser();
		Parser(const Parser &other);
		Parser &operator=(const Parser &other);

	public:
		Parser(const std::string &configFile);
		~Parser();

		/* GETERS AND SETERS*/
		// server_count
		int	getServerCount() const;

		// parsed_servers
		std::vector<Server>	getParsedServers() const;
		Server				getParsedServer(const std::string &server_name) const;
		void				setServer(Server server);

		/* METHODS */
		void	parseFile();

		/* EXCEPTIONS */
		class ErrorException : public std::exception
		{
			private:
				std::string _message;
			public:
				ErrorException() : _message(RED "Error" RESET) {}
				ErrorException(const std::string &message) : _message(RED "Error: " + message + RESET) {}
				virtual ~ErrorException() throw() {}
				virtual const char* what() const throw();
		};

		class FailOpenFileException : public std::exception
		{
			private:
				std::string _filename;
				std::string	_message;

			public:
				FailOpenFileException(const std::string &file) : _filename(file) {
					_message = (RED "Error: Failed to open file: " + this->_filename + RESET);
				}
				virtual ~FailOpenFileException() throw() {}
				virtual const char* what() const throw();
		};
};