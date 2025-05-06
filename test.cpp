// simple_server.cpp
#include <iostream>
#include <cstring>      // memset
#include <unistd.h>     // close
#include <sys/socket.h> // socket, bind, listen, accept
#include <netinet/in.h> // sockaddr_in
#include <arpa/inet.h>  // htons

int main() {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        std::cerr << "Socket creation failed\n";
        return 1;
    }
    sockaddr_in server_addr;
    std::memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY; // 0.0.0.0（全てのIPアドレスから受け付ける）
    server_addr.sin_port = htons(12345);       // ポート12345番を使う
    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        std::cerr << "Bind failed\n";
        close(server_fd);
        return 1;
    }
    if (listen(server_fd, 1) == -1) {
        std::cerr << "Listen failed\n";
        close(server_fd);
        return 1;
    }
    std::cout << "Waiting for a connection on port 12345...\n";
    sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    int client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);
    if (client_fd == -1) {
        std::cerr << "Accept failed\n";
        close(server_fd);
        return 1;
    }
    std::cout << "Client connected!\n";
    const char* message = "Hello from server!\n";
    send(client_fd, message, strlen(message), 0);
    close(client_fd);
    close(server_fd);
    return 0;
}
