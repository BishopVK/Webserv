/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danjimen,isainz-r,serferna <webserv@stu    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 23:41:41 by danjimen,is       #+#    #+#             */
/*   Updated: 2025/07/01 23:41:50 by danjimen,is      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/webserv.hpp"
#include "../../include/Server.hpp"
#include "../../include/Parser.hpp"
#include "../../include/definitions.hpp"

std::string Parser::removeSpaces(const std::string& line)
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
}