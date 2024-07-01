#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include "rtd_generated.h"

#define PORT 8083

int main() {
    int sockfd;
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


    char buffer[1024];
    socklen_t len = sizeof(cliaddr);
    int n = recvfrom(sockfd, buffer, 1024, MSG_WAITALL, (struct sockaddr *)&cliaddr, &len);

    // Десериализация данных
    flatbuffers::FlatBufferBuilder builder(1024);

    // Создание данных для каждого мотора
    std::vector<robot::rtd::MotorData> motor_data;
    motor_data.push_back(robot::rtd::MotorData(1, 36.5f, 10.0f, 1500.0f, 50.0f, true));
    motor_data.push_back(robot::rtd::MotorData(2, 37.0f, 15.0f, 1600.0f, 55.0f, true));
    motor_data.push_back(robot::rtd::MotorData(3, 35.5f, 20.0f, 1400.0f, 45.0f, false));

    // Сериализация массива данных моторов
    auto motors = builder.CreateVectorOfStructs(motor_data);

    // Временная метка и версия данных
    auto version = 1;
    auto timestamp = 1623847200;

    // Создание RTD
    robot::rtd::RTDBuilder rtd_builder(builder);
    rtd_builder.add_version(version);
    rtd_builder.add_timestamp(timestamp);
    rtd_builder.add_motors(motors);
    auto rtd = rtd_builder.Finish();

    // Завершение построения FlatBuffer
    builder.Finish(rtd);

    // Отправка данных через UDP
    sendto(sockfd, builder.GetBufferPointer(), builder.GetSize(), MSG_CONFIRM, (const struct sockaddr*)&cliaddr, sizeof(cliaddr));

    close(sockfd);
    return 0;
}
