/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danjimen,isainz-r,serferna <webserv@stu    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 17:23:27 by danjimen,is       #+#    #+#             */
/*   Updated: 2025/07/10 12:21:31 by danjimen,is      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Config.hpp"

class Location : public Config
{
	private:
		std::string	_route;	/* Route of the location */
		bool		_upload_enable;
		std::string	_upload_store;
		std::string	_cgi_extension;
		std::string	_cgi_path;

	public:
		Location();
		Location(const std::string &route);
		Location &operator=(const Location &other);
		~Location();

		/* GETERS and SETERS */
		// route
		std::string	getRoute() const;
		void		setRoute(const std::string &route);

		// upload_enable
		bool	getUploadEnable() const;
		void	setUploadEnable(bool value);

		// upload_store
		std::string	getUploadStore() const;
		void		setUploadStore(const std::string &path);

		// cgi_extension
		std::string		getCgiExtension() const;
		void			setCgiExtension(const std::string &extension);

		// cgi_path
		std::string		getCgiPath() const;
		void			setCgiPath(const std::string &path);

		// Será necesario heredar de la configuración inicial del server
		void inherit(const Config &config);

		/* TOOLS */
		void	print() const;
};
