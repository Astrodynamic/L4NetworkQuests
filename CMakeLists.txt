cmake_minimum_required(VERSION 3.10)
project(minimal_project)

set(CMAKE_CXX_STANDARD 11)

# Путь к FlatBuffers
set(FLATBUFFERS_DIR "/flatbuffers")

include_directories(${FLATBUFFERS_DIR}/include)
link_directories(${FLATBUFFERS_DIR}/lib)

# Генерация FlatBuffers кода
add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/schema_generated.h
    COMMAND ${FLATBUFFERS_DIR}/flatc --cpp ${CMAKE_CURRENT_SOURCE_DIR}/flatbuffers/schema.fbs
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/flatbuffers/schema.fbs
)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

# Добавление исходных файлов и зависимостей
add_executable(server_tcp tcp/server/main.cc ${CMAKE_CURRENT_BINARY_DIR}/schema_generated.h)
target_link_libraries(server_tcp flatbuffers)

add_executable(client_tcp tcp/client/main.cc ${CMAKE_CURRENT_BINARY_DIR}/schema_generated.h)
target_link_libraries(client_tcp flatbuffers)

add_executable(server_udp udp/server/main.cc ${CMAKE_CURRENT_BINARY_DIR}/schema_generated.h)
target_link_libraries(server_udp flatbuffers)

add_executable(client_udp udp/client/main.cc ${CMAKE_CURRENT_BINARY_DIR}/schema_generated.h)
target_link_libraries(client_udp flatbuffers)
