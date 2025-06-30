/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_config.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danjimen,isainz-r,serferna <webserv@stu    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 10:51:33 by danjimen,is       #+#    #+#             */
/*   Updated: 2025/06/30 11:55:11 by danjimen,is      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/webserv.hpp"
#include "../../include/Server.hpp"
#include "../../include/definitions.hpp"

void	parse(std::ifstream &configFile)
{
	std::string	line;

	// Read the file line by line
	while (std::getline(configFile, line))
	{
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