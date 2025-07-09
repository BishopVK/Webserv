/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgis.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danjimen,isainz-r,serferna <webserv@stu    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 11:23:48 by danjimen          #+#    #+#             */
/*   Updated: 2025/07/09 11:24:15 by danjimen,is      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "definitions.hpp"
#include "webserv.hpp"

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
		int			chunked = false;
	char **create_command(std::string file_path, std::string file_name);
	char	**create_env();

public:
	void	hardcode();
	void execute();
	//std::string deschunk( std::string body );
	Cgis( std::string method, std::string file_path, std::string file_name,
		std::string content_type, std::string boundary, std::string content_lenght,
		std::string body, int chunked );
	~Cgis();
};

void Cgis::hardcode()
{
	this->method = "POST";
	this->file_path = "/home/isainz-r/Webserv/cgis/";
	this->file_name = "a_cgi.php";
	//std::string	file_name = "file.php";
	this->content_type = "application/x-www-form-urlencoded";
	//std::string	content_type = "multipart/form-data";
	this->content_lenght = "10";
	this->body = "age=20";
	//std::string	body = "--XYZ\r\n"
	//					"Content-Disposition: form-data; name=\"file\"; filename=\"test.txt\"\r\n"
	//					"Content-Type: text/plain\r\n"
	//					"\r\n"
	//					"Hello from the file!\r\n"
	//					"--XYZ--\r\n";
	this->boundary = "";
	this->chunked = false;

}

Cgis::Cgis( std::string method, std::string file_path, std::string file_name,
		std::string content_type, std::string boundary, std::string content_lenght,
		std::string body, int chunked )
{
	hardcode();
	// this->method = method;
	// this->file_path = file_path;
	// this->file_name = file_name;
	// this->content_type = content_type;
	// this->boundary = boundary;
	// this->content_lenght = content_lenght;
	// this->body = body;
	// if (chunked == true)
	// {
	// 	this->body = deschunk(body);
	// }

}

Cgis::~Cgis()
{
}
