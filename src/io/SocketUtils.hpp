#pragma once

class SocketUtils
{
  public:
    static bool setNonBlocking(int fd);
    static bool setReuseAddr(int fd);
    static int  createServerSocket(const char* ip, const char* port);
    //static int  createServerSocket(const char* port);
    static void closeSocket(int fd);
};
