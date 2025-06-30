/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_config.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danjimen,isainz-r,serferna <webserv@stu    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 10:51:33 by danjimen,is       #+#    #+#             */
/*   Updated: 2025/06/30 13:36:55 by danjimen,is      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/webserv.hpp"
#include "../../include/Server.hpp"
#include "../../include/definitions.hpp"

std::string removeSpaces(const std::string& line)
{
	if (line.empty())
		return ("");
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
	std::string::iterator result_begin;;
	std::string::iterator result_end = result.end();
	for (result_begin = result.begin(); result_begin != result_end; ++result_begin)
	{
		if (std::isspace(static_cast<unsigned char>(*result_begin)))
		{
			spaces++;
			if (spaces <= 1)
				final_result += ' ';
		}
		else
		{
			final_result += *result_begin;
			spaces = 0;
		}
	}

	return final_result;
}

//std::vector<Server>	parse(std::ifstream &configFile) // Tendrá que devolver un vector de Servers
void	parse(std::ifstream &configFile)
{
	std::string	line;

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

		// Read each field separated by space
		while (std::getline(ss, field, ' '))
		{
			if (!field.empty())
				std::cout << "Field: " << field << std::endl;
		}
		if (!line.empty())
			std::cout << "---- End of line ----" << std::endl;
	}
	configFile.close();
}