/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danjimen,isainz-r,serferna <webserv@stu    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 09:58:17 by danjimen          #+#    #+#             */
/*   Updated: 2025/06/24 17:52:35 by danjimen,is      ###   ########.fr       */
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
		std::map<std::string, std::string>	_cgi; /* List of possible cgis */
		ReturnData							_return_data;
		std::vector<std::string>			_methods;

		Config( void );
		virtual ~Config( void );
};