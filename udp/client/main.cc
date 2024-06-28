#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
// #include "schema_generated.h"

#define PORT 8080

int main() {
    int sockfd;
    struct sockaddr_in servaddr;

    // Создание сокета
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    std::memset(&servaddr, 0, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = INADDR_ANY;

    // // Сериализация данных
    // flatbuffers::FlatBufferBuilder builder(1024);
    // auto text = builder.CreateString("Hello, World!");
    // auto message = example::CreateMessage(builder, 1, text);
    // builder.Finish(message);

    // // Отправка данных
    // sendto(sockfd, builder.GetBufferPointer(), builder.GetSize(), MSG_CONFIRM, (const struct sockaddr *)&servaddr, sizeof(servaddr));
    // std::cout << "Message sent" << std::endl;

    close(sockfd);
    return 0;
}
