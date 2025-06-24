/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danjimen,isainz-r,serferna <webserv@stu    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 17:23:27 by danjimen,is       #+#    #+#             */
/*   Updated: 2025/06/25 00:25:09 by danjimen,is      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Config.hpp"

class Location : public Config
{
	private:
		std::string	_route;	/* Route of the location */
		std::string	_alias;	/* Alias of the route */

	public:
		Location();
		Location(const std::string &route);
		~Location();

		/* GETERS and SETERS */
		// route
		std::string	getRoute() const;
		void		setRoute(const std::string &route);

		// alias
		std::string	getAlias() const;
		void		setAlias(const std::string &alias);

		// Será necesario heredar de la configuración inicial del server
		void inherit(const Config &config);
};
