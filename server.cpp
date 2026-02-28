#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <thread>
#include <string>
#include <cstdio>
#include "InventoryManager.h"
#include <sstream>
#include <cctype>


InventoryManager manager;

void handle_client(int client_fd) {
    std::cout << "Client connected (fd=" << client_fd << ")" << std::endl;

    bool loggedIn = false;
    std::string username;

    char buffer[1024];

    while (true) {
        int bytes = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
        if (bytes <= 0) {
            break;
        }

        buffer[bytes] = '\0';
        std::string request(buffer);
        std::string response;

        try {
            std::istringstream iss(request);
            std::string command;
            iss >> command;

            if (request == "QUIT") {
                response = "OK BYE\n";
                send(client_fd, response.c_str(), response.length(), 0);
                break;
            }


            if (!loggedIn) {
                if (command != "HELLO") {
                    throw std::runtime_error("STATE not_authenticated");
                }

                iss >> username;
                if (username.empty()) {
                    throw std::runtime_error("PROTOCOL missing_username");
                }

                for (char c : username) {
                    if (!isalpha(c)) {
                        throw std::runtime_error("PROTOCOL invalid_username");
                    }
                }

                loggedIn = true;
                response = "OK HELLO " + username + "\n" ;
            }


            else {
                if (command == "LIST") {
                    response = manager.listItems();
                }

                else if (command == "BORROW") {
                    std::string id_str;
                    iss >> id_str;

                    if (id_str.empty())
                        throw std::runtime_error("PROTOCOL missing_item_id");

                    for (char c : id_str)
                        if (!isdigit(c))
                            throw std::runtime_error("PROTOCOL invalid_id");

                    int item_id = std::stoi(id_str);
                    manager.borrowItem(item_id, username);
                    response = "OK BORROW " + id_str+ "\n";
                }

                else if (command == "RETURN") {
                    std::string id_str;
                    iss >> id_str;

                    if (id_str.empty())
                        throw std::runtime_error("PROTOCOL missing_item_id");

                    for (char c : id_str)
                        if (!isdigit(c))
                            throw std::runtime_error("PROTOCOL invalid_id");

                    int item_id = std::stoi(id_str);
                    manager.returnItem(item_id, username);
                    response = "OK RETURN " + id_str + "\n";
                }

                else if (command == "WAIT") {
                    std::string id_str;
                    iss >> id_str;

                    if (id_str.empty())
                        throw std::runtime_error("PROTOCOL missing_item_id");

                    for (char c : id_str)
                        if (!isdigit(c))
                            throw std::runtime_error("PROTOCOL invalid_id");

                    int item_id = std::stoi(id_str);
                    manager.waitUntilAvailable(item_id, username);
                    response = "OK AVAILABLE " + id_str +"\n";
                }

                else {
                    throw std::runtime_error("PROTOCOL invalid_command");
                }
            }
        }
        catch (const std::exception& e) {
            response = std::string("ERR ") + e.what() + "\n";
        }

        send(client_fd, response.c_str(), response.length(), 0);
    }

    close(client_fd);
    std::cout << "Client disconnected (fd=" << client_fd << ")" << std::endl;
}
bool isOnlyDigits(std::string& s) {
    if (s.empty()) return false;
    for (char c : s) {
        if (!std::isdigit(static_cast<unsigned char> (c))) return false;
    }
    return true;
}

/**
 * Program entry point.
 *
 * Creates a TCP server socket, binds it to port 5555,
 * listens for incoming connections, and spawns a new
 * thread for each connected client.
 */
int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr<<"Usage: ./server <port>\n";
        return 1;
    }
    std::string portStr=argv[1];
    if (!isOnlyDigits(portStr)) {
        std::cerr<<"Invalid port : not a number";
        return 1;
    }
    int port=atoi(portStr.c_str());
    if (port<1||port>65535) {
        std::cerr<<"Invalid port : out of range";
        return 1;
    }

    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("socket");
        return 1;
    }

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);

    if (bind(server_fd, (sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("bind");
        return 1;
    }

    if (listen(server_fd, 5) < 0) {
        perror("listen");
        return 1;
    }

    std::cout << "Server listening on port " << port<< std::endl;

    while (true) {
        int client_fd = accept(server_fd, nullptr, nullptr);
        if (client_fd < 0) {
            perror("accept");
            continue;
        }
        std::thread(handle_client, client_fd).detach();
    }
}