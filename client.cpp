#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <string>

bool isOnlyDigits(std::string& s) {
    if (s.empty()) return false;
    for (char c : s) {
        if (!std::isdigit(static_cast<unsigned char> (c))) return false;
    }
    return true;
}
int main(int args, char* argv[]) {
    if (args !=3) {
        std::cerr << "Usage: ./client <ip> <port>\n";
        return 1;
    }
    std::string ip = argv[1];
    std::string portStr= argv[2];

    if (!isOnlyDigits(portStr)) {
        std::cerr<<"Invalid port : not a number";
        return 1;
    }
    int port=atoi(portStr.c_str());
    if (port<1||port>65535) {
        std::cerr<<"Invalid port : out of range";
        return 1;
    }


    int client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (client_fd < 0) {
        perror("socket");
        return 1;
    }

    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);

   if (inet_pton(AF_INET, ip.c_str(), &server_addr.sin_addr) <= 0) {
       std::cerr<<"Invalid address/ Address not supported \n";
       return 1;
   }


    if (connect(client_fd, (sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("connect");
        close(client_fd);
        return 1;
    }

    std::cout << "Connected to server" <<ip << ":" << port << "\n";
    std::cout << "Type commands (HELLO, LIST, BORROW <id>, RETURN <id>, WAIT <id>, QUIT)\n";

    std::string line;
    char buffer[4096];

    while (true) {
        std::cout << "> ";
        std::getline(std::cin, line);

        if (line.empty())
            continue;

        send(client_fd, line.c_str(), line.length(), 0);

        int bytes = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
        if (bytes <= 0) {
            std::cout << "Server disconnected.\n";
            break;
        }

        buffer[bytes] = '\0';
        std::cout << buffer;

        if (line == "QUIT")
            break;
    }

    close(client_fd);
    return 0;
}