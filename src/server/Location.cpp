/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danjimen,isainz-r,serferna <webserv@stu    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 00:15:57 by danjimen,is       #+#    #+#             */
/*   Updated: 2025/07/10 12:21:54 by danjimen,is      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"

//Location::Location() : Config(), _route(""), _alias(ALIAS_DEFAULT) {}
Location::Location() : Config(), _route(""), _upload_enable(false), _upload_store(""), _cgi_extension(""), _cgi_path("") {}

//Location::Location(const std::string &route) : Config(), _route(route), _alias(ALIAS_DEFAULT) {}
Location::Location(const std::string &route) : Config(), _route(route), _upload_enable(false), _upload_store(""), _cgi_extension(""), _cgi_path("") {}

Location &Location::operator=(const Location &other)
{
	if (this != &other)
	{
		_root = other.getRoot();
		_index_files = other.getIndexFiles();
		_client_max_body_size = other.getClientMaxBodySize();
		_autoindex = other.getAutoindex();
		_error_pages = other.getErrorPages();
		_upload_enable = other.getUploadEnable();
		_upload_store = other.getUploadStore();
		_return_data = other.getReturnData();
		_methods = other.getMethods();
		_cgi_extension = other.getCgiExtension();
		_cgi_path = other.getCgiPath();
	}
	return (*this);
}

Location::~Location() {}

/* GETERS and SETERS */
// route
std::string	Location::getRoute() const { return _route; }
void	Location::setRoute(const std::string &route)
{
	if (route[0] != '/')
		throw ErrorException(route + ": Invalidad location route format");
	_route = route;
}

// upload_enable
bool	Location::getUploadEnable() const { return _upload_enable; }

void	Location::setUploadEnable(bool value) { _upload_enable = value; }

// upload_store
std::string	Location::getUploadStore() const { return _upload_store; }

void	Location::setUploadStore(const std::string &path)
{
	if (path[0] != '/')
		throw ErrorException(path + ": Invalidad upload store path format");
	_upload_store = path;
}

// cgi_extension
std::string		Location::getCgiExtension() const { return _cgi_extension; }

void			Location::setCgiExtension(const std::string &extension)
{
	if (extension.empty())
		throw ErrorException("Empty cgi extension not allowed");
	if (extension[0] != '.')
		throw ErrorException(extension + ": Cgi extension must begin whit '.'");
	if (extension != ".php")
		throw ErrorException(extension + ": Only '.php' extension allowed");
	_cgi_extension = extension;
}

// cgi_path
std::string		Location::getCgiPath() const { return _cgi_path; }

void			Location::setCgiPath(const std::string &path)
{
	if (path.empty())
		throw ErrorException("Empty cgi path not allowed");
	if (path[0] != '/')
		throw ErrorException(path + ": CGI path must begin whit '/'");
	if (path != PATH_INFO)
		throw ErrorException(path + ": Only correct php path allowed");
	_cgi_path = path;
}

// FALTA POR DEFINIR
void Location::inherit(const Config &config)
{
	// root
	if (_inherit_initizalized.at(_init_root) == false)
	{
		_root = config.getRoot();
		_inherit_initizalized.at(_init_root) = true;
	}

	// index_files
	if (_inherit_initizalized.at(_init_index_files) == false)
	{
		_index_files = config.getIndexFiles();
		_inherit_initizalized.at(_init_index_files) = true;
	}

	// autoindex
	if (_inherit_initizalized.at(_init_autoindex) == false)
	{
		_autoindex = config.getAutoindex();
		_inherit_initizalized.at(_init_autoindex) = true;
	}

	// error_pages
	if (_inherit_initizalized.at(_init_error_pages) == false)
	{
		_error_pages = config.getErrorPages();
		_inherit_initizalized.at(_init_error_pages) = true;
	}
	
	// client_max_body_size
	if (_inherit_initizalized.at(_init_client_max_body_size) == false)
	{
		_client_max_body_size = config.getClientMaxBodySize();
		_inherit_initizalized.at(_init_client_max_body_size) = true;
	}

	// methods
	if (_methods.empty())
	{
		addMethod("GET");
		addMethod("POST");
		addMethod("DELETE");
	}
}

/* TOOLS */
void	Location::print() const
{
	// route
	std::cout << "route = " << _route << std::endl;

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
	std::cout << "autoindex = " << (_autoindex ? std::string("on") : std::string("off")) << std::endl;

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

	// cgi_extension
	if (!_cgi_extension.empty())
		std::cout << "cgi extension: " << _cgi_extension << std::endl;
	
	// cgi_path
	if (!_cgi_path.empty())
		std::cout << "cgi path: " << _cgi_path << std::endl;

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

	// upload_enable
	if (_upload_enable)
		std::cout << "upload_enable = " << (_upload_enable ? std::string("on") : std::string("off")) << std::endl;

	// upload_store
	if (!_upload_store.empty())
		std::cout << "upload_store = " << _upload_store << std::endl;
}