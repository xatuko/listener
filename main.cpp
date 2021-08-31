#include <iostream>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/poll.h>
#include <vector>
#include <iomanip>
#include <string>
#include <cstring>
#include <chrono>

#include "ctcpserver.h"
#include "carguments.h"

char hex_map[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

std::string bytearray2string(const ByteArray & arr, const int & n)
{
    std::string res = "";
    for (int i = 0; i < n; i++) {
        res += hex_map[int(arr[i] >> 4)];
        res += hex_map[int(arr[i] & 0x0F)];        
        res += ' ';
    }
    return res;
}

const arguments arg_set = {.args = {argument{"--ipaddr", "127.0.0.1", true},
                                    argument{"-p",       "5002",      true},
                                    argument{"--help",   "",          false}},
                           .size = 3};

int main(const int n, const char** args) {

    std::shared_ptr<IArguments> argctrl = std::make_shared<CArguments>();
    argctrl->init(arg_set);
    if (argctrl->setArgs(args, n) == -1)
        return -1;

    std::string ip;
    int port;
    argctrl->getString("--ipaddr", ip);
    argctrl->getInt("-p", port);

    std::shared_ptr<ITcpServer> sock = std::make_shared<CTcpServer>();
    sock->setAddr(ip, port);

    if (!sock->init())
        return -1;
    
    if (!sock->startListen())
        return -1;

    auto start_time = std::chrono::steady_clock::now();
    auto dur = std::chrono::minutes(1);
    ByteArray message;
    while(std::chrono::steady_clock::now() < start_time + dur)
    {
        ByteArray message(256);
        if (sock->getMessage(message))
            std::cout << "Message: " << bytearray2string(message, message.size()) << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }

    return 0;
    
}
