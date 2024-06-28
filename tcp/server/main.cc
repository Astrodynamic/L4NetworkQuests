#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
// #include "schema_generated.h"

#define PORT 8080

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
    // int valread = read(new_socket, buffer, 1024);

    // // Десериализация данных
    // auto message = example::GetMessage(buffer);
    // if (message->content_type() == example::MessageContent::TextMessage) {
    //     auto text_message = message->content_as_TextMessage();
    //     std::cout << "Received Text Message: " << text_message->text()->c_str() << std::endl;
    // } else if (message->content_type() == example::MessageContent::ImageMessage) {
    //     auto image_message = message->content_as_ImageMessage();
    //     std::cout << "Received Image Message: " << image_message->url()->c_str() << " (" 
    //               << image_message->width() << "x" << image_message->height() << ")" << std::endl;
    // }

    close(new_socket);
    close(server_fd);
    return 0;
}
