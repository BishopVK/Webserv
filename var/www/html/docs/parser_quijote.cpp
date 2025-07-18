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
#include <algorithm>
#include <cctype> // std::isgigit()
#include <fcntl.h>



int main()
{
	std::ifstream	fd;
	std::string line = "";
	fd.open("quijote.txt", std::ios::in);
	while (std::getline(fd, line))
	{
		line = "\"" + line + "\"\n";
		std::cout << line;
	}
}