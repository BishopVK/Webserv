/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   definitions.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danjimen,isainz-r,serferna <webserv@stu    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 10:00:39 by danjimen          #+#    #+#             */
/*   Updated: 2025/06/24 18:12:21 by danjimen,is      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# define ROOT_DEFAULT					"."
# define CLIENT_MAX_BODY_SIZE_DEFAULT	100 /* Default max size of a HTTP request -> 100 bytes/characters */
# define SERVERS_BACKLOG				10 /* Max. number of connections to a server simultaneously */
# define AUTOINDEX_DEFAULT				false
# define IP_DEFAULT						"127.0.0.1"
# define MAX_BODY_SIZE_BYTES			52428800 /* 50MB */
# define SERVER_PROTOCOL				"HTML/1.1"
# define ALIAS_DEFAULT					""