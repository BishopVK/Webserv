/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parseServer.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danjimen,isainz-r,serferna <webserv@stu    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 23:44:02 by danjimen,is       #+#    #+#             */
/*   Updated: 2025/07/04 13:10:59 by danjimen,is      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
#include "Server.hpp"
#include "Parser.hpp"
#include "definitions.hpp"

Server	Parser::parseServer(const std::vector<std::string> &tokens, size_t &i)
{
	Server	server;

	while (i < tokens.size() && tokens[i] != "}")
	{
		if (tokens[i] == "location")
		{
			Location location = parseLocation(tokens, ++i); // Need Define parseLocation() and add copy assignment constructor to Location
			location.inherit(server);
			server.addLocation(location);
		}
		else
			parseDirective(server, tokens, i);
	}

	if (i == tokens.size() || tokens[i] != "}")
		throw ErrorException("Expected closing '}' for server block");
	
	++i; // Skip '}'

	return server; // ??
}