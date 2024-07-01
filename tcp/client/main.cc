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

    // Пример создания MoveCommand с использованием C_Space
    robot::command::Vector3D position(1.0, 2.0, 3.0);
    robot::command::Vector3D orientation(0.0, 1.0, 0.0);
    auto c_space = robot::command::C_Space(position, orientation);

    // Упаковка структуры C_Space в FlatBuffers
    auto c_space_offset = builder.CreateStruct(c_space);

    robot::command::MoveCommandBuilder move_command_builder(builder);
    move_command_builder.add_move_type(robot::command::MoveData_C_Space);
    move_command_builder.add_move(c_space_offset.Union());
    auto move_command = move_command_builder.Finish();

    // Создание команды Command с использованием MoveCommand
    robot::command::CommandBuilder command_builder(builder);
    command_builder.add_timestamp(0UL);
    command_builder.add_data_type(robot::command::CommandData_MoveCommand);
    command_builder.add_data(move_command.Union());
    auto command = command_builder.Finish();

    // Завершение построения FlatBuffer
    builder.Finish(command);

    // Отправка данных
    send(sock, builder.GetBufferPointer(), builder.GetSize(), 0);
    std::cout << "Text Message sent" << std::endl;

    close(sock);
    return 0;
}
