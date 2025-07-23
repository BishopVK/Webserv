#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include "Cgis.hpp"
#include "../../include/http/HttpResponse.hpp"
#include "Logger.hpp"
#include "ErrorPageGenerator.hpp"

char **Cgis::create_command(std::string file_path, std::string file_name)
{
	std::vector<std::string> command_vec;
	command_vec.push_back(PATH_INFO);
	command_vec.push_back("-c");
	command_vec.push_back("./config/php.ini");
	//command_vec.push_back("-d");
	//command_vec.push_back("upload_max_filesize=20M");
	//command_vec.push_back("-f");
	command_vec.push_back(file_path + file_name);

	char	**command = new char*[command_vec.size() + 1];
	for (size_t i = 0; i < command_vec.size(); i++)
	{
		command[i] = strdup(command_vec[i].c_str());
	}
	command[command_vec.size()] = NULL;
	return (command);
}

char	**Cgis::create_env()
{
	std::vector<std::string> env_vec;
	env_vec.push_back("GATEWAY_INTERFACE=CGI/1.1");
	env_vec.push_back("REQUEST_METHOD=" + this->method);
	env_vec.push_back("PATH_INFO=" + this->file_path + this->file_name);
	env_vec.push_back("SCRIPT_FILENAME=" + this->file_path + this->file_name);
	env_vec.push_back("SCRIPT_NAME=" + this->file_name);
	if (this->method == "GET")
		env_vec.push_back("QUERY_STRING=" + this->body);
	env_vec.push_back("SERVER_PROTOCOL=HTTP/1.1");
	if (this->method == "POST")
	{
		std::string type = "CONTENT_TYPE=" + this->content_type;
		if (!(this->boundary.empty()))
			type  += "; boundary=" + this->boundary;
		env_vec.push_back(type);
	}
	env_vec.push_back("CONTENT_LENGTH=" + this->content_lenght);
	env_vec.push_back("REDIRECT_STATUS=200");

	char	**env = new char*[env_vec.size() + 1];
	for (size_t i = 0; i < env_vec.size(); i++)
	{
		env[i] = strdup(env_vec[i].c_str());
	}
	env[env_vec.size()] = NULL;
	return (env);
}

// Versión corregida por CHATGPT
/* HttpResponse Cgis::build_the_response(int cgi_to_server_pipe)
{
	HttpResponse response;
	std::string whole_answer;
	char buffer[4096];
	int n;

	// Leer toda la salida del CGI
	while ((n = read(cgi_to_server_pipe, buffer, sizeof(buffer))) > 0)
		whole_answer.append(buffer, n);

	close(cgi_to_server_pipe);

	// Separar headers del cuerpo usando "\r\n\r\n"
	size_t header_end = whole_answer.find("\r\n\r\n");
	if (header_end == std::string::npos)
		return HttpResponse::internalServerError("CGI response malformed (no header-body delimiter)");

	std::string header_part = whole_answer.substr(0, header_end);
	std::string body_part = whole_answer.substr(header_end + 4); // saltar \r\n\r\n

	// Parsear cabeceras línea por línea
	std::istringstream header_stream(header_part);
	std::string line;
	while (std::getline(header_stream, line)) {
		// Saltar líneas vacías
		if (line.empty() || line == "\r")
			continue;

		size_t colon = line.find(':');
		if (colon == std::string::npos)
			continue;

		std::string key = line.substr(0, colon);
		std::string value = line.substr(colon + 1);

		// Quitar espacios iniciales en value
		while (!value.empty() && (value[0] == ' ' || value[0] == '\t'))
			value.erase(0, 1);
		// Quitar saltos finales
		while (!value.empty() && (value[value.size() - 1] == '\r' || value[value.size() - 1] == '\n'))
			value.erase(value.size() - 1, 1);

		response.setHeader(key, value);
	}

	return response.ok(body_part);
} */

HttpResponse	Cgis::build_the_response(int cgi_to_server_pipe)
{
	HttpResponse response;
	std::string whole_answer = "";
	std::string body_answer = "";
	std::string header_key_answer = "";
	std::string header_value_answer = "";
	char buffer[4096];
	int n = 0;
	while ((n = read(cgi_to_server_pipe, buffer, sizeof(buffer))) > 0)
	{
		whole_answer.append(buffer, n);
	}
	//std::cout << "whole answer\n" << whole_answer << "\n\n" << std::endl;

	std::istringstream ss(whole_answer);
	std::string line;
	while (std::getline(ss, line))
	{
		if (line == "\r")
			break ;
		size_t middle = line.find(':');
		header_key_answer = line.substr(0, middle);
		header_value_answer = line.substr(middle + 2, line.find(';') - (middle + 1));
		//std::cout << "key=" << header_key_answer << std::endl;
		//std::cout << "value=" << header_value_answer << std::endl;
		response.setHeader(header_key_answer, header_value_answer);
	}
	while (std::getline(ss, line))
	{
		//std::cout << "line=" << line << std::endl;
		body_answer += line;
		if (!ss.eof())
			body_answer += "\n";
	}
	//std::cout << header_key_answer << std::endl;
	//std::cout << "body_answer=" << body_answer << std::endl;
	close(cgi_to_server_pipe);
	response.setBody(body_answer);
	return (response);
}

