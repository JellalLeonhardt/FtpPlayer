#include <ftpmanager.hpp>

using namespace std;

ftpManager::ftpManager(std::string &name, std::string &passwd, std::string &ip, short port)
    :user_name(name),
      user_passwd(passwd),
      ftp_ip(ip),
      ftp_port(port),
      recvbuf(new char[4096]),
      recv_max(4096)
{
    WSAData wsa;
    if(WSAStartup(MAKEWORD(2, 2), &wsa) == 0){
        cout<< "WSA init success" <<endl;
    }
    memset(&addrServer, 0, sizeof(sockaddr_in));
    addrServer.sin_family = AF_INET;
    addrServer.sin_port = htons(ftp_port);
    addrServer.sin_addr.S_un.S_addr = inet_addr(ftp_ip.c_str());
    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
}

ftpManager::~ftpManager()
{
    switch(status){
    case LOGIN:
        sprintf(sendbuf, "QUIT");
        send(sock, sendbuf, strlen(sendbuf) + 1, 0);
        break;
    default:;
    }

    WSACleanup();
}

bool ftpManager::login()
{
    int cnt = 0;
    if(connect(sock, (sockaddr *)&addrServer, sizeof(sockaddr)) == 0){ //成功建立连接
        if(220 == parseResponse()){
            while(1){
                sprintf(sendbuf, "USER %s\r\n", user_name.c_str());
                send(sock, sendbuf, strlen(sendbuf), 0);
                while(331 != parseResponse() && cnt < 10){
                    cnt++;
                    sprintf(sendbuf, "USER %s\r\n", user_name.c_str());
                    send(sock, sendbuf, strlen(sendbuf), 0);
                }
                sprintf(sendbuf, "PASS %s\r\n", user_passwd.c_str());
                send(sock, sendbuf, strlen(sendbuf), 0);
                if(230 == parseResponse()){
                    status = LOGIN;
                    cout<< "login success" <<endl;
                    return true;
                }
                else{
                    sprintf(sendbuf, "REIN\r\n");
                    send(sock, sendbuf, strlen(sendbuf), 0);
                    parseResponse();
                }
            }
        }
        cout<< "login failed" <<endl;
    }
    cout<< WSAGetLastError() <<endl;
    return false;
}

int ftpManager::parseResponse()
{
    int code;
    _cnt = recv(sock, recvbuf, recv_max, 0);
    recvbuf[_cnt] = 0;
    string recv(recvbuf);
    *(new stringstream(recv)) >> code;
    cout<< recv <<endl;

    switch(code){
    case 220:
        break;
    case 227:
    {
        int cnt = 0, port0, port1;
        for(int i = 0; i < 4;i++){
            cnt = recv.find(',', cnt + 1);
        }
        *(new stringstream(recv.substr(cnt + 1, recv.find(',', cnt + 1) - cnt - 1))) >> port0;
        *(new stringstream(recv.substr(recv.find(',', cnt + 1) + 1))) >> port1;
        passive_port = port0 * 256 + port1;
    }
        break;
    default:
        cout<< "unkown" <<endl;
        break;
    }

    return code;
}

string ftpManager::enterPassive(string op)
{
    sprintf(sendbuf, "PASV\r\n");
    send(sock, sendbuf, strlen(sendbuf), 0);
    if(227 == parseResponse()){
        SOCKET _sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        addrServer.sin_port = htons(passive_port);
        if(op.substr(0, 4) == "LIST"){
            sprintf(sendbuf, op.c_str());
        }
        else{
            sprintf(sendbuf, "NOPE\r\n");
        }
        send(sock, sendbuf, strlen(sendbuf), 0);
        if(connect(_sock, (sockaddr *)&addrServer, sizeof(sockaddr)) == 0){
            _cnt = recv(_sock, recvbuf, recv_max, 0);
            recv(sock, sendbuf, sizeof(sendbuf), 0);
            recv(sock, sendbuf, sizeof(sendbuf), 0);//为了不破坏recvbuf
            recvbuf[_cnt] = 0;
            return string(recvbuf);
        }
    }
    return "";
}

std::vector<string> ftpManager::getList(std::string dir)
{
    string list = enterPassive("LIST " + dir + "\r\n");
    vector<string> listSteam;
    stringstream listStream(list);
    string tmp;
    while(listStream >> tmp){
        char type = tmp.at(0);
        for(int i = 0; i < 7;i++){
            listStream >> tmp;
        }
        getline(listStream, tmp, '\r');
        //cout<< type + tmp <<endl; //查看读取情况
        listSteam.push_back(type + tmp);
    }

    return listSteam;
}

string ftpManager::getUrl(string where)
{
    return "ftp://" + user_name + ":" + user_passwd + "@" + ftp_ip + where;
}
