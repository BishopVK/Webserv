/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgis.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danjimen,isainz-r,serferna <webserv@stu    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 11:23:48 by danjimen          #+#    #+#             */
/*   Updated: 2025/07/18 13:26:10 by danjimen,is      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../../include/definitions.hpp"
#include "../../include/webserv.hpp"
#include "../../include/http/HttpResponse.hpp"

class Cgis
{
	private:
		std::string	method;
		std::string	file_path;
		std::string	file_name;
		std::string	content_type;
		std::string	boundary;
		std::string	content_lenght; //en bytes
		std::string	body;

		char			**create_command(std::string file_path, std::string file_name);
		char			**create_env();
		void			hardcode();   //  borrar
		HttpResponse	build_the_response(int cgi_to_server_pipe);
		
		public:
		HttpResponse	execute();
		Cgis( std::string method, std::string file_path, std::string file_name,
			std::string content_type, std::string boundary, std::string content_lenght,
			std::string body );
		~Cgis();
};
