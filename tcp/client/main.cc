#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "commands_generated.h"

#define PORT 8085

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;

    // Создание сокета
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cerr << "Socket creation error" << std::endl;
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Преобразование адреса IPv4 и IPv6 из текстовой формы в двоичную
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        std::cerr << "Invalid address/ Address not supported" << std::endl;
        return -1;
    }

    // Подключение к серверу
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cerr << "Connection Failed" << std::endl;
        return -1;
    }

    // // Сериализация текстового сообщения
    flatbuffers::FlatBufferBuilder builder(1024);

    auto stop_command = robot::command::CreateStopCommand(builder);
    auto stop_data = robot::command::CreateCommand(builder, robot::command::CommandType_StopCommand, 0UL, robot::command::CommandData_StopCommand, stop_command.Union());
    builder.Finish(stop_data);

    // Отправка данных
    send(sock, builder.GetBufferPointer(), builder.GetSize(), 0);
    std::cout << "Text Message sent" << std::endl;

    close(sock);
    return 0;
}
