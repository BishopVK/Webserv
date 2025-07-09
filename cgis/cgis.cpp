#include <iostream>
#include <unistd.h>
#include <vector>
#include <stdlib.h>
# include <sys/wait.h>
#include <string.h>

char **create_command(std::string file_path, std::string file_name)
{
	std::vector<std::string> command_vec;
	command_vec.push_back("/usr/bin/php-cgi");
	command_vec.push_back(file_path + file_name);

	char	**command = new char*[command_vec.size() + 1];
	for (size_t i = 0; i < command_vec.size(); i++)
	{
		command[i] = strdup(command_vec[i].c_str());
	}
	command[command_vec.size()] = NULL;
	return (command);
}

char **create_env(std::string method, std::string file_path,
	std::string file_name, std::string body, std::string type,
	std::string boundary, std::string length)
{
	std::vector<std::string> env_vec;
	env_vec.push_back("GATEWAY_INTERFACE=CGI/1.1");
	env_vec.push_back("REQUEST_METHOD=" + method);	
	env_vec.push_back("PATH_INFO=" + file_path + file_name);
	env_vec.push_back("SCRIPT_FILENAME=" + file_path + file_name);
	env_vec.push_back("SCRIPT_NAME=" + file_name);
	if (method == "GET")
		env_vec.push_back("QUERY_STRING=" + body);
	env_vec.push_back("SERVER_PROTOCOL=HTTP/1.1");
	if (method == "POST")
	{
		std::string content_type = "CONTENT_TYPE=" + type;
		if (!(boundary.empty()))
			content_type  += "; boundary=" + boundary;
		env_vec.push_back(content_type);
	}
	env_vec.push_back("CONTENT_LENGTH=" + length);
	env_vec.push_back("REDIRECT_STATUS=200");

	char	**env = new char*[env_vec.size() + 1];
	for (size_t i = 0; i < env_vec.size(); i++)
	{
		env[i] = strdup(env_vec[i].c_str());
	}
	env[env_vec.size()] = NULL;
	return (env);
}

int	main(void)
{
	std::string	method = "POST";
	std::string	file_path = "/home/isainz-r/Webserv/cgis/";
	std::string	file_name = "a_cgi.php";
	//std::string	file_name = "file.php";
	std::string	content_type = "application/x-www-form-urlencoded";
	//std::string	content_type = "multipart/form-data";
	std::string	content_lenght = "10";
	std::string	body = "age=20";
	//std::string	body = "--XYZ\r\n"
	//					"Content-Disposition: form-data; name=\"file\"; filename=\"test.txt\"\r\n"
	//					"Content-Type: text/plain\r\n"
	//					"\r\n"
	//					"Hello from the file!\r\n"
	//					"--XYZ--\r\n";
	std::string	boundary = "";

	pid_t		num_fork;
	int			server_to_cgi_pipe[2];
	int			cgi_to_server_pipe[2];
	char **env;
	char **command;

	pipe(server_to_cgi_pipe);
	pipe(cgi_to_server_pipe);
	num_fork = fork();
	if (num_fork == -1)
		return (-1);
	if (num_fork == 0)
	{
		dup2(server_to_cgi_pipe[0], 0);
		dup2(cgi_to_server_pipe[1], 1);
		close(server_to_cgi_pipe[0]);
		close(server_to_cgi_pipe[1]);
		close(cgi_to_server_pipe[0]);
		close(cgi_to_server_pipe[1]);
		env = create_env(method, file_path, file_name, body,
		content_type, boundary, content_lenght);
		command = create_command(file_path, file_name);

		// for (size_t i = 0; env[i]; i++)
		// {
		// 	printf("->%s\n", env[i]);
		// }
		// for (size_t i = 0; command[i]; i++)
		// {
		// 	printf("*>%s\n", command[i]);
		// }
		execve("/usr/bin/php-cgi", command, env);
		perror("execve cgi error");
		exit(127);
	}
	if (method == "POST")
	{
		write(server_to_cgi_pipe[1], body.c_str(), body.size());
	}

	close(server_to_cgi_pipe[0]);
	close(server_to_cgi_pipe[1]);
	close(cgi_to_server_pipe[1]);

	char buffer[4096];
	int n = 0;
	while ((n = read(cgi_to_server_pipe[0], buffer, sizeof(buffer))) > 0)
	{
		std::cout.write(buffer, n);
	}
	close(cgi_to_server_pipe[0]);
	waitpid(num_fork, NULL, 0);
	return (0);
}