/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danjimen,isainz-r,serferna <webserv@stu    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 21:20:04 by danjimen,is       #+#    #+#             */
/*   Updated: 2025/07/01 23:42:43 by danjimen,is      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/Parser.hpp"
#include "../../include/prototypes.hpp"

Parser::Parser(const std::string &configFile) : _server_count(0), _config_file(configFile)
{
 	_parsed_servers.clear();
	tokenizeConfigFile();
}

Parser::~Parser() { _parsed_servers.clear(); }

/* GETERS AND SETERS*/
// server_count
int	Parser::getServerCount() const { return _server_count; }

// parsed_servers
std::vector<Server>	Parser::getParsedServers() const { return _parsed_servers; }

Server	Parser::getParsedServer(const std::string &server_name) const
{
	std::vector<Server>::const_iterator it;
	for (it = _parsed_servers.begin(); it != _parsed_servers.end(); ++it)
	{
		if (it->getServerName() == server_name)
			return *it;
	}
	throw ErrorException("Server not found: " + server_name);
}

void	Parser::setServer(Server server)
{
	_parsed_servers.push_back(server);
}

/* METHODS */
void	Parser::tokenizeConfigFile()
{
	std::ifstream configFile(_config_file.c_str());
	if (!configFile.is_open())
		throw FailOpenFileException(_config_file);
	else
		std::cout << GREEN << _config_file << " opened correctly!" << RESET << std::endl;

	std::string	line;

	// Create vector to save tokens
	std::vector<std::string>	tokens;

	// Read the file line by line
	while (std::getline(configFile, line))
	{
		// Remove isspace()
		line = removeSpaces(line);
		if (line.empty())
			continue;

		// Create an input stream from the line
		std::istringstream ss(line);
		std::string field;

		/* // Create vector to save tokens
		std::vector<std::string>	tokens; */

		// Read each field separated by space
		while (std::getline(ss, field, ' '))
		{
			if (!field.empty())
			{
				//std::cout << "Field: " << field << std::endl; // DB
				tokens.push_back(field);
			}
		}
		/* if (!line.empty()) // DB
			std::cout << "---- End of line ----" << std::endl; //DB */
		
		// Verify tokens
		//check_tokens(tokens);

		// Clear vector
		//tokens.clear();
	}
	std::vector<std::string>::iterator it; // DB
	std::cout << "TOKENS:" << std::endl; // DB
	for (it = tokens.begin(); it != tokens.end(); ++it) // DB
		std::cout << *it << " "; // DB
	std::cout << std::endl; // DB

	parseConfig(tokens);

	configFile.close();
}

/* EXCEPTIONS */
const char* Parser::ErrorException::what() const throw()
{
	return (_message).c_str();
}

const char* Parser::FailOpenFileException::what() const throw()
{
	return _message.c_str();
}