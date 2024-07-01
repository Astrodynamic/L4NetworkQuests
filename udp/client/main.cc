#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include "rtd_generated.h"

#define PORT 8083

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

    // Отправка сообщения серверу
    const char* message = "Hello from client";
    sendto(sockfd, message, strlen(message), MSG_CONFIRM, (const struct sockaddr*)&servaddr, sizeof(servaddr));

    std::byte buffer[1024];
    
    // Ожидание данных от сервера
    socklen_t len;
    int n = recvfrom(sockfd, (char*)buffer, 1024, MSG_WAITALL, (struct sockaddr*)&servaddr, &len);

    // Десериализация данных FlatBuffers
    auto rtd = flatbuffers::GetRoot<robot::rtd::RTD>(buffer);

    // Обработка и вывод полученных данных
    std::cout << "Version: " << rtd->version() << std::endl;
    std::cout << "Timestamp: " << rtd->timestamp() << std::endl;

    auto motors = rtd->motors();
    for (auto motor : *motors) {
        std::cout << "Motor ID: " << motor->id()
                  << ", Temperature: " << motor->temperature()
                  << ", Angle: " << motor->angle()
                  << ", Speed: " << motor->speed()
                  << ", Torque: " << motor->torque()
                  << ", Status: " << (motor->status() ? "On" : "Off") << std::endl;
    }

    close(sockfd);
    return 0;
}
