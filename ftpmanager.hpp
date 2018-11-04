#ifndef FTPMANAGER_HPP
#define FTPMANAGER_HPP

#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include <winsock2.h>
#include <windows.h>

class ftpManager
{
public:
    ftpManager() = default;
    ftpManager(std::string &name, std::string &passwd, std::string &ip, short port);
    ~ftpManager();
    bool login();
    bool bye();
    std::vector<std::string> getList(std::string dir);
    std::string getUrl(std::string where);

private:
    int parseResponse();
    std::string enterPassive(std::string op);

private:
    enum status{UNLOGIN = 0, LOGIN = 1};
    std::string user_name, user_passwd, ftp_ip;
    short ftp_port;
    SOCKET sock;
    sockaddr_in addrServer;
    char sendbuf[256];
    char *recvbuf;
    enum status status = UNLOGIN;
    short passive_port;
    int _cnt, recv_max;
    std::vector<std::string> listFiles;
};

#endif // FTPMANAGER_HPP
