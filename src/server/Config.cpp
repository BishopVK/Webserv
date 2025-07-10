/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danjimen,isainz-r,serferna <webserv@stu    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 18:08:18 by danjimen,is       #+#    #+#             */
/*   Updated: 2025/07/10 12:18:17 by danjimen,is      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"

Config::Config() : _root(ROOT_DEFAULT), _autoindex(AUTOINDEX_DEFAULT), _client_max_body_size(CLIENT_MAX_BODY_SIZE_DEFAULT)
{
	// Empty STLs
	_index_files.clear();
	_error_pages.clear();
	_methods.clear();

	// Initialize inherit bools
	_inherit_initizalized.clear();
	_inherit_initizalized.resize(_init_count_elements, false);

	// Init Return struct
	_return_data.code = -1;
	_return_data.text = "";
}

Config::~Config() {}

/* GETERS and SETERS */
// root
std::string	Config::getRoot() const { return _root; }

void	Config::setRoot(const std::string &root)
{
	if (root[0] != '/' || *(root.end() - 1) == '/')
		throw ErrorException("Invalid root path format");
	_root = root;
	_inherit_initizalized.at(_init_root) = true;
}

// index_files
std::vector<std::string>	Config::getIndexFiles() const { return _index_files; }

void	Config::addIndexFile(const std::string &index_file)
{
	if (std::find(_index_files.begin(), _index_files.end(), index_file) == _index_files.end())
	{
		std::vector<std::string>	tokens;
		size_t						start = 0;
		size_t						end;
		
		while ((end = index_file.find('.', start)) != std::string::npos)
		{
			tokens.push_back(index_file.substr(start, end - start));
			start = end + 1;
		}

		// Add last part
		tokens.push_back(index_file.substr(start));
		if (tokens.size() != 2)
			throw ErrorException(index_file + ": invalid index_file format");
			
		for (std::vector<std::string>::iterator it = tokens.begin(); it != tokens.end(); ++it)
		{
			if ((*it).empty())
				throw ErrorException(index_file + ": invalid index_file format");
		}
		
		_index_files.push_back(index_file);
		_inherit_initizalized.at(_init_index_files) = true;
	}
	else
		throw ErrorException(index_file + ": repeated index file");
}

// autoindex
bool	Config::getAutoindex() const { return _autoindex; }

void	Config::setAutoindex(bool autoindex)
{
	_autoindex = autoindex;
	_inherit_initizalized.at(_init_autoindex) = true;
}

// client_max_body_size
size_t	Config::getClientMaxBodySize() const { return _client_max_body_size; }

void	Config::setClientMaxBodySize(size_t client_max_body_size)
{
	_client_max_body_size = client_max_body_size;
	_inherit_initizalized.at(_init_client_max_body_size) = true;
}

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
	_inherit_initizalized.at(_init_error_pages) = true;
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
	if (method != "GET" && method != "POST" && method != "DELETE")
		throw ErrorException(method + ": Method not allowed");

	if (!hasMethod(method))
		_methods.push_back(method);
}

bool	Config::hasMethod(const std::string &Method) const
{
	return std::find(_methods.begin(), _methods.end(), Method) != _methods.end();
}

// inherit_initizalized
std::vector<bool>	Config::getInheritInitialized() const { return _inherit_initizalized; }

bool	Config::getInheritInitialized(int index) const
{
	if (index < 0 || index > static_cast<int>(_inherit_initizalized.size() - 1))
		throw ErrorException("Invalid acces to inherit_initialized vector position");
	return _inherit_initizalized.at(index);
}

/* EXCEPTIONS */
const char* Config::ErrorException::what() const throw()
{
	return (_message).c_str();
}
