/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danjimen,isainz-r,serferna <webserv@stu    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 12:44:17 by danjimen,is       #+#    #+#             */
/*   Updated: 2025/07/02 03:23:10 by danjimen,is      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/webserv.hpp"
#include "include/prototypes.hpp"
#include "include/Parser.hpp"
#include "include/Config.hpp"
#include "include/Server.hpp"
#include "include/valueObjects/Port.hpp"
#include "src/http/HttpServer.hpp"
#include <vector>

Server	create_server_for_testing()
{
	Server server;
	server.setRoot("www.testing.cat");
	server.addIndexFile("index.html index.htm;");
	server.setAutoindex(true);
	server.setClientMaxBodySize(10);
	server.addErrorPage(404, "/errors/404.html");
	server.setIp("127.0.0.2");
	server.addPort(1234);
	server.setServerName("Testing Server in Catalán");

	Location location1;
	location1.setRoute("/var/www/html/location1Route");
	location1.setRoot("/var/www/html/location1Root");
	location1.addIndexFile("location1.html");
	location1.setAutoindex(false);
	location1.addErrorPage(505, "/errors/50x.html");
	location1.setClientMaxBodySize(5);
	location1.addCgi(".php", "/usr/bin/php-cgi");
	location1.setReturnData(301, "http://example.com");
	location1.addMethod("GET");
	location1.addMethod("POST");

	server.addLocation(location1);

	Location location2;
	location2.setRoute("/var/www/html/location2Route");
	location2.setRoot("/var/www/html/location2Root");
	location2.addIndexFile("location2.html");
	location2.setAutoindex(true);
	location2.addErrorPage(400, "/errors/40x.html");
	location2.setClientMaxBodySize(20);
	location2.addCgi(".py", "/usr/bin/python3");
	location2.setReturnData(302, "http://python-example.com");
	location2.addMethod("DELETE");

	server.addLocation(location2);

	Location location3;
	location3.setRoute("/var/www/html/location3Route");
	location3.addCgi(".py", "/usr/bin/python3");
	location3.setReturnData(302, "http://python-example.com");
	location3.addMethod("GET");
	location3.addMethod("POST");
	location3.addMethod("DELETE");
	location3.inherit(server);

	server.addLocation(location3);

	return server;
}

int main(int argc, char** argv)
{
	if (argc != 2)
	{
		std::cerr << "Error: Correct use => " << argv[0] << " + configFile" << std::endl;
		return EXIT_FAILURE;
	}

	std::string configPath = argv[1];

	/* std::ifstream configFile(configPath.c_str());
	if (!configFile.is_open())
	{
		std::cerr << "Error: The configuration file " << configPath << " could not be opened." << std::endl;
		return EXIT_FAILURE;
	} */

	// TODO TENDRÁ QUE IR DENTRO DE UN TRY CATCH
	try
	{
		Parser parser(configPath);
		Server server = create_server_for_testing(); // TESTING
		std::vector<Server> servers;
		servers.push_back(server);
		server.print();
		HttpServer httpServer(servers);
		httpServer.run();
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	
	//parse(configFile);

	/* std::stringstream buffer;
	buffer << configFile.rdbuf(); // lee todo el contenido
	std::string configContent = buffer.str(); */

	/* std::cout << "Contenido del archivo de configuración:" <<`std::endl;
	std::cout << "----------------------------------------" << std::endl;
	std::cout << configContent << std::endl;
	std::cout << "---------------------------------------- " << std::endl; */

	// Aquí más adelante vendrá el parser

	//int port = 8080; // lo obtendremos del parser cuando lo hagamos
	//Server server(port);
	// // Server server = create_server_for_testing(); // TESTING
	// // std::vector<Server> servers;
	// // servers.push_back(server);

	// // server.print();
	// server.start();
	// while (true) // Mantener el servidor corriendo
	// 	;1
	//

	// // HttpServer httpServer(servers);

	// // httpServer.run();

	// Hacer "telnet localhost 8080" en otra terminal para comprobar que el servidor está escuchando en el puerto 8080

	return EXIT_SUCCESS;
}
