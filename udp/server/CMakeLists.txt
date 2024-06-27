project(udp_server VERSION 0.0.1 LANGUAGES CXX)

set(HEADERS
)

set(SOURCES
)

add_executable(${PROJECT_NAME}
    ${HEADERS}
    ${SOURCES}
    main.cc
)

target_compile_features(${PROJECT_NAME}
    PRIVATE
    cxx_std_20
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
)