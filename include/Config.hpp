/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danjimen,isainz-r,serferna <webserv@stu    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 09:58:17 by danjimen          #+#    #+#             */
/*   Updated: 2025/06/24 18:54:14 by danjimen,is      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "definitions.hpp"
#include "webserv.hpp"

class  Config
{
	public:
		struct ReturnData
		{
			int			code;
			std::string	text;
		};

	protected:
		std::string							_root;
		std::vector<std::string>			_index_files;
		bool								_autoindex;
		size_t								_client_max_body_size;
		std::map<int, std::string>			_error_pages; /* <status_code, page_path> */
		std::map<std::string, std::string>	_cgi; /* List of possible cgis <extension, path> */
		ReturnData							_return_data;
		std::vector<std::string>			_methods;

		Config();
		virtual ~Config();

		/* GETERS and SETERS */
		// root
		std::string	getRoot() const;
		void		setRoot(const std::string &root);

		// index_files
		std::vector<std::string>	getIndexFiles() const;
		void						addIndexFile(const std::string &index_file);

		// autoindex
		bool	getAutoindex() const;
		void	setAutoindex(bool autoindex);

		// client_max_body_size
		size_t	getClientMaxBodySize() const;
		void	setClientMaxBodySize(size_t client_max_body_size);

		// error_pages
		std::map<int, std::string>	getErrorPages() const;
		std::string					getErrorPage(int status_code) const;
		void						addErrorPage(int status_code, const std::string &path);

		// cgi
		std::map<std::string, std::string>	getCgis() const;
		std::string							getCgi(const std::string &extension) const;
		void								addCgi(const std::string &extension, const std::string &path);

		// return_data
		const ReturnData	&getReturnData() const;
		void		setReturnData(int code, const std::string &text);
		//void		setReturnData(ReturnData data);

		// methods
		std::vector<std::string>	getMethods() const;
		void						addMethod(const std::string &method);
		bool						hasMethod(const std::string &Method) const;
};