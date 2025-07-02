# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <fcntl.h>
# include <unistd.h>
# include <stdint.h>
# include <sys/wait.h>

#include <iostream>

// POST /upload.php HTTP/1.1
// Host: example.com
// Content-Type: multipart/form-data; boundary=---XYZ
// Content-Length: 145

// ---XYZ
// Content-Disposition: form-data; name="file"; filename="test.txt"
// Content-Type: text/plain

// Hello from the file!
// ---XYZ--


// PATH_INFO=/full/path/to/script.php
// SCRIPT_FILENAME=/path/to/script.php

int main(int argc, char **argv, char **env)
{
	pid_t		num_fork;
	std::string	php_path = "/home/isainz-r/Webserv/cgis/file.php";
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
			const_cast<char*>("PATH_INFO=/home/isainz-r/Webserv/cgis/file.php"),
			const_cast<char*>("SCRIPT_FILENAME=/home/isainz-r/Webserv/cgis/file.php"),
			const_cast<char*>("SCRIPT_NAME=/file.php"),
			const_cast<char*>("SERVER_PROTOCOL=HTTP/1.1"),
			const_cast<char*>("CONTENT_TYPE=multipart/form-data; boundary=XYZ"),
			const_cast<char*>("CONTENT_LENGTH=145"),
			const_cast<char*>("REDIRECT_STATUS=200"),
			NULL
		};

		// execve args
		char* argv[] = {
			const_cast<char*>("/usr/bin/php-cgi"),
			const_cast<char*>("/home/isainz-r/Webserv/cgis/file.php"),
			NULL
		};

		//chdir if needed
		
		execve(cgi_path.c_str(), argv, envp);
		perror("execve cgi error");
		exit(1);
	}

	// If POST: write body to child stdin
	std::string body =
"--XYZ\r\n"
"Content-Disposition: form-data; name=\"file\"; filename=\"test.txt\"\r\n"
"Content-Type: text/plain\r\n"
"\r\n"
"Hello from the file!\r\n"
"--XYZ--\r\n";

	write(server_to_cgi_pipe[1], body.c_str(), body.size());

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