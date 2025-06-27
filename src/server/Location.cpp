/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danjimen,isainz-r,serferna <webserv@stu    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 00:15:57 by danjimen,is       #+#    #+#             */
/*   Updated: 2025/06/27 09:36:39 by danjimen,is      ###   ########.fr       */
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
	std::cout << "route = " << _route << std::endl;

	// alias
	std::cout << "alias = " << _alias << std::endl;

	// root
	std::cout << "root = " << _root << std::endl;

	// index_files
	if (!_index_files.empty())
	{
		std::cout << "index_files:" << std::endl;
		std::vector<std::string>::const_iterator it;
		for (it = _index_files.begin(); it != _index_files.end(); ++it)
			std::cout << "\t- " << *it << std::endl;
	}

	// autoindex
	std::cout << "autoindex = " << (_autoindex ? std::string("true") : std::string("false")) << std::endl;

	// client_max_body_size
	std::cout << "client_max_body_size = " << _client_max_body_size << "M" << std::endl;

	// error_pages
	if (!_error_pages.empty())
	{
		std::cout << "error_pages:" << std::endl;
		std::map<int, std::string>::const_iterator it;
		for (it = _error_pages.begin(); it != _error_pages.end(); ++it)
			std::cout << "\t- " << it->first << " => " << it->second << std::endl;
	}

	// cgi
	if (!_cgi.empty())
	{
		std::cout << "cgi:" << std::endl;
		std::map<std::string, std::string>::const_iterator it;
		for (it = _cgi.begin(); it != _cgi.end(); ++it)
			std::cout << "\t- " << it->first << " => " << it->second << std::endl;
	}

	// return_data
	if (_return_data.code != -1)
	{
		std::cout << "return_data:" << std::endl;
		std::cout << "\t- " << _return_data.code << " => " << _return_data.text << std::endl;
	}

	// methods
	if (!_methods.empty())
	{
		std::cout << "methods:" << std::endl;
		std::vector<std::string>::const_iterator it;
		for (it = _methods.begin(); it != _methods.end(); ++it)
			std::cout << "\t- " << *it << std::endl;
	}
}