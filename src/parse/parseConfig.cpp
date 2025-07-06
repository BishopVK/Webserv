/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parseConfig.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danjimen,isainz-r,serferna <webserv@stu    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 10:51:33 by danjimen,is       #+#    #+#             */
/*   Updated: 2025/07/06 09:51:13 by danjimen,is      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
#include "Server.hpp"
#include "Parser.hpp"
#include "definitions.hpp"

std::vector<Server>	Parser::parseConfig(const std::vector<std::string> &tokens)
{
	std::vector<Server>	servers;
	size_t				i = 0;

	while (i < tokens.size())
	{
		if (tokens[i] == "server")
		{
			++i;
			if (tokens[i] != "{")
				throw ErrorException("Expected '{' after 'server'");
			++i;

			Server server = parseServer(tokens, i);
			servers.push_back(server);
		}
		else
			throw ErrorException("Expected 'server' block at top level");
	}

	return servers;
}
