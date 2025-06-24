/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danjimen,isainz-r,serferna <webserv@stu    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 18:08:18 by danjimen,is       #+#    #+#             */
/*   Updated: 2025/06/24 19:00:22 by danjimen,is      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/Config.hpp"

Config::Config() : _root(ROOT_DEFAULT), _client_max_body_size(CLIENT_MAX_BODY_SIZE_DEFAULT), _autoindex(AUTOINDEX_DEFAULT)
{
	// Empty STLs
	_error_pages.clear();
	_cgi.clear();
	_methods.clear();

	// Init Return struct
	_return_data.code = -1;
	_return_data.text = "";

	// Append the GET method
	_methods.push_back("GET");
}

Config::~Config() {}

/* GETERS and SETERS */
// root
std::string	Config::getRoot() const { return _root; }

void	Config::setRoot(const std::string &root) { _root = root; }

// index_files
std::vector<std::string>	Config::getIndexFiles() const
{

}

void	Config::addIndexFile(const std::string &index_file)
{

}

// autoindex
bool	Config::getAutoindex() const
{

}

void	Config::setAutoindex(bool autoindex)
{

}

// client_max_body_size
size_t	Config::getClientMaxBodySize() const
{

}

void	Config::setClientMaxBodySize(size_t client_max_body_size)
{

}

// error_pages
std::map<int, std::string>	Config::getErrorPages() const
{

}

std::string	Config::getErrorPage(int status_code) const
{

}

void	Config::addErrorPage(int status_code, const std::string &path)
{

}

// cgi
std::map<std::string, std::string>	Config::getCgis() const
{

}

std::string	Config::getCgi(const std::string &extension) const
{

}

void	Config::addCgi(const std::string &extension, const std::string &path)
{

}

// return_data
const Config::ReturnData	&Config::getReturnData() const
{

}

void	Config::setReturnData(int code, const std::string &text)
{

}

//void	Config::setReturnData(ReturnData data) {}

// methods
std::vector<std::string>	Config::getMethods() const
{

}

void	Config::addMethod(const std::string &method)
{

}

bool	Config::hasMethod(const std::string &Method) const
{

}
