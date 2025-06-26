/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danjimen,isainz-r,serferna <webserv@stu    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 00:15:57 by danjimen,is       #+#    #+#             */
/*   Updated: 2025/06/27 00:56:36 by danjimen,is      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/Location.hpp"

Location::Location() : Config(), _route(""), _alias(ALIAS_DEFAULT) {}

Location::Location(const std::string &route) : Config(), _route(route), _alias(ALIAS_DEFAULT) {}

Location::~Location() {}

/* GETERS and SETERS */
// route
std::string	Location::getRoute() const { return _route; }
void	Location::setRoute(const std::string &route) { _route = route; }

// alias
std::string	Location::getAlias() const { return _alias; }
void	Location::setAlias(const std::string &alias) { _alias = alias; }

//void Location::inherit(const Config &config) {} // FALTA POR DEFINIR

/* TOOLS */
void	Location::print() const
{
	// route
	std::cout << "route = " << getRoute() << std::endl;

	// alias
	std::cout << "alias = " << getAlias() << std::endl;

	// root
	std::cout << "root = " << getRoot() << std::endl;

	// index_files
	if (!getIndexFiles().empty())
	{
		std::cout << "index_files:" << std::endl;
		std::vector<std::string>::iterator it;
		for (it = getIndexFiles().begin(); it != getIndexFiles().end(); ++it)
			std::cout << "\t- " << *it << std::endl;
	}

	// autoindex
	std::cout << "autoindex = " << (getAutoindex() ? std::string("true") : std::string("false")) << std::endl;

	// client_max_body_size
	std::cout << "client_max_body_size = " << getClientMaxBodySize() << std::endl;

	// error_pages
	if (!getErrorPages().empty())
	{
		std::cout << "error_pages:" << std::endl;
		std::map<int, std::string>::iterator it;
		for (it = getErrorPages().begin(); it != getErrorPages().end(); ++it)
			std::cout << "\t- " << it->first << " => " << it->second << std::endl;
	}

	// cgi
	if (!getCgis().empty())
	{
		std::cout << "cgi:" << std::endl;
		std::map<std::string, std::string>::iterator it;
		for (it = getCgis().begin(); it != getCgis().end(); ++it)
			std::cout << "\t- " << it->first << " => " << it->second << std::endl;
	}

	// return_data
	if (_return_data.code != -1)
	{
		std::cout << "return_data:" << std::endl;
		std::cout << "\t- " << getReturnData().code << " => " << getReturnData().text << std::endl;
	}

	// methods
	if (!getMethods().empty())
	{
		std::cout << "methods:" << std::endl;
		std::vector<std::string>::iterator it;
		for (it = getMethods().begin(); it != getMethods().end(); ++it)
			std::cout << "\t- " << *it << std::endl;
	}
}