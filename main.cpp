/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danjimen,isainz-r,serferna <webserv@stu    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 12:44:17 by danjimen,is       #+#    #+#             */
/*   Updated: 2025/06/26 14:01:48 by danjimen,is      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/Config.hpp"
#include "include/Server.hpp"

int main(int argc, char** argv)
{
	if (argc != 2)
	{
		std::cerr << "Error: Correct use => " << argv[0] << " + configFile" << std::endl;
		return EXIT_FAILURE;
	}

	std::string configPath = argv[1];

	std::ifstream configFile(configPath.c_str());
	if (!configFile.is_open())
	{
		std::cerr << "Error: The configuration file " << configPath << " could not be opened." << std::endl;
		return EXIT_FAILURE;
	}

	std::stringstream buffer;
	buffer << configFile.rdbuf(); // lee todo el contenido
	std::string configContent = buffer.str();

	std::cout << "Contenido del archivo de configuración:" << std::endl;
	std::cout << "----------------------------------------" << std::endl;
	std::cout << configContent << std::endl;
	std::cout << "---------------------------------------- " << std::endl;

	// Aquí más adelante vendrá el parser

	int port = 8080; // lo obtendremos del parser cuando lo hagamos
	Server server(port);
	server.start();
	while (true) // Mantener el servidor corriendo
		;

	// Hacer "telnet localhost 8080" en otra terminal para comprobar que el servidor está escuchando en el puerto 8080

	return EXIT_SUCCESS;
}