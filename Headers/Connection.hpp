#pragma once
#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <utility>

class Connection {
public:
    static int OpenConnection(const char* address, unsigned short port, int type) {
        int sockfd = socket(type, SOCK_STREAM, 0);
        sockaddr_in sockAddress;
        memset(&sockAddress, 0, sizeof(sockAddress));
        inet_aton(address, &sockAddress.sin_addr);
        sockAddress.sin_port = htons(port);
        sockAddress.sin_family = type;
        if (connect(sockfd, (sockaddr*) &sockAddress, sizeof(sockaddr_in))) {
            std::cerr << "[Error]::Connection problem" << std::endl;
            exit(-1);
        }
        return sockfd;
    }

    static void CloseConnection(int sockfd) {
        close(sockfd);
    }

    static void SendData(int sockfd, const std::string &Buffer) {
        int sent = 0, total = Buffer.size(), bytes;
        const char* data = Buffer.data();
        while (total != 0) {
            bytes = send(sockfd,  (void*) (data + sent), total, 0);
            if (bytes < 0) {
                std::cerr << "[Error]::Send problem" << std::endl;
                exit(-1);
            }
            else if (bytes == 0) {
                std::cout << "Connection closed" << std::endl;
                return;
            }
            total -= bytes  ,   sent += bytes;
        }
    }

    static std::string ReceiveHttps(int sockfd) {
        std::string result = "", piece(256,'\0');
        int bytes, dataStart;
        while(true) {
            bytes = recv(sockfd, (void *) piece.data(), 256, 0);
            piece.resize(bytes);
            if (bytes < 0) {
                std::cerr << "[Error]::Receive problem" << std::endl;
                exit(-1);
            }
            else if (bytes == 0) {
                std::cout << "Connection close" << std::endl;
                return "";
            }
            result.append(piece);
            if ((dataStart = result.rfind("\r\n\r\n")) != std::string::npos) {
                dataStart += 4;
                break;
            }
        }
        int posStart, posEnd;
        //std::cout << result << std::endl;
        if ((posStart = result.find("Content-Length")) == std::string::npos) {
            return result;
        }
        posStart += strlen("Content-Length") + 1;
        if ((posEnd = result.find('\r',posStart)) == std::string::npos) {
            std::cerr << "[Error]::Content-Length wrong header" << std::endl;
            return "";
        }
        int contentLength = std::stoi(result.substr(posStart, posEnd - posStart));
        int remainedBytes = contentLength - (result.length() - dataStart);
        while (remainedBytes != 0) {
            bytes = recv(sockfd, (void *) piece.data(), 256, 0);
            piece.resize(bytes);
            result.append(piece);
            remainedBytes -= bytes;
        }
        return result;
    }
};