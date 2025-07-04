/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parseConfig.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danjimen,isainz-r,serferna <webserv@stu    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 10:51:33 by danjimen,is       #+#    #+#             */
/*   Updated: 2025/07/04 13:10:59 by danjimen,is      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
#include "Server.hpp"
#include "Parser.hpp"
#include "definitions.hpp"

/* std::string Parser::removeSpaces(const std::string& line)
{
	if (line.empty())
		return ("");
	// Remove isspace() at start and end
	std::string result; // PROTEGER QUE PASA SI UN STRING FALLA AL RESERVAR MEMORIA
	std::string::const_iterator line_begin = line.begin();
	std::string::const_iterator line_end = line.end();
	while (line_begin != line_end && std::isspace(static_cast<unsigned char>(*line_begin)))
		line_begin++;
	if (line_begin != line_end)
	{
		line_end--;
		while (line_end != line_begin && std::isspace(static_cast<unsigned char>(*line_end)))
			line_end--;
	}
	else
		return ("");
	
	result.assign(line_begin, line_end + 1);

	int	spaces = 0;
	std::string final_result;
	std::string::iterator result_begin = result.begin();
	std::string::iterator result_end = result.end();
	// Ignore comments
	if (*result_begin == '#')
		return ("");
	// Remove isspace() between tokens leaving only one space
	for (result_begin = result.begin(); result_begin != result_end; ++result_begin)
	{
		if (std::isspace(static_cast<unsigned char>(*result_begin)))
		{
			if (spaces < 1)
				final_result += ' ';
			spaces++;
		}
		else
		{
			spaces = 0;
			if ((*result_begin == ';' || *result_begin == '{' || *result_begin == '}') && !std::isspace(static_cast<unsigned char>(*result_begin - 1)))
				final_result += ' ';
			final_result += *result_begin;
		}
	}
	
	return final_result;
} */

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

// DELETE METHOD -- NOT NECESARY
/* void		Parser::check_tokens(std::vector<std::string> tokens)
{
	size_t	vector_size = tokens.size();

	// Only 1 token
	if (vector_size == 1)
	{
		// if (tokens[0].length() != 1)
		// 	throw Parser::ErrorException("Wrong line '" + tokens[0] + "'");
		if (tokens[0].length() == 1 && (tokens[0][0] != '{' && tokens[0][0] != '}'))
			throw Parser::ErrorException("Wrong line '" + tokens[0] + "'");
	}

	// Create Server
	// if ((vector_size == 1 && tokens[0] == "server") || (vector_size == 2 && tokens[0] == "server" && tokens[1] == "{"))
	// {
	// 	Server server;
	// 	_server_count++;
	// }
} */