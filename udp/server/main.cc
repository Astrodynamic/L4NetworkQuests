#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
// #include "schema_generated.h"

#define PORT 8080

int main() {
    int sockfd;
    char buffer[1024];
    struct sockaddr_in servaddr, cliaddr;

    // Создание сокета
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    std::memset(&servaddr, 0, sizeof(servaddr));
    std::memset(&cliaddr, 0, sizeof(cliaddr));

    // Привязка сокета к порту
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);

    if (bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("bind failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // socklen_t len = sizeof(cliaddr);
    // int n = recvfrom(sockfd, buffer, 1024, MSG_WAITALL, (struct sockaddr *)&cliaddr, &len);

    // // Десериализация данных
    // auto message = example::GetMessage(buffer);
    // std::cout << "Received message: " << message->text()->c_str() << std::endl;

    close(sockfd);
    return 0;
}
