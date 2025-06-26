/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danjimen,isainz-r,serferna <webserv@stu    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 00:15:57 by danjimen,is       #+#    #+#             */
/*   Updated: 2025/06/26 12:13:38 by danjimen,is      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/Location.hpp"

Location::Location() : Config(), _route(""), _alias(ALIAS_DEFAULT) {}

Location::Location(const std::string &route) : Config(), _route(route), _alias(ALIAS_DEFAULT) {}

Location::~Location() {}

/* GETERS and SETERS */
// route
std::string	Location::getRoute() const { return _route; }
void	Location::setRoute(const std::string &route) { _route = route; }

// alias
std::string	Location::getAlias() const { return _alias; }
void	Location::setAlias(const std::string &alias) { _alias = alias; }

//void Location::inherit(const Config &config) {} // FALTA POR DEFINIR