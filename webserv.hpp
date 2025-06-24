/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danjimen,isainz-r,serferna <webserv@stu    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 17:52:11 by danjimen,is       #+#    #+#             */
/*   Updated: 2025/06/24 17:52:21 by danjimen,is      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <vector>
#include <map>
#include <cstring>      // memset
#include <cstdlib>      // exit
#include <unistd.h>     // close
#include <netinet/in.h> // sockaddr_in, htons
#include <sys/socket.h> // socket, setsockopt, bind, listen
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>