/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parseDirective.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danjimen,isainz-r,serferna <webserv@stu    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 00:56:27 by danjimen,is       #+#    #+#             */
/*   Updated: 2025/07/06 20:58:19 by danjimen,is      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
#include "Server.hpp"
#include "Parser.hpp"
#include "definitions.hpp"

void	Parser::parseDirective(Server &server, const std::vector<std::string> &tokens, size_t &i)
{
	const std::string &key = tokens[i++];

	if (key == "listen")
	{
		while (tokens[i] != ";")
		{
			size_t pos = tokens[i].find(':');

			if (pos != std::string::npos) // ':' was found
			{
				std::string ip = tokens[i].substr(0, pos);
				std::string port = tokens[i].substr(pos + 1);

				if (ip.empty())
					throw ErrorException(tokens[i] + ": invalid ip format");
				if (port.empty())
					throw ErrorException(tokens[i] + ": invalid port");
				for (size_t j = 0; j < port.length(); ++j)
				{
					if (!std::isdigit(port[j]))
						throw ErrorException(tokens[i] + ": invalid port");
				}
				server.setIp(ip);
				server.addPort(std::atoi(port.c_str()));
				server.addListenSet(ip, std::atoi(port.c_str()));
			}
			else
			{
				for (size_t j = 0; j < tokens[i].length(); ++j)
				{
					if (!std::isdigit(tokens[i][j]))
						throw ErrorException(tokens[i] + ": invalid port");
				}
				server.addPort(std::atoi(tokens[i].c_str()));
				server.addListenPort(std::atoi(tokens[i].c_str()));
			}
			i++;
		}
	}
	else if (key == "server_name")
		server.setServerName(tokens[i++]);
	else if (key == "root")
		server.setRoot(tokens[i++]);
	else if (key == "index")
	{
		while (tokens[i] != ";")
			server.addIndexFile(tokens[i++]);
	}
	else if (key == "client_max_body_size")
		server.setClientMaxBodySize(static_cast<size_t>(std::strtoul(tokens[i++].c_str(), NULL, 10)));
	else if (key == "autoindex")
	{
		if (tokens[i] == "on")
			server.setAutoindex(true);
		else if (tokens[i] == "off")
			server.setAutoindex(false);
		else
			throw ErrorException("Wrong Autoindex " + tokens[i] + " value");
		i++;
	}
	else if (key == "error_page")
	{
		std::vector<int> codes;

		while (i < tokens.size() && tokens[i][0] != '/' && tokens[i] != ";")
		{
			// Only digits avalible and positive numbers.
			for (size_t j = 0; j < tokens[i].length(); ++j)
			{
				if (!std::isdigit(tokens[i][j]))
					throw ErrorException(tokens[i] + ": invalid error_pages code");
			}
			codes.push_back(std::atoi(tokens[i++].c_str()));
		}

		if (i >= tokens.size() || tokens[i] == ";")
			throw ErrorException("Expected path after error_page codes");
		std::string path = tokens[i++];

		/* if (i >= tokens.size() || tokens[i] != ";")
			throw ErrorException("Expected ';' after error_page path");
		++i; */ // NECESSARY ????

		for (size_t j = 0; j < codes.size(); ++j)
			server.addErrorPage(codes[j], path);
	}
	else
		throw ErrorException("Unknown directive: " + key);
	
	if (tokens[i] != ";")
		throw ErrorException("Expected ';' after directive " + key);

	++i;
}