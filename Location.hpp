/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danjimen,isainz-r,serferna <webserv@stu    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 17:23:27 by danjimen,is       #+#    #+#             */
/*   Updated: 2025/06/24 17:36:31 by danjimen,is      ###   ########.fr       */
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
};
