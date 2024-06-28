#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include "commands_generated.h"

#define PORT 8085

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    // Создание сокета
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Привязка сокета к порту
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
        perror("accept");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Получение данных
    char buffer[1024] = {0};
    int valread = read(new_socket, buffer, 1024);

    // // Десериализация данных
    auto command = robot::command::GetCommand(buffer);
    if (command->type() == robot::command::CommandType_MoveCommand) {
        // auto move_command = command->data_as<robot::command::MoveCommand>();
        // if (move_command->move_type() == robot::command::MoveType_MOVE_L) {
        //     std::cout << "Received Move Command: " << move_command->move_type() << std::endl;
        // } else if (move_command->move_type() == robot::command::MoveType_MOVE_G) {
        //     std::cout << "Received Move Command: " << move_command->move_type() << std::endl;
        // }
    } else if (command->type() == robot::command::CommandType_StopCommand) {
        auto stop_command = command->data_as<robot::command::StopCommand>();
        std::cout << "Received Stop Command: " << std::endl;
    }

    close(new_socket);
    close(server_fd);
    return 0;
}
