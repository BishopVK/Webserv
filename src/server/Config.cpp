/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danjimen,isainz-r,serferna <webserv@stu    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 18:08:18 by danjimen,is       #+#    #+#             */
/*   Updated: 2025/06/26 12:13:18 by danjimen,is      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/Config.hpp"

Config::Config() : _root(ROOT_DEFAULT), _autoindex(AUTOINDEX_DEFAULT), _client_max_body_size(CLIENT_MAX_BODY_SIZE_DEFAULT)
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
std::vector<std::string>	Config::getIndexFiles() const { return _index_files; }

void	Config::addIndexFile(const std::string &index_file)
{
	if (std::find(_index_files.begin(), _index_files.end(), index_file) == _index_files.end())
		_index_files.push_back(index_file);
}

// autoindex
bool	Config::getAutoindex() const { return _autoindex; }

void	Config::setAutoindex(bool autoindex) { _autoindex = autoindex; }

// client_max_body_size
size_t	Config::getClientMaxBodySize() const { return _client_max_body_size; }

void	Config::setClientMaxBodySize(size_t client_max_body_size) { _client_max_body_size = client_max_body_size; }

// error_pages
std::map<int, std::string>	Config::getErrorPages() const { return _error_pages; }

std::string	Config::getErrorPage(int status_code) const
{
	std::map<int, std::string>::const_iterator it = _error_pages.find(status_code);
	if (it != _error_pages.end())
		return it->second;
	return "";
}

void	Config::addErrorPage(int status_code, const std::string &path)
{
	std::map<int, std::string>::iterator it = _error_pages.find(status_code);

	if (it == _error_pages.end())
		_error_pages.insert(std::pair<int, std::string>(status_code, path));
	else
		it->second = path;
}

// cgi
std::map<std::string, std::string>	Config::getCgis() const { return _cgi; }

std::string	Config::getCgi(const std::string &extension) const
{
	std::map<std::string, std::string>::const_iterator it = _cgi.find(extension);
	if (it != _cgi.end())
		return it->second;
	return "";
}

void	Config::addCgi(const std::string &extension, const std::string &path)
{
	std::map<std::string, std::string>::iterator it = _cgi.find(extension);

	if (it == _cgi.end())
		_cgi.insert(std::pair<std::string, std::string>(extension, path));
	else
		it->second = path;
}

// return_data
const Config::ReturnData	&Config::getReturnData() const { return _return_data; }

void	Config::setReturnData(int code, const std::string &text)
{
	_return_data.code = code;
	_return_data.text = text;
}

void	Config::setReturnData(ReturnData data)
{
	_return_data.code = data.code;
	_return_data.text = data.text;
}

// methods
std::vector<std::string>	Config::getMethods() const { return _methods; }

void	Config::addMethod(const std::string &method)
{
	if (!hasMethod(method))
		_methods.push_back(method);
}

bool	Config::hasMethod(const std::string &Method) const
{
	return std::find(_methods.begin(), _methods.end(), Method) != _methods.end();
}
