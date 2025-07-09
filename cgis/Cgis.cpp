#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include "Cgis.hpp"

char **Cgis::create_command(std::string file_path, std::string file_name)
{
	std::vector<std::string> command_vec;
	command_vec.push_back(PATH_INFO);
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

void Cgis::execute()
{
	pid_t		num_fork;
	int			server_to_cgi_pipe[2];
	int			cgi_to_server_pipe[2];
	char **env;
	char **command;

	pipe(server_to_cgi_pipe);
	pipe(cgi_to_server_pipe);
	num_fork = fork();
	if (num_fork == -1)
		return ;
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
	if (method == "POST")
		write(server_to_cgi_pipe[1], body.c_str(), body.size());
	close(server_to_cgi_pipe[0]);
	close(server_to_cgi_pipe[1]);
	close(cgi_to_server_pipe[1]);
	char buffer[4096];
	int n = 0;
	while ((n = read(cgi_to_server_pipe[0], buffer, sizeof(buffer))) > 0)
		std::cout.write(buffer, n);
	close(cgi_to_server_pipe[0]);
	waitpid(num_fork, NULL, 0);
}