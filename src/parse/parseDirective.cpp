/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parseDirective.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danjimen,isainz-r,serferna <webserv@stu    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 00:56:27 by danjimen,is       #+#    #+#             */
/*   Updated: 2025/07/02 02:06:03 by danjimen,is      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/webserv.hpp"
#include "../../include/Server.hpp"
#include "../../include/Parser.hpp"
#include "../../include/definitions.hpp"

void	Parser::parseDirective(Server &server, const std::vector<std::string> &tokens, size_t &i)
{
	const std::string &key = tokens[i++];

	if (key == "listen")
	{
		size_t pos = tokens[i].find(':');

		if (pos != std::string::npos) // ':' was found
		{
			server.setIp(tokens[i].substr(0, pos));
			std::string port = tokens[i].substr(pos + 1);
			server.addPort(std::atoi(port.c_str()));
			i++;
		}
		else
		{
			while (tokens[i] != ";")
				server.addPort(std::atoi(tokens[i++].c_str()));
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
			throw ErrorException("Wrong Autoindex value");
		i++;
	}
	else if (key == "error_page")
	{
		std::vector<int> codes;

		while (i < tokens.size() && tokens[i][0] != '/' && tokens[i] != ";")
			codes.push_back(std::atoi(tokens[i++].c_str()));

		if (i >= tokens.size() || tokens[i] == ";")
			throw ErrorException("Expected path after error_page codes");
		std::string path = tokens[i++];

		if (i >= tokens.size() || tokens[i] == ";")
			throw ErrorException("Expected ';' after error_page path");
		++i;

		for (size_t j = 0; j < codes.size(); ++j)
			server.addErrorPage(codes[j], path);
	}
	else
		throw ErrorException("Unknown directive: " + key);
	
	if (tokens[i] != ";")
		throw ErrorException("Expected ';' after directive");

	++i;
}