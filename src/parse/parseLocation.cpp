/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parseLocation.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danjimen,isainz-r,serferna <webserv@stu    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 02:06:57 by danjimen,is       #+#    #+#             */
/*   Updated: 2025/07/04 13:10:59 by danjimen,is      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
#include "Config.hpp"
#include "Server.hpp"
#include "Parser.hpp"
#include "definitions.hpp"

Location	Parser::parseLocation(const std::vector<std::string> &tokens, size_t &i)
{
	//std::cout << "First token at location: " << tokens[i] << std::endl;
	Location location(tokens[i++]);

	// location.setRoute(tokens[i++]);

	if (tokens[i++] != "{")
		throw ErrorException("Expected '{' after location path");

	// CGIs values
	std::string cgiExtension;
	std::string cgiPath;
	while (tokens[i] != "}")
	{
		const std::string &key = tokens[i++];

		if (key == "limit_except")
		{
			while (tokens[i] != "{")
				location.addMethod(tokens[i++]);
			if (tokens[i++] != "{")
				throw ErrorException("Expected '{' after limit_except");
			while (tokens[i] != "}")
				++i; // skip "deny all"
			++i; // Skip '}' limit_except close
		}
		else
		{
			if (key == "root")
				location.setRoot(tokens[i++]);
			else if (key == "index")
			{
				while (tokens[i] != ";")
					location.addIndexFile(tokens[i++]);
			}
			else if (key == "client_max_body_size")
				location.setClientMaxBodySize(static_cast<size_t>(std::strtoul(tokens[i++].c_str(), NULL, 10)));
			else if (key == "autoindex")
			{
				if (tokens[i] == "on")
					location.setAutoindex(true);
				else if (tokens[i] == "off")
					location.setAutoindex(false);
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
					location.addErrorPage(codes[j], path);
			}
			else if (key == "upload_enable")
			{
				if (tokens[i] == "on")
					location.setUploadEnable(true);
				else if (tokens[i] == "off")
					location.setUploadEnable(false);
				else
					throw ErrorException("Wrong upload_enable value");
				i++;
			}
			else if (key == "upload_store")
				location.setUploadStore(tokens[i++]);
			else if (key == "return")
			{
				int code = std::atoi(tokens[i++].c_str());
				std::string	path = tokens[i++];
				location.setReturnData(code, path);
			}
			else if (key == "cgi_extension" || key == "cgi_pass")
			{
				if (!cgiExtension.empty() && !cgiPath.empty())
					throw ErrorException("Multiple cgis in a single location not allowed");
				if (key == "cgi_extension")
					cgiExtension = tokens[i++];
				else if (key == "cgi_pass")
					cgiPath = tokens[i++];
				if (!cgiExtension.empty() && !cgiPath.empty())
					location.addCgi(cgiExtension, cgiPath);
			}
			else
				throw ErrorException("Unknown location directive: " + key);

			if (tokens[i] != ";")
				throw ErrorException("Expected ';' after directive");
			++i;
		}
	}

	++i; // Skip '}' location close

	return location;
}