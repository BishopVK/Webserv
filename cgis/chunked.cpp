# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <fcntl.h>
# include <unistd.h>
# include <stdint.h>
# include <sys/wait.h>

#include <iostream>

// POST /upload.php HTTP/1.1
// Host: localhost
// Transfer-Encoding: chunked
// Content-Type: application/x-www-form-urlencoded

// 4\r\n
// name\r\n
// 1\r\n
// =\r\n
// 4\r\n
// john\r\n
// 0\r\n
// \r\n


// que se convierte en:
// POST /upload.php HTTP/1.1
// Host: localhost
// Content-Length: 9
// Content-Type: application/x-www-form-urlencoded

// name=john


int main(int argc, char **argv, char **env)
{
	pid_t		num_fork;
	std::string	php_path = "/home/isainz-r/Webserv/tests/a_cgi.php";
	std::string	cgi_path = "/usr/bin/php-cgi";
	int			server_to_cgi_pipe[2]; 
	int			cgi_to_server_pipe[2]; 

	pipe(server_to_cgi_pipe);
	pipe(cgi_to_server_pipe);
	num_fork = fork();
	if (num_fork == 0)
	{
		dup2(server_to_cgi_pipe[0], 0);
		dup2(cgi_to_server_pipe[1], 1);

		close(server_to_cgi_pipe[0]);
		close(server_to_cgi_pipe[1]);
		close(cgi_to_server_pipe[0]);
		close(cgi_to_server_pipe[1]);

		// content_lenght está en bytes
		// Setup environment variables for CGI  COMO ES POST NECESITA CONTENT_TYPE
		//name=ines&age=20
		char* envp[] = {
			const_cast<char*>("GATEWAY_INTERFACE=CGI/1.1"),
			const_cast<char*>("REQUEST_METHOD=POST"),
			const_cast<char*>("PATH_INFO=/home/isainz-r/Webserv/tests/a_cgi.php"),
			const_cast<char*>("SCRIPT_FILENAME=/home/isainz-r/Webserv/tests/a_cgi.php"),
			const_cast<char*>("SCRIPT_NAME=/a_cgi.php"),
			const_cast<char*>("SERVER_PROTOCOL=HTTP/1.1"),
			const_cast<char*>("CONTENT_TYPE=application/x-www-form-urlencoded"),
			const_cast<char*>("CONTENT_LENGTH=10"),
			const_cast<char*>("REDIRECT_STATUS=200"),
			NULL
		};

		// execve args
		char* argv[] = {
			const_cast<char*>("php-cgi"),
			const_cast<char*>("/home/isainz-r/Webserv/tests/a_cgi.php"),
			NULL
		};

		//chdir if needed
		
		execve(cgi_path.c_str(), argv, envp);
		perror("execve cgi error");
		exit(1);
	}

	// If POST: write body to child stdin
	write(server_to_cgi_pipe[1], "age=20", 6);

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