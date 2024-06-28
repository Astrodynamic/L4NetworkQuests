#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
// #include "schema_generated.h"

#define PORT 8080

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
    // flatbuffers::FlatBufferBuilder builder(1024);
    // auto text = builder.CreateString("Hello, World!");
    // auto text_message = example::CreateTextMessage(builder, text);
    // auto message_content = example::CreateMessageContent(builder, example::MessageContent::TextMessage, text_message.Union());
    // auto message = example::CreateMessage(builder, 1, message_content);
    // builder.Finish(message);

    // // Отправка данных
    // send(sock, builder.GetBufferPointer(), builder.GetSize(), 0);
    // std::cout << "Text Message sent" << std::endl;

    close(sock);
    return 0;
}
