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
    auto command = flatbuffers::GetRoot<robot::command::Command>(buffer);
    auto timestamp = command->timestamp();

    if (command->data_type() == robot::command::CommandData_MoveCommand) {
        std::cout << "Received Move Command: " << std::endl;

        auto move_command = command->data_as<robot::command::MoveCommand>();
        if (move_command->move_type() == robot::command::MoveData_C_Space) {
            auto c_space = move_command->move_as<robot::command::C_Space>();
            auto position = c_space->position();
            auto orientation = c_space->orientation();
            std::cout << "Position: " << position.x() << ", " << position.y() << ", " << position.z() << std::endl;
            std::cout << "Orientation: " << orientation.x() << ", " << orientation.y() << ", " << orientation.z() << std::endl;
        }
    } else if (command->data_type() == robot::command::CommandData_StopCommand) {
        std::cout << "Received Stop Command: " << std::endl;
        auto stop_command = command->data_as<robot::command::StopCommand>();
    }

    close(new_socket);
    close(server_fd);
    return 0;
}