HttpResponse Cgis::execute()
{
	pid_t			num_fork;
	int				server_to_cgi_pipe[2];
	int				cgi_to_server_pipe[2];
	char			**env;
	char			**command;
	HttpResponse	response;
	int				status;

	if (pipe(server_to_cgi_pipe) == -1 || pipe(cgi_to_server_pipe) == -1)
		return ErrorPageGenerator::GenerateErrorResponse(response.internalServerError());
	num_fork = fork();
	if (num_fork == -1)
		return (ErrorPageGenerator::GenerateErrorResponse(response.internalServerError()));
	if (num_fork == 0)
	{
		dup2(server_to_cgi_pipe[0], 0);
		dup2(cgi_to_server_pipe[1], 1);
		close(server_to_cgi_pipe[0]);
		close(server_to_cgi_pipe[1]);
		close(cgi_to_server_pipe[0]);
		close(cgi_to_server_pipe[1]);
		env = create_env();
		command = create_command(this->file_path, this->file_name);
		execve(PATH_INFO, command, env);
		perror("execve cgi error");
		exit(127);
	}
	signal(SIGINT, SIG_IGN);
	if (method == "POST")
	{
		write(server_to_cgi_pipe[1], body.c_str(), body.size());
	}
	close(server_to_cgi_pipe[0]);
	close(server_to_cgi_pipe[1]);
	close(cgi_to_server_pipe[1]);

	// TIMEOUT
	int waited_ms = 0;
	const int timeout_ms = 10000;
	while (waited_ms < timeout_ms)
	{
		pid_t result = waitpid(num_fork, &status, WNOHANG);
		if (result == 0) {
			usleep(10000); // 10ms
			waited_ms+= 10;
		} else {
			break;
		}
	}
	if (waited_ms >= timeout_ms)
	{
		kill(num_fork, SIGKILL);
		close(cgi_to_server_pipe[0]);
		waitpid(num_fork, &status, 0);
		signal(SIGINT, SIG_DFL);
		return (ErrorPageGenerator::GenerateErrorResponse(response.gatewayTimeout()));
	}
	
	else if (WIFSIGNALED(status))
	{
		close(cgi_to_server_pipe[0]);
		signal(SIGINT, SIG_DFL);
		return (ErrorPageGenerator::GenerateErrorResponse(response.gatewayTimeout()));
	}
	if (WIFEXITED(status))
	{
		if (WEXITSTATUS(status) != 0)
		{
			close(cgi_to_server_pipe[0]);
			signal(SIGINT, SIG_DFL);
			return (ErrorPageGenerator::GenerateErrorResponse(response.internalServerError()));
		}
	}
	response = build_the_response(cgi_to_server_pipe[0]);
	waitpid(num_fork, &status, 0);
	signal(SIGINT, SIG_DFL);
	return (response);
}

Cgis::Cgis( std::string method, std::string file_path, std::string file_name,
		std::string content_type, std::string boundary, std::string content_lenght,
		std::string body)
{
	this->method = method;
	this->file_path = file_path;
	this->file_name = file_name;
	this->content_type = content_type;
	this->boundary = boundary;
	this->content_lenght = content_lenght;
	this->body = body;
	// Logger::instance().debug("CONSTRUCTOR ==> method: |" + method + "|");
	// Logger::instance().debug("CONSTRUCTOR ==> file_path: |" + file_path + "|");
	// Logger::instance().debug("CONSTRUCTOR ==> file_name: |" + file_name + "|");
	// Logger::instance().debug("CONSTRUCTOR ==> content_type: |" + content_type + "|");
	// Logger::instance().debug("CONSTRUCTOR ==> boundary: |" + boundary + "|");
	// Logger::instance().debug("CONSTRUCTOR ==> content_lenght: |" + content_lenght + "|");
	// Logger::instance().debug("CONSTRUCTOR ==> body: |" + body + "|");
	//Logger::instance().debug("CONSTRUCTOR ==> chunked: " + chunked);
}

Cgis::~Cgis()
{
}
